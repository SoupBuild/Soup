import std.core;

#include "LLVMBitCodeParser.h"

using namespace Soup;

template <std::size_t N>
void ReadBytes(std::istream &stream, std::array<char, N> &result)
{
	if (!stream.read(result.data(), result.size()))
		throw std::runtime_error("Failed read.");
}

char ReadByte(std::istream &stream)
{
	char result;
	if (!stream.read(&result, 1))
		throw std::runtime_error("Failed read.");
	return result;
}

void LLVMBitCodeParser::Parse(std::istream &stream)
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
		auto code = static_cast<StandardAbbreviationId>(reader.Read(initialAbbreviationLength));
		switch (code)
		{
		case StandardAbbreviationId::EnterSubBlock:
			ParseRootBlock(reader, initialAbbreviationLength);
			break;
		default:
			throw std::runtime_error(
					std::string("Unexpected StandardAbbreviationId In Root: ") + std::to_string(static_cast<uint32_t>(code)));
		}
	}

	std::cout << "Done." << std::endl;
}

std::string_view LLVMBitCodeParser::GetBlockName(uint32_t blockId)
{
	if (blockId < FirstApplicationBlockId)
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
				[blockId](const BlockInfo &value) { return value.Id == blockId; });
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

std::string_view LLVMBitCodeParser::GetRecordName(uint32_t blockId, uint32_t code)
{
	if (blockId < FirstApplicationBlockId)
	{
		switch (static_cast<StandardBlockId>(blockId))
		{
		case StandardBlockId::BlockInfo:
			return "UNKNOWN (BLOCK_INFO)";
		default:
			return "UNKNOWN (STANDARD)";
		}
	}
	else
	{
		auto blockInfoResult = std::find_if(
			m_blocks.begin(),
			m_blocks.end(),
			[blockId](const BlockInfo &value) { return value.Id == blockId; });
		if (blockInfoResult != m_blocks.end())
		{
			auto recordInfoResult = std::find_if(
				blockInfoResult->Records.begin(),
				blockInfoResult->Records.end(),
				[code](const RecordInfo &value) { return value.Id == code; });
			if (recordInfoResult != blockInfoResult->Records.end())
			{
				return recordInfoResult->Name;
			}
			else
			{
				return "UNKNOWN";
			}
		}
		else
		{
			return "UNKNOWN (MISSING BLOCK)";
		}
	}
}

void LLVMBitCodeParser::ParseRootBlock(BitReader &reader, size_t abbreviationLength)
{
	auto blockId = reader.ReadVBR32(8);
	auto newAbbreviationLength = reader.ReadVBR32(4);
	reader.Align32Bit();
	auto blockLength = reader.Read(32);

	std::cout << "EnterSubBlock: " << GetBlockName(blockId) << std::endl;
	// std::cout << "BlockId:" << blockId << std::endl;
	// std::cout << "NewAbbreviationLength:" << newAbbreviationLength << std::endl;
	// std::cout << "BlockLength:" << blockLength << std::endl;

	// Update the current context
	abbreviationLength = newAbbreviationLength;

	if (blockId < FirstApplicationBlockId)
	{
		switch (static_cast<StandardBlockId>(blockId))
		{
		case StandardBlockId::BlockInfo:
			ParseBlockInfoBlock(reader, abbreviationLength);
			break;
		default:
			throw std::runtime_error("Unknown stadard block id.");
		}
	}
	else
	{
		switch (static_cast<ClangASTBlockID>(blockId))
		{
		case ClangASTBlockID::AST:
			ParseASTBlock(reader, abbreviationLength);
			break;
		case ClangASTBlockID::Control:
			ParseControlBlock(reader, abbreviationLength);
			break;
		default:
			// Skip unknown block
			std::cout << "Skip Block" << std::endl;
			reader.SeekByteOffset(blockLength * 4);
		}
	}
}

void LLVMBitCodeParser::ParseBlockInfoBlock(BitReader &reader, size_t abbreviationLength)
{
	// Read until we hit an end block
	BlockInfo *activeBlock = nullptr;
	auto abbreviations = std::vector<BitCodeAbbreviation>();
	while (true)
	{
		auto code = static_cast<StandardAbbreviationId>(reader.Read(abbreviationLength));
		switch (code)
		{
		case StandardAbbreviationId::DefineAbbreviation:
		{
			abbreviations.push_back(ParseDefineAbbreviation(reader));
			break;
		}
		case StandardAbbreviationId::UnabbreviatedRecord:
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
					// Downcast the 64 bit value to the 32 bit id
					activeBlock->Id = static_cast<uint32_t>(record.Operands[0]);
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
					auto recordInfo = RecordInfo();
					recordInfo.Id = static_cast<uint32_t>(record.Operands[0]);
					recordInfo.Name = std::string(record.Operands.begin() + 1, record.Operands.end());
					activeBlock->Records.push_back(std::move(recordInfo));
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown BlockInfoBlock Record.");
				}
			}
			break;
		}
		case StandardAbbreviationId::EndBlock:
		{
			std::cout << "EndBlock" << std::endl;
			reader.Align32Bit();
			return;
		}
		default:
		{
			throw std::runtime_error(
					std::string("Unexpected StandardAbbreviationId In BlockInfoBlock: ") + std::to_string(static_cast<uint32_t>(code)));
		}
		}
	}
}

BitCodeAbbreviation LLVMBitCodeParser::ParseDefineAbbreviation(BitReader &reader)
{
	auto result = BitCodeAbbreviation();
	auto numberOperands = reader.ReadVBR32(5);
	result.Operands.reserve(numberOperands);

	for (auto i = 0; i < numberOperands; i++)
	{
		auto operand = BitCodeAbbreviationOperand();
		operand.IsLiteral = static_cast<bool>(reader.Read(1));
		if (operand.IsLiteral)
		{
			operand.Value = reader.ReadVBR32(8);
		}
		else
		{
			operand.Encoding = static_cast<AbbreviationEncoding>(reader.Read(3));
			switch (operand.Encoding)
			{
			case AbbreviationEncoding::Fixed:
			case AbbreviationEncoding::VBR:
				operand.Value = reader.ReadVBR32(5);
				break;
			case AbbreviationEncoding::Array:
			case AbbreviationEncoding::Char6:
			case AbbreviationEncoding::Blob:
				break;
			default:
				throw std::runtime_error("Unknown AbbreviationEncoding");
			}
		}

		result.Operands.push_back(std::move(operand));
	}

	return result;
}

UnabbreviatedRecord LLVMBitCodeParser::ParseUnabbreviatedRecord(BitReader &reader)
{
	UnabbreviatedRecord result = {};
	result.Code = reader.ReadVBR32(6);
	auto numberOperands = reader.ReadVBR32(6);
	result.Operands.reserve(numberOperands);

	for (auto i = 0; i < numberOperands; i++)
	{
		auto operand = reader.ReadVBR64(6);
		result.Operands.push_back(operand);
	}

	return result;
}

uint64_t ReadSingleOperand(
	const BitCodeAbbreviationOperand& operand,
	BitReader &reader)
{
	if (operand.IsLiteral)
	{
		return operand.Value;
	}
	else
	{
		switch (operand.Encoding)
		{
			case AbbreviationEncoding::Fixed:
				return reader.Read(operand.Value);
			case AbbreviationEncoding::VBR:
				return reader.ReadVBR64(operand.Value);
			case AbbreviationEncoding::Char6:
				return reader.Read(6);
			case AbbreviationEncoding::Array:
			case AbbreviationEncoding::Blob:
				throw std::runtime_error("Expected a single value encoding.");
			default:
				throw std::runtime_error("Unknown AbbreviationEncoding");
		}
	}
}

void LLVMBitCodeParser::ParseRecord(uint32_t blockId, uint32_t abbreviationId, const std::vector<BitCodeAbbreviation>& abbreviations, BitReader &reader)
{
	auto abbreviationIndex = abbreviationId - FirstApplicationAbbreviationId;
	if (abbreviationIndex >= abbreviations.size())
		throw std::runtime_error("Unknown application defined abbreviation id: " + std::to_string(abbreviationId));
	auto& abbreviation = abbreviations[abbreviationIndex];

	// read the record code
	if (abbreviation.Operands.empty())
		throw std::runtime_error("Abbreviation must have at least one operand.");
	auto code = ReadSingleOperand(abbreviation.Operands[0], reader);
	// std::cout << "Parse Record: " << GetRecordName(blockId, code) << std::endl;

	// Read the remaining operands
	for (auto i = 1; i < abbreviation.Operands.size(); i++)
	{
		auto& operand = abbreviation.Operands[i];
		if (operand.IsLiteral || (operand.Encoding != AbbreviationEncoding::Array && operand.Encoding != AbbreviationEncoding::Blob))
		{
			auto value = ReadSingleOperand(operand, reader);
			// TODO:
		}
		else
		{
			// Parse the complex cases
			switch (operand.Encoding)
			{
				case AbbreviationEncoding::Array:
				{
					if (i != abbreviation.Operands.size() - 2)
						throw std::runtime_error("A array can only occur as the second to last operand.");

					auto arraySize = reader.ReadVBR32(6);
					auto& typeOperand = abbreviation.Operands[++i];

					for (auto arrayIndex = 0; arrayIndex < arraySize; arrayIndex++)
					{
						auto value = ReadSingleOperand(typeOperand, reader);
						// TODO:
					}

					break;
				}
				case AbbreviationEncoding::Blob:
				{
					if (i != abbreviation.Operands.size() - 1)
						throw std::runtime_error("A blob can only occur as the last operand.");

					auto blobSize = reader.ReadVBR32(6);
					reader.Align32Bit();

					for (auto blobIndex = 0; blobIndex < blobSize; blobIndex++)
					{
						auto value = reader.Read(8);
						// TODO:
					}

					reader.Align32Bit();
					break;
				}
				default:
				{
					throw std::runtime_error("Unexpected AbbreviationEncoding");
				}
			}
		}
	}
}

void LLVMBitCodeParser::ParseControlBlock(BitReader &reader, size_t abbreviationLength)
{
	// Read until we hit an end block
	auto blockId = static_cast<uint32_t>(ClangASTBlockID::Control);
	auto abbreviations = std::vector<BitCodeAbbreviation>();
	while (true)
	{
		auto abbreviationId = reader.Read(abbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					auto blockId = reader.ReadVBR32(8);
					auto newAbbreviationLength = reader.ReadVBR32(4);
					reader.Align32Bit();
					auto blockLength = reader.Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(blockId) << std::endl;
					std::cout << "Skip Block" << std::endl;
					reader.SeekByteOffset(blockLength * 4);
					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					abbreviations.push_back(ParseDefineAbbreviation(reader));
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					auto record = ParseUnabbreviatedRecord(reader);
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					reader.Align32Bit();
					return;
				}
				default:
				{
					throw std::runtime_error(
						std::string("Unexpected AbbreviationId In ControlBlock: ") +
						std::to_string(abbreviationId));
				}
			}
		}
		else
		{
			ParseRecord(blockId, abbreviationId, abbreviations, reader);
		}
	}
}


void LLVMBitCodeParser::ParseASTBlock(BitReader &reader, size_t abbreviationLength)
{
	// Read until we hit an end block
	auto blockId = static_cast<uint32_t>(ClangASTBlockID::AST);
	auto abbreviations = std::vector<BitCodeAbbreviation>();
	while (true)
	{
		auto abbreviationId = reader.Read(abbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					auto blockId = reader.ReadVBR32(8);
					auto newAbbreviationLength = reader.ReadVBR32(4);
					reader.Align32Bit();
					auto blockLength = reader.Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(blockId) << std::endl;
					switch (static_cast<ClangASTBlockID>(blockId))
					{
						case ClangASTBlockID::DeclTypes:
							ParseDeclTypesBlock(reader, newAbbreviationLength);
							break;
						default:
							// Skip unknown block
							std::cout << "Skip Block" << std::endl;
							reader.SeekByteOffset(blockLength * 4);
					}

					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					abbreviations.push_back(ParseDefineAbbreviation(reader));
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					auto record = ParseUnabbreviatedRecord(reader);
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					reader.Align32Bit();
					return;
				}
				default:
				{
					throw std::runtime_error(
						std::string("Unexpected AbbreviationId In ASTBlock: ") +
						std::to_string(abbreviationId));
				}
			}
		}
		else
		{
			ParseRecord(blockId, abbreviationId, abbreviations, reader);
		}
	}
}

void LLVMBitCodeParser::ParseDeclTypesBlock(BitReader &reader, size_t abbreviationLength)
{
	// Read until we hit an end block
	auto blockId = static_cast<uint32_t>(ClangASTBlockID::DeclTypes);
	auto abbreviations = std::vector<BitCodeAbbreviation>();
	while (true)
	{
		auto abbreviationId = reader.Read(abbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					auto blockId = reader.ReadVBR32(8);
					auto newAbbreviationLength = reader.ReadVBR32(4);
					reader.Align32Bit();
					auto blockLength = reader.Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(blockId) << std::endl;
					switch (static_cast<ClangASTBlockID>(blockId))
					{
						default:
							// Skip unknown block
							std::cout << "Skip Block" << std::endl;
							reader.SeekByteOffset(blockLength * 4);
					}

					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					abbreviations.push_back(ParseDefineAbbreviation(reader));
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					auto record = ParseUnabbreviatedRecord(reader);
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					reader.Align32Bit();
					return;
				}
				default:
				{
					throw std::runtime_error(
						std::string("Unexpected AbbreviationId In DeclTypesBlock: ") +
						std::to_string(abbreviationId));
				}
			}
		}
		else
		{
			ParseRecord(blockId, abbreviationId, abbreviations, reader);
		}
	}
}
