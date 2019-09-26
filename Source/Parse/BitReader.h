#pragma once

namespace Soup
{
	class BitReader
	{
	public:
		BitReader(std::istream& stream);
		uint32_t Read(size_t countBits);
		uint32_t ReadVBR32(size_t countBits);
		uint64_t ReadVBR64(size_t countBits);
		void Align32Bit();
		size_t GetBitIndex();
		void SeekByteOffset(size_t offset);
		bool AtEndOfStream() const;

	private:
		std::istream& m_stream;
		uint64_t m_buffer;
		size_t m_bufferBitSize;
	};
}
