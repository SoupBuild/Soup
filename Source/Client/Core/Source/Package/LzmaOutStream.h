// <copyright file="LzmaOutStream.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// A output stream wrapper around the standard library ostream
	/// to allow write operations for the LZMA SDK
	/// </summary>
	class LzmaOutStream : public LzmaSdk::IOutStream
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LzmaOutStream"/> class.
		/// </summary>
		LzmaOutStream(std::shared_ptr<System::IOutputFile> file) :
			_file(std::move(file))
		{
			if (_file == nullptr)
				throw std::runtime_error("Null file");
		}

		uint32_t Write(const void* data, uint32_t size) override final
		{
			auto& stream = _file->GetOutStream();
			stream.write((char*)data, size);
			if (stream.bad())
				throw std::runtime_error("Failed to write.");

			return size;
		}

		uint64_t Seek(int64_t offset, LzmaSdk::StreamSeekOrigin origin) override final
		{
			std::ostream::seekdir dir;
			switch (origin)
			{
				case LzmaSdk::StreamSeekOrigin::Begin:
					dir = std::ostream::beg;
					break;
				case LzmaSdk::StreamSeekOrigin::Current:
					dir = std::ostream::cur;
					break;
				case LzmaSdk::StreamSeekOrigin::End:
					dir = std::ostream::end;
					break;
				default:
					throw std::runtime_error("Unknown seek origin.");
			}

			auto& stream = _file->GetOutStream();
			stream.seekp(offset, dir);
			if (stream.bad())
				throw std::runtime_error("Failed to write.");

			return stream.tellp();
		}

		virtual void SetSize(uint64_t /*size*/) override final
		{
			throw std::runtime_error("Not implemented");
		}

	private:
		std::shared_ptr<System::IOutputFile> _file;
	};
}
