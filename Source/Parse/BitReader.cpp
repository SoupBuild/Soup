import std.core;

#include "BitReader.h"

using namespace Soup;

BitReader::BitReader(std::istream& stream) :
	m_stream(stream),
	m_buffer(0),
	m_bufferBitSize(0)
{
}

uint32_t BitReader::Read(size_t countBits)
{
	// std::cout << "Read: " << countBits << " " << GetBitIndex() << std::endl;
	constexpr size_t ResultBitSize = sizeof(uint32_t) * 8;
	constexpr size_t BufferBitSize = sizeof(uint64_t) * 8;
	if (countBits == 0 || countBits > ResultBitSize)
		throw std::runtime_error("The countBits is outside of the allowed range.");

	// Ensure we have enough data loaded into the buffer
	while (m_bufferBitSize < countBits)
	{
		// Read a byte and put it in the buffer
		char value;
		auto prevLoc = m_stream.tellg();
		if (!m_stream.read(&value, 1))
			throw std::runtime_error("Failed read.");

		uint64_t expandValue = 0xFF & (uint64_t)value;
		// std::cout << "ReadVal: 0x" << std::hex << expandValue << " 0x" << prevLoc << " 0x" << m_stream.tellg() << std::dec << std::endl;

		m_buffer |= expandValue << m_bufferBitSize;
		m_bufferBitSize += 8;
		// std::cout << "Buffer: 0x" << std::hex << m_buffer << std::dec << " " << m_bufferBitSize << std::endl;

		if (m_bufferBitSize > BufferBitSize)
		{
			std::cout << "Read: " << countBits << " " << GetBitIndex() << std::endl;
			throw std::runtime_error("Ran out of room in the buffer: " + std::to_string(m_bufferBitSize));
		}
	}

	// Pull out the requested number of bits
	// Note: Ensure the shifted value is 64 bits to have room to go beyond the 32 bit buffer
	uint64_t valueMask = (0x1LL << countBits) - 1;
	uint32_t result = m_buffer & valueMask;

	// Shift the remaining bits to the start of the buffer
	m_buffer = (m_buffer & ~valueMask) >> countBits;
	m_bufferBitSize -= countBits;

	// std::cout << "ReadResult: " << result << " " << m_bufferBitSize << std::endl;
	return result;
}

uint32_t BitReader::ReadVBR32(size_t countBits)
{
	// std::cout << "ReadVBR32: " << countBits << " " << std::endl;
	constexpr size_t ResultBitSize = sizeof(uint32_t) * 8;

	// Setup the continuation and value masks
	uint32_t continuationFlagMask = 0x1 << (countBits - 1);
	uint32_t valueMask = continuationFlagMask - 1;

	// Read chucks until the upper flag is not set
	uint32_t result = 0;
	uint32_t index = 0;
	uint32_t maxIndex = ResultBitSize - (countBits - 1);
	while (index <= maxIndex)
	{
		// Read the next chunk
		uint32_t chunk = Read(countBits);

		// Combine the new chunk into the final result
		result |= (chunk & valueMask) << index;

		// Check if we have a continuation flag set
		if ((chunk & continuationFlagMask) == 0)
			return result;

		// Update the index offset for the current value bits
		index += countBits - 1;
	}

	std::cout << countBits << std::endl;
	throw std::runtime_error("Ran out of room in the requested buffer size: " + std::to_string(index));
}

// TODO: Template implementation for both types
uint64_t BitReader::ReadVBR64(size_t countBits)
{
	// std::cout << "ReadVBR64: " << countBits << " " << std::endl;
	constexpr size_t ResultBitSize = sizeof(uint64_t) * 8;

	// Setup the continuation and value masks
	uint64_t continuationFlagMask = 0x1 << (countBits - 1);
	uint64_t valueMask = continuationFlagMask - 1;

	// Read chucks until the upper flag is not set
	uint64_t result = 0;
	uint64_t index = 0;
	uint64_t maxIndex = ResultBitSize - (countBits - 1);
	while (index <= maxIndex)
	{
		// Read the next chunk
		uint32_t chunk = Read(countBits);

		// Combine the new chunk into the final result
		result |= (chunk & valueMask) << index;

		// Check if we have a continuation flag set
		if ((chunk & continuationFlagMask) == 0)
			return result;

		// Update the index offset for the current value bits
		index += countBits - 1;
	}

	std::cout << countBits << std::endl;
	throw std::runtime_error("Ran out of room in the requested buffer size: " + std::to_string(index));
}

void BitReader::Align32Bit()
{
	// Calcuate the offset to get to the next 32 bit word
	auto currentBitOffset = GetBitIndex();
	auto currentWordOffset = currentBitOffset % 32;
	if (currentWordOffset != 0)
	{
		auto nextWordOffset = 32 - currentWordOffset;

		// Read to the next word
		// std::cout << "Align32Bit: " << currentBitOffset << " " << currentWordOffset << " " << nextWordOffset << std::endl;
		auto ignoreResult = Read(nextWordOffset);

		// std::cout << "Align32Bit SKIP: 0x" << std::hex << ignoreResult << std::dec << std::endl;
	}
}

size_t BitReader::GetBitIndex()
{
	size_t streamByteLocation = m_stream.tellg();
	return (streamByteLocation * 8) - m_bufferBitSize;
}

void BitReader::SeekByteOffset(size_t offset)
{
	// std::cout << "SeekByteOffset: " << offset << std::endl;
	if ((m_bufferBitSize % 8) != 0)
		throw std::runtime_error("Cannot seek byte offset when not aligned to a byte.");

	// First subtract the number of bytes available in the buffer
	auto bufferByteSize = m_bufferBitSize / 8;
	auto bytesRemovedFromBuffer = std::min(bufferByteSize, offset);
	m_bufferBitSize -= bytesRemovedFromBuffer * 8;
	offset -= bytesRemovedFromBuffer;

	// Perform the seek if required
	if (offset > 0)
	{
		if (!m_stream.seekg(offset, m_stream.cur))
			throw std::runtime_error("Failed to seek stream.");
	}
}

bool BitReader::AtEndOfStream() const
{
	bool atEndOfStream = m_stream.peek() == std::char_traits<char>::eof();
	// std::cout << "AtEndOfStream: " << atEndOfStream << " " << m_bufferBitSize << std::endl;
	return atEndOfStream && m_bufferBitSize == 0;
}