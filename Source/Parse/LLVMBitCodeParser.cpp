import std.core;

#include "LLVMBitCodeParser.h"

using namespace Soup;

enum class AbbreviationId : char
{
	// This abbrev ID marks the end of the current block.
	EndBlock = 0,
	// This abbrev ID marks the beginning of a new block.
	EnterSubBlock = 1,
	// This defines a new abbreviation.
	DefineAbbreviation = 2,
	// This ID specifies the definition of an unabbreviated record.
	UnabbreviatedRecord = 3,
};

enum class StandardBlockId : char
{
	BlockInfo = 0,
};

template<std::size_t N>
void ReadBytes(std::istream& stream, std::array<char, N>& result)
{
	if (!stream.read(result.data(), result.size()))
		throw std::runtime_error("Failed read.");
}

char ReadByte(std::istream& stream)
{
	char result;
	if (!stream.read(&result, 1))
		throw std::runtime_error("Failed read.");
	return result;
}

void LLVMBitCodeParser::Parse(std::istream& stream)
{
	std::cout << "Parse" << std::endl;

	// Verify this is Clang Serialized AST Bitstream
	std::array<char, 4> headerSignature;
	ReadBytes(stream, headerSignature);
	if (headerSignature[0] != 'C' ||
		headerSignature[1] != 'P' ||
		headerSignature[2] != 'C' ||
		headerSignature[3] != 'H')
	{
		throw std::runtime_error("File signture was not expected type.");
	}

	// Setup the initial context
	auto reader = BitReader(stream);
	size_t initialAbbreviationLength = 2;

	// Read until end of stream
	while (!reader.AtEndOfStream())
	{
		auto code = static_cast<AbbreviationId>(reader.Read(initialAbbreviationLength));
		switch (code)
		{
			case AbbreviationId::EnterSubBlock:
				ParseSubBlock(reader, initialAbbreviationLength);
				break;
			default:
				throw std::runtime_error(
					std::string("Unexpected AbbreviationId In Root: ") + std::to_string(static_cast<uint32_t>(code)));
		}
	}

	std::cout << "Done." << std::endl;
}

std::string_view LLVMBitCodeParser::GetBlockName(uint32_t blockId)
{
	if (blockId < 8)
	{
		switch (static_cast<StandardBlockId>(blockId))
		{
			case StandardBlockId::BlockInfo:
				return "BLOCK_INFO";
			default:
				return "UNKNOWN_STANDARD";
		}
	}
	else
	{
		auto blockInfoResult = std::find_if(
			m_blocks.begin(),
			m_blocks.end(),
			[blockId](const BlockInfo& value) { return value.Id == blockId; });
		if (blockInfoResult != m_blocks.end())
		{
			return blockInfoResult->Name;
		}
		else
		{
			return "UNKNOWN";
		}
	}
}

void LLVMBitCodeParser::ParseSubBlock(BitReader& reader, size_t abbreviationLength)
{
	auto blockId = reader.ReadVBR(8);
	auto newAbbreviationLength = reader.ReadVBR(4);
	reader.Align32Bit();
	auto blockLength = reader.Read(32);

	std::cout << "EnterSubBlock: " << GetBlockName(blockId) << std::endl;
	// std::cout << "BlockId:" << blockId << std::endl;
	// std::cout << "NewAbbreviationLength:" << newAbbreviationLength << std::endl;
	// std::cout << "BlockLength:" << blockLength << std::endl;

	// Update the current context
	abbreviationLength = newAbbreviationLength;

	if (blockId == 0)
	{
		ParseBlockInfo(reader, abbreviationLength);
	}
	else
	{
		// Skip unknown block
		std::cout << "Skip Block" << std::endl;
		reader.SeekByteOffset(blockLength * 4);
	}
}

enum class BlockRecordId
{
	SetBlockId = 1,
	BlockName = 2,
	SetRecordName = 3,
};

void LLVMBitCodeParser::ParseBlockInfo(BitReader& reader, size_t abbreviationLength)
{
	// Read until we hit an end block
	BlockInfo* activeBlock = nullptr;
	while (true)
	{
		auto code = static_cast<AbbreviationId>(reader.Read(abbreviationLength));
		switch (code)
		{
			case AbbreviationId::EnterSubBlock:
				ParseSubBlock(reader, abbreviationLength);
				break;
			case AbbreviationId::DefineAbbreviation:
				ParseDefineAbbreviation(reader);
				break;
			case AbbreviationId::UnabbreviatedRecord:
			{
				auto record = ParseUnabbreviatedRecord(reader);
				switch (static_cast<BlockRecordId>(record.Code))
				{
					case BlockRecordId::SetBlockId:
					{
						if (record.Operands.size() != 1)
							throw std::runtime_error("A SetBlockId record must have exactly one operand.");
						
						// Create the new block info
						m_blocks.emplace_back();
						activeBlock = &*std::prev(m_blocks.end());
						activeBlock->Id = record.Operands[0];
						break;
					}
					case BlockRecordId::BlockName:
					{
						if (activeBlock == nullptr)
							throw std::runtime_error("Cannot set BlockName without an active block.");
						activeBlock->Name = std::string(record.Operands.begin(), record.Operands.end());
						break;
					}
					case BlockRecordId::SetRecordName:
					{
						if (activeBlock == nullptr)
							throw std::runtime_error("Cannot set RecordName without an active block.");
						if (record.Operands.size() < 1)
							throw std::runtime_error("Set RecordName must have at least one operand.");
						auto recordId = record.Operands[0];
						auto recordName = std::string(record.Operands.begin()+1, record.Operands.end());
						break;
					}
					default:
					{
						throw std::runtime_error("Unknown BlockInfo Record.");
					}
				}
				break;
			}
			case AbbreviationId::EndBlock:
				// std::cout << "EndBlock" << std::endl;
				reader.Align32Bit();
				return;
			default:
				throw std::runtime_error(
					std::string("Unexpected AbbreviationId In SubBlock: ") + std::to_string(static_cast<uint32_t>(code)));
		}
	}
}

enum class DefineAbbreviationEncoding
{
	Fixed = 1,
	VBR = 2,
	Array = 3,
	Char6 = 4,
	Blob = 5,
};

void LLVMBitCodeParser::ParseDefineAbbreviation(BitReader& reader)
{
	auto numberOperands = reader.ReadVBR(5);

	std::cout << "DefineAbbreviation: " << numberOperands << std::endl;
	
	uint32_t previousValue = 0;
	bool isArray = false;
	for (auto i = 0; i < numberOperands; i++)
	{
		auto isLiteralOperand = static_cast<bool>(reader.Read(1));
		if (isLiteralOperand)
		{
			auto literalValue = reader.ReadVBR(8);
			std::cout << "L" << literalValue << " ";
		}
		else
		{
			auto encoding = static_cast<DefineAbbreviationEncoding>(reader.Read(3));
			switch (encoding)
			{
				case DefineAbbreviationEncoding::Fixed:
				{
					auto value = reader.ReadVBR(5);
					std::cout << "F" << value << " ";
					previousValue = value;
					isArray = false;
					break;
				}
				case DefineAbbreviationEncoding::VBR:
				{
					auto value = reader.ReadVBR(5);
					std::cout << "V" << value << " ";
					previousValue = value;
					isArray = false;
					break;
				}
				case DefineAbbreviationEncoding::Array:
				{
					std::cout << "A" << " ";
					isArray = true;
					break;
				}
				case DefineAbbreviationEncoding::Char6:
				{
					if (!isArray)
						throw std::runtime_error("A type must be in an array.");
					std::cout << "C[";
					for (auto i = 0; i < previousValue; i++)
					{
						auto character = static_cast<char>(reader.Read(6));
						std::cout << character;
					}

					std::cout << "] ";
					break;
				}
				case DefineAbbreviationEncoding::Blob:
				{
					std::cout << "B ";
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown DefineAbbreviationEncoding");
				}
			}
		}
	}

	std::cout << std::endl;
}

UnabbreviatedRecord LLVMBitCodeParser::ParseUnabbreviatedRecord(BitReader& reader)
{
	UnabbreviatedRecord result = {};
	result.Code = reader.ReadVBR(6);
	auto numberOperands = reader.ReadVBR(6);
	result.Operands.reserve(numberOperands);

	for (auto i = 0; i < numberOperands; i++)
	{
		auto operand = reader.ReadVBR(6);
		result.Operands.push_back(operand);
	}

	return result;
}