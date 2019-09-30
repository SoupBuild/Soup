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
	m_reader = std::make_unique<BitReader>(stream);
	size_t initialAbbreviationLength = 2;

	// Read until end of stream
	while (!m_reader->AtEndOfStream())
	{
		auto code = static_cast<StandardAbbreviationId>(m_reader->Read(initialAbbreviationLength));
		switch (code)
		{
		case StandardAbbreviationId::EnterSubBlock:
			ParseRootBlock();
			break;
		default:
			throw std::runtime_error(
					std::string("Unexpected StandardAbbreviationId In Root: ") + std::to_string(static_cast<uint32_t>(code)));
		}
	}

	m_reader = nullptr;
	std::cout << "Done." << std::endl;
}

BlockContext& LLVMBitCodeParser::GetActiveContext()
{
	return m_context.top();
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

void LLVMBitCodeParser::ParseRootBlock()
{
	// Read the new block and add a new context to the stack
	m_context.emplace();
	GetActiveContext().BlockId = m_reader->ReadVBR32(8);
	GetActiveContext().AbbreviationLength = m_reader->ReadVBR32(4);
	m_reader->Align32Bit();

	auto blockLength = m_reader->Read(32);
	std::cout << "EnterSubBlock: " << GetBlockName(GetActiveContext().BlockId) << std::endl;

	if (GetActiveContext().BlockId < FirstApplicationBlockId)
	{
		switch (static_cast<StandardBlockId>(GetActiveContext().BlockId))
		{
		case StandardBlockId::BlockInfo:
			ParseBlockInfoBlock();
			break;
		default:
			throw std::runtime_error("Unknown stadard block id.");
		}
	}
	else
	{
		switch (static_cast<ClangAST::BlockId>(GetActiveContext().BlockId))
		{
		case ClangAST::BlockId::AST:
			ParseASTBlock();
			break;
		case ClangAST::BlockId::Control:
			ParseControlBlock();
			break;
		default:
			// Skip unknown block
			std::cout << "Skip Block" << std::endl;
			m_reader->SeekByteOffset(blockLength * 4);
		}
	}

	// Pop this context
	m_context.pop();
}

void LLVMBitCodeParser::ParseBlockInfoBlock()
{
	// Read until we hit an end block
	BlockInfo* activeBlock = nullptr;
	while (true)
	{
		auto code = m_reader->Read(GetActiveContext().AbbreviationLength);
		switch (static_cast<StandardAbbreviationId>(code))
		{
		case StandardAbbreviationId::DefineAbbreviation:
		{
			GetActiveContext().Abbreviations.push_back(ParseDefineAbbreviation());
			break;
		}
		case StandardAbbreviationId::UnabbreviatedRecord:
		{
			auto record = ParseUnabbreviatedRecord();
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
			m_reader->Align32Bit();
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

BitCodeAbbreviation LLVMBitCodeParser::ParseDefineAbbreviation()
{
	auto result = BitCodeAbbreviation();
	auto numberOperands = m_reader->ReadVBR32(5);
	result.Operands.reserve(numberOperands);

	for (auto i = 0; i < numberOperands; i++)
	{
		auto operand = BitCodeAbbreviationOperand();
		operand.IsLiteral = static_cast<bool>(m_reader->Read(1));
		if (operand.IsLiteral)
		{
			operand.Value = m_reader->ReadVBR32(8);
		}
		else
		{
			operand.Encoding = static_cast<AbbreviationEncoding>(m_reader->Read(3));
			switch (operand.Encoding)
			{
			case AbbreviationEncoding::Fixed:
			case AbbreviationEncoding::VBR:
				operand.Value = m_reader->ReadVBR32(5);
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

UnabbreviatedRecord LLVMBitCodeParser::ParseUnabbreviatedRecord()
{
	UnabbreviatedRecord result = {};
	result.Code = m_reader->ReadVBR32(6);
	auto numberOperands = m_reader->ReadVBR32(6);
	result.Operands.reserve(numberOperands);

	for (auto i = 0; i < numberOperands; i++)
	{
		auto operand = m_reader->ReadVBR64(6);
		result.Operands.push_back(operand);
	}

	return result;
}

uint64_t LLVMBitCodeParser::ReadSingleOperand(const BitCodeAbbreviationOperand& operand)
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
				return m_reader->Read(operand.Value);
			case AbbreviationEncoding::VBR:
				return m_reader->ReadVBR64(operand.Value);
			case AbbreviationEncoding::Char6:
				return m_reader->Read(6);
			case AbbreviationEncoding::Array:
			case AbbreviationEncoding::Blob:
				throw std::runtime_error("Expected a single value encoding.");
			default:
				throw std::runtime_error("Unknown AbbreviationEncoding");
		}
	}
}

BitCodeRecord LLVMBitCodeParser::ParseRecord(uint32_t abbreviationId)
{
	auto result = BitCodeRecord();

	auto abbreviationIndex = abbreviationId - FirstApplicationAbbreviationId;
	if (abbreviationIndex >= GetActiveContext().Abbreviations.size())
		throw std::runtime_error("Unknown application defined abbreviation id: " + std::to_string(abbreviationId));
	auto& abbreviation = GetActiveContext().Abbreviations[abbreviationIndex];

	// read the record code
	if (abbreviation.Operands.empty())
		throw std::runtime_error("Abbreviation must have at least one operand.");
	result.Code = ReadSingleOperand(abbreviation.Operands[0]);
	// std::cout << "Parse Record: " << GetRecordName(blockId, result.Code) << std::endl;

	// Read the remaining operands
	for (auto i = 1; i < abbreviation.Operands.size(); i++)
	{
		auto& operand = abbreviation.Operands[i];
		if (operand.IsLiteral || (operand.Encoding != AbbreviationEncoding::Array && operand.Encoding != AbbreviationEncoding::Blob))
		{
			auto value = ReadSingleOperand(operand);
			result.Values.push_back(value);
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

					auto arraySize = m_reader->ReadVBR32(6);
					auto& typeOperand = abbreviation.Operands[++i];

					for (auto arrayIndex = 0; arrayIndex < arraySize; arrayIndex++)
					{
						auto value = ReadSingleOperand(typeOperand);
						result.Values.push_back(value);
					}

					break;
				}
				case AbbreviationEncoding::Blob:
				{
					if (i != abbreviation.Operands.size() - 1)
						throw std::runtime_error("A blob can only occur as the last operand.");

					auto blobSize = m_reader->ReadVBR32(6);
					m_reader->Align32Bit();

					for (auto blobIndex = 0; blobIndex < blobSize; blobIndex++)
					{
						auto value = m_reader->Read(8);
						result.Values.push_back(value);
					}

					m_reader->Align32Bit();
					break;
				}
				default:
				{
					throw std::runtime_error("Unexpected AbbreviationEncoding");
				}
			}
		}
	}

	return result;
}

void LLVMBitCodeParser::ParseControlBlock()
{
	// Read until we hit an end block
	while (true)
	{
		auto abbreviationId = m_reader->Read(GetActiveContext().AbbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					// Read the new block and add a new context to the stack
					m_context.emplace();
					GetActiveContext().BlockId = m_reader->ReadVBR32(8);
					GetActiveContext().AbbreviationLength = m_reader->ReadVBR32(4);
					m_reader->Align32Bit();
					auto blockLength = m_reader->Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(GetActiveContext().BlockId) << std::endl;
					std::cout << "Skip Block" << std::endl;
					m_reader->SeekByteOffset(blockLength * 4);

					// Pop this context
					m_context.pop();
					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					GetActiveContext().Abbreviations.push_back(ParseDefineAbbreviation());
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					auto record = ParseUnabbreviatedRecord();
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					m_reader->Align32Bit();
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
			auto record = ParseRecord(abbreviationId);
			// TODO
		}
	}
}


void LLVMBitCodeParser::ParseASTBlock()
{
	// Read until we hit an end block
	while (true)
	{
		auto abbreviationId = m_reader->Read(GetActiveContext().AbbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					// Read the new block and add a new context to the stack
					m_context.emplace();
					GetActiveContext().BlockId = m_reader->ReadVBR32(8);
					GetActiveContext().AbbreviationLength = m_reader->ReadVBR32(4);
					m_reader->Align32Bit();
					auto blockLength = m_reader->Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(GetActiveContext().BlockId) << std::endl;
					switch (static_cast<ClangAST::BlockId>(GetActiveContext().BlockId))
					{
						case ClangAST::BlockId::DeclTypes:
							ParseDeclTypesBlock();
							break;
						default:
							// Skip unknown block
							std::cout << "Skip Block" << std::endl;
							m_reader->SeekByteOffset(blockLength * 4);
					}

					// Pop this context
					m_context.pop();

					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					GetActiveContext().Abbreviations.push_back(ParseDefineAbbreviation());
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					auto record = ParseUnabbreviatedRecord();
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					m_reader->Align32Bit();
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
			auto record = ParseRecord(abbreviationId);
			// TODO
		}
	}
}

void LLVMBitCodeParser::ParseDeclTypesBlock()
{
	// Read until we hit an end block
	while (true)
	{
		auto abbreviationId = m_reader->Read(GetActiveContext().AbbreviationLength);
		if (abbreviationId < FirstApplicationAbbreviationId)
		{
			switch (static_cast<StandardAbbreviationId>(abbreviationId))
			{
				case StandardAbbreviationId::EnterSubBlock:
				{
					// Read the new block and add a new context to the stack
					m_context.emplace();
					GetActiveContext().BlockId = m_reader->ReadVBR32(8);
					GetActiveContext().AbbreviationLength = m_reader->ReadVBR32(4);
					m_reader->Align32Bit();
					auto blockLength = m_reader->Read(32);

					std::cout << "EnterSubBlock: " << GetBlockName(GetActiveContext().BlockId) << std::endl;
					switch (static_cast<ClangAST::BlockId>(GetActiveContext().BlockId))
					{
						default:
							// Skip unknown block
							std::cout << "Skip Block" << std::endl;
							m_reader->SeekByteOffset(blockLength * 4);
					}

					// Pop this context
					m_context.pop();

					break;
				}
				case StandardAbbreviationId::DefineAbbreviation:
				{
					GetActiveContext().Abbreviations.push_back(ParseDefineAbbreviation());
					break;
				}
				case StandardAbbreviationId::UnabbreviatedRecord:
				{
					std::cout << "UnabbreviatedRecord: " << std::endl;
					ParseExternalCPPBaseSpecifiers();
					break;
				}
				case StandardAbbreviationId::EndBlock:
				{
					std::cout << "EndBlock" << std::endl;
					m_reader->Align32Bit();
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
			// The first record should be a declaration context
			auto record = ParseRecord(abbreviationId);
			std::cout << "RECORD: " << record.Code << std::endl;
		}
	}
}

void ParseTypeSourceInfo(const std::vector<uint64_t>& values, uint32_t& index)
{
		auto typeId = values[index++];
		std::cout << "ParseTypeSourceInfo: " << typeId << std::endl;
		// TODO: 
}

void LLVMBitCodeParser::ParseExternalCPPBaseSpecifiers()
{
	auto record = ParseUnabbreviatedRecord();
  if (static_cast<ClangAST::DeclarationCode>(record.Code) != ClangAST::DeclarationCode::CPP_BaseSpecifiers)
		throw std::runtime_error("Record must be CPP_BaseSpecifiers");

  uint32_t index = 0;
  auto basesCount = record.Operands[index++];
  for (auto i = 0; i < basesCount; i++)
	{
		auto isVirtual = static_cast<bool>(record.Operands[index++]);
		auto isBaseOfClass = static_cast<bool>(record.Operands[index++]);
		auto accessSpecifier = static_cast<ClangAST::AccessSpecifier>(record.Operands[index++]);
		auto inheritConstructors = static_cast<bool>(record.Operands[index++]);

		// Parse the type info source location
		ParseTypeSourceInfo(record.Operands, index);

		auto rawSourceBegin = record.Operands[index++];
		auto rawSourceEnd = record.Operands[index++];

		auto rawEllipsisLocation = record.Operands[index++];

		std::cout << "CPPBaseSpecifier: " << isVirtual << " " << isBaseOfClass << " " << static_cast<uint32_t>(accessSpecifier) << std::endl;
	}

	// TODO
	// if (index != record.Operands.size())
	// 	throw std::runtime_error("Did not consume entire record: CPP_BaseSpecifiers");
}

void LLVMBitCodeParser::ParseDeclarationContext(const BitCodeRecord& record)
{
	if (static_cast<ClangAST::DeclarationCode>(record.Code) != ClangAST::DeclarationCode::ContextLexical)
		throw std::runtime_error("Record must be ContextLexical");

	auto blockId = static_cast<uint32_t>(ClangAST::BlockId::DeclTypes);
	std::cout << record.Values.size() << std::endl;
	for (auto value : record.Values)
	{
		auto abbreviationId = m_reader->Read(GetActiveContext().AbbreviationLength);
					auto otherRecord = ParseUnabbreviatedRecord();
		//auto declarationRecord = ParseRecord(abbreviationId);
		std::cout << value << " : " << otherRecord.Code << std::endl;
	}
}

void LLVMBitCodeParser::ParseDeclaration(const BitCodeRecord& record)
{
	switch (static_cast<ClangAST::DeclarationCode>(record.Code))
	{
		case ClangAST::DeclarationCode::TypeDef:
			throw std::runtime_error("NOT_IMPLEMENTED: TypeDef");
		case ClangAST::DeclarationCode::TypeAlias:
			throw std::runtime_error("NOT_IMPLEMENTED: TypeAlias");
		case ClangAST::DeclarationCode::Enum:
			throw std::runtime_error("NOT_IMPLEMENTED: Enum");
		case ClangAST::DeclarationCode::Record:
			throw std::runtime_error("NOT_IMPLEMENTED: Record");
		case ClangAST::DeclarationCode::EnumConstant:
			throw std::runtime_error("NOT_IMPLEMENTED: EnumConstant");
		case ClangAST::DeclarationCode::Function:
			throw std::runtime_error("NOT_IMPLEMENTED: Function");
		case ClangAST::DeclarationCode::Method:
			throw std::runtime_error("NOT_IMPLEMENTED: Method");
		case ClangAST::DeclarationCode::ObjC_Interface:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_Interface");
		case ClangAST::DeclarationCode::ObjC_Protocol:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_Protocol");
		case ClangAST::DeclarationCode::ObjC_IVar:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_IVar");
		case ClangAST::DeclarationCode::ObjC_AtDefsField:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_AtDefsField");
		case ClangAST::DeclarationCode::ObjC_Category:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_Category");
		case ClangAST::DeclarationCode::OBJC_CategoryImpl:
			throw std::runtime_error("NOT_IMPLEMENTED: OBJC_CategoryImpl");
		case ClangAST::DeclarationCode::OBJC_Implementation:
			throw std::runtime_error("NOT_IMPLEMENTED: OBJC_Implementation");
		case ClangAST::DeclarationCode::OBJC_CompatibleAlias:
			throw std::runtime_error("NOT_IMPLEMENTED: OBJC_CompatibleAlias");
		case ClangAST::DeclarationCode::OBJC_Property:
			throw std::runtime_error("NOT_IMPLEMENTED: OBJC_Property");
		case ClangAST::DeclarationCode::OBJC_PropertyImpl:
			throw std::runtime_error("NOT_IMPLEMENTED: OBJC_PropertyImpl");
		case ClangAST::DeclarationCode::Field:
			throw std::runtime_error("NOT_IMPLEMENTED: Field");
		case ClangAST::DeclarationCode::MS_Property:
			throw std::runtime_error("NOT_IMPLEMENTED: MS_Property");
		case ClangAST::DeclarationCode::Var:
			throw std::runtime_error("NOT_IMPLEMENTED: Var");
		case ClangAST::DeclarationCode::ImplicitParam:
			throw std::runtime_error("NOT_IMPLEMENTED: ImplicitParam");
		case ClangAST::DeclarationCode::ParmVar:
			throw std::runtime_error("NOT_IMPLEMENTED: ParmVar");
		case ClangAST::DeclarationCode::Decomposition:
			throw std::runtime_error("NOT_IMPLEMENTED: Decomposition");
		case ClangAST::DeclarationCode::Binding:
			throw std::runtime_error("NOT_IMPLEMENTED: Binding");
		case ClangAST::DeclarationCode::FileScopeASM:
			throw std::runtime_error("NOT_IMPLEMENTED: FileScopeASM");
		case ClangAST::DeclarationCode::Block:
			throw std::runtime_error("NOT_IMPLEMENTED: Block");
		case ClangAST::DeclarationCode::Captured:
			throw std::runtime_error("NOT_IMPLEMENTED: Captured");
		case ClangAST::DeclarationCode::ContextLexical:
			throw std::runtime_error("NOT_IMPLEMENTED: ContextLexical");
		case ClangAST::DeclarationCode::ContextVisible:
			throw std::runtime_error("NOT_IMPLEMENTED: ContextVisible");
		case ClangAST::DeclarationCode::Label:
			throw std::runtime_error("NOT_IMPLEMENTED: Label");
		case ClangAST::DeclarationCode::Namespace:
			throw std::runtime_error("NOT_IMPLEMENTED: Namespace");
		case ClangAST::DeclarationCode::NamespaceAlias:
			throw std::runtime_error("NOT_IMPLEMENTED: NamespaceAlias");
		case ClangAST::DeclarationCode::Using:
			throw std::runtime_error("NOT_IMPLEMENTED: Using");
		case ClangAST::DeclarationCode::UsingPack:
			throw std::runtime_error("NOT_IMPLEMENTED: UsingPack");
		case ClangAST::DeclarationCode::UsingShadow:
			throw std::runtime_error("NOT_IMPLEMENTED: UsingShadow");
		case ClangAST::DeclarationCode::ConstrutorUsingShadow:
			throw std::runtime_error("NOT_IMPLEMENTED: ConstrutorUsingShadow");
		case ClangAST::DeclarationCode::UsingDirective:
			throw std::runtime_error("NOT_IMPLEMENTED: UsingDirective");
		case ClangAST::DeclarationCode::UnresolvedUsingValue:
			throw std::runtime_error("NOT_IMPLEMENTED: UnresolvedUsingValue");
		case ClangAST::DeclarationCode::UnresolvedUsingTypename:
			throw std::runtime_error("NOT_IMPLEMENTED: UnresolvedUsingTypename");
		case ClangAST::DeclarationCode::LinkageSpec:
			throw std::runtime_error("NOT_IMPLEMENTED: LinkageSpec");
		case ClangAST::DeclarationCode::Export:
			throw std::runtime_error("NOT_IMPLEMENTED: Export");
		case ClangAST::DeclarationCode::CPP_Record:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_Record");
		case ClangAST::DeclarationCode::CPP_DeductionGuide:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_DeductionGuide");
		case ClangAST::DeclarationCode::CPP_Method:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_Method");
		case ClangAST::DeclarationCode::CPP_Constructor:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_Constructor");
		case ClangAST::DeclarationCode::CPP_Destructor:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_Destructor");
		case ClangAST::DeclarationCode::CPP_Conversion:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_Conversion");
		case ClangAST::DeclarationCode::AccessSpec:
			throw std::runtime_error("NOT_IMPLEMENTED: AccessSpec");
		case ClangAST::DeclarationCode::Friend:
			throw std::runtime_error("NOT_IMPLEMENTED: Friend");
		case ClangAST::DeclarationCode::FriendTemplate:
			throw std::runtime_error("NOT_IMPLEMENTED: FriendTemplate");
		case ClangAST::DeclarationCode::ClassTemplate:
			throw std::runtime_error("NOT_IMPLEMENTED: ClassTemplate");
		case ClangAST::DeclarationCode::ClassTemplateSpecialization:
			throw std::runtime_error("NOT_IMPLEMENTED: ClassTemplateSpecialization");
		case ClangAST::DeclarationCode::ClassTemplatePartialSpecialization:
			throw std::runtime_error("NOT_IMPLEMENTED: ClassTemplatePartialSpecialization");
		case ClangAST::DeclarationCode::VarTemplate:
			throw std::runtime_error("NOT_IMPLEMENTED: VarTemplate");
		case ClangAST::DeclarationCode::VarTemplateSpecialization:
			throw std::runtime_error("NOT_IMPLEMENTED: VarTemplateSpecialization");
		case ClangAST::DeclarationCode::VarTemplatePartialSpecialization:
			throw std::runtime_error("NOT_IMPLEMENTED: VarTemplatePartialSpecialization");
		case ClangAST::DeclarationCode::FunctionTemplate:
			throw std::runtime_error("NOT_IMPLEMENTED: FunctionTemplate");
		case ClangAST::DeclarationCode::TemplateTypeParm:
			throw std::runtime_error("NOT_IMPLEMENTED: TemplateTypeParm");
		case ClangAST::DeclarationCode::NonTypeTemplateParm:
			throw std::runtime_error("NOT_IMPLEMENTED: NonTypeTemplateParm");
		case ClangAST::DeclarationCode::TemplateTemplateParm:
			throw std::runtime_error("NOT_IMPLEMENTED: TemplateTemplateParm");
		case ClangAST::DeclarationCode::TypeAliasTemplate:
			throw std::runtime_error("NOT_IMPLEMENTED: TypeAliasTemplate");
		case ClangAST::DeclarationCode::Concept:
			throw std::runtime_error("NOT_IMPLEMENTED: Concept");
		case ClangAST::DeclarationCode::StaticAssert:
			throw std::runtime_error("NOT_IMPLEMENTED: StaticAssert");
		case ClangAST::DeclarationCode::CPP_BaseSpecifiers:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_BaseSpecifiers");
		case ClangAST::DeclarationCode::CPP_ConstructorInitialzers:
			throw std::runtime_error("NOT_IMPLEMENTED: CPP_ConstructorInitialzers");
		case ClangAST::DeclarationCode::IndirectField:
			throw std::runtime_error("NOT_IMPLEMENTED: IndirectField");
		case ClangAST::DeclarationCode::ExpandedNonTypeTemplateParmPack:
			throw std::runtime_error("NOT_IMPLEMENTED: ExpandedNonTypeTemplateParmPack");
		case ClangAST::DeclarationCode::ExpandedTemplateTemplateParmPack:
			throw std::runtime_error("NOT_IMPLEMENTED: ExpandedTemplateTemplateParmPack");
		case ClangAST::DeclarationCode::ClassScopeFunctionSpecification:
			throw std::runtime_error("NOT_IMPLEMENTED: ClassScopeFunctionSpecification");
		case ClangAST::DeclarationCode::Import:
			throw std::runtime_error("NOT_IMPLEMENTED: Import");
		case ClangAST::DeclarationCode::OMP_ThreadPrivate:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_ThreadPrivate");
		case ClangAST::DeclarationCode::OMP_Requires:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_Requires");
		case ClangAST::DeclarationCode::OMP_Allocate:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_Allocate");
		case ClangAST::DeclarationCode::Empty:
			throw std::runtime_error("NOT_IMPLEMENTED: Empty");
		case ClangAST::DeclarationCode::ObjC_TypeParam:
			throw std::runtime_error("NOT_IMPLEMENTED: ObjC_TypeParam");
		case ClangAST::DeclarationCode::OMP_CaptureDExpr:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_CaptureDExpr");
		case ClangAST::DeclarationCode::PragmaComment:
			throw std::runtime_error("NOT_IMPLEMENTED: PragmaComment");
		case ClangAST::DeclarationCode::PagmaDetectMismatch:
			throw std::runtime_error("NOT_IMPLEMENTED: PagmaDetectMismatch");
		case ClangAST::DeclarationCode::OMP_DeclareMapper:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_DeclareMapper");
		case ClangAST::DeclarationCode::OMP_DeclareReduction:
			throw std::runtime_error("NOT_IMPLEMENTED: OMP_DeclareReduction");
		default:
			throw std::runtime_error("Unknown ClangAST::DeclarationCode record: " + std::to_string(record.Code));
	}
}