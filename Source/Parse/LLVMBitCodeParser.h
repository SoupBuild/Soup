#pragma once
#include "BitReader.h"

namespace Soup
{
	struct UnabbreviatedRecord
	{
		uint32_t Code;
		std::vector<uint32_t> Operands;
	};

	class LLVMBitCodeParser
	{
	public:
		static void Parse(std::istream& stream);

	private:
		static void ParseSubBlock(BitReader& reader, size_t abbreviationLength);
		static void ParseBlockInfo(BitReader& reader, size_t abbreviationLength);
		static void ParseDefineAbbreviation(BitReader& reader);
		static UnabbreviatedRecord ParseUnabbreviatedRecord(BitReader& reader);
	};
}
