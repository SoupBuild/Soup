#pragma once

namespace Soup
{
	class BitReader
	{
	public:
		BitReader(std::istream& stream);
		uint32_t Read(size_t countBits);
		uint32_t ReadVBR(size_t countBits);
		void Align32Bit();
		uint32_t GetBitIndex();
		void SeekByteOffset(size_t offset);
		bool AtEndOfStream() const;

	private:
		std::istream& m_stream;
		uint32_t m_buffer;
		size_t m_bufferBitSize;
	};
}
