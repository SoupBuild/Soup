#pragma once
#include "BitReader.h"

namespace Soup
{
	constexpr uint32_t FirstApplicationAbbreviationId = 4;
	enum class StandardAbbreviationId : char
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

	constexpr uint32_t FirstApplicationBlockId = 8;
	enum class StandardBlockId
	{
		BlockInfo = 0,
	};

	enum class BlockRecordId
	{
		SetBlockId = 1,
		BlockName = 2,
		SetRecordName = 3,
	};

	enum class ClangASTBlockID
	{
		AST = FirstApplicationBlockId,
		SourceManager,
		Preprocessor,
		DeclTypes,
		PreprocessorDetail,
		SubModule,
		Comments,
		Control,
		InputFiles,
		Options,
		Extension,
		UnhashedControl,
	};

	enum class AbbreviationEncoding
	{
		Fixed = 1,
		VBR = 2,
		Array = 3,
		Char6 = 4,
		Blob = 5,
	};

	struct UnabbreviatedRecord
	{
		uint32_t Code;
		std::vector<uint64_t> Operands;
	};

	struct RecordInfo
	{
		uint32_t Id;
		std::string Name;
	};

	struct BlockInfo
	{
		uint32_t Id;
		std::string Name;
		std::vector<RecordInfo> Records;
	};

	struct BitCodeAbbreviationOperand
	{
		bool IsLiteral;
		AbbreviationEncoding Encoding;
		uint32_t Value;
	};

	struct BitCodeAbbreviation
	{
		std::vector<BitCodeAbbreviationOperand> Operands;
	};

	class LLVMBitCodeParser
	{
	public:
		void Parse(std::istream& stream);

	private:
		std::string_view GetBlockName(uint32_t blockId);
		std::string_view GetRecordName(uint32_t blockId, uint32_t code);
		void ParseRootBlock(BitReader& reader, size_t abbreviationLength);
		void ParseBlockInfoBlock(BitReader& reader, size_t abbreviationLength);
		BitCodeAbbreviation ParseDefineAbbreviation(BitReader& reader);
		UnabbreviatedRecord ParseUnabbreviatedRecord(BitReader& reader);
		void ParseRecord(uint32_t blockId, uint32_t abbreviationId, const std::vector<BitCodeAbbreviation>& abbreviations, BitReader& reader);

		// Clang AST Parsing
		void ParseControlBlock(BitReader& reader, size_t abbreviationLength);
		void ParseASTBlock(BitReader& reader, size_t abbreviationLength);

	private:
		std::vector<BlockInfo> m_blocks;
	};
}
