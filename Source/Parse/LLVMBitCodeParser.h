#pragma once
#include "BitReader.h"

namespace Soup
{
	struct UnabbreviatedRecord
	{
		uint32_t Code;
		std::vector<uint32_t> Operands;
	};

	struct BlockInfo
	{
		uint32_t Id;
		std::string Name;
	};

	class LLVMBitCodeParser
	{
	public:
		void Parse(std::istream& stream);

	private:
		std::string_view GetBlockName(uint32_t blockId);
		void ParseSubBlock(BitReader& reader, size_t abbreviationLength);
		void ParseBlockInfo(BitReader& reader, size_t abbreviationLength);
		void ParseDefineAbbreviation(BitReader& reader);
		UnabbreviatedRecord ParseUnabbreviatedRecord(BitReader& reader);

	private:
		std::vector<BlockInfo> m_blocks;
	};
}
