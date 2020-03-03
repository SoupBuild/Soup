// <copyright file="LzmaInStream.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// A input stream wrapper around the standard library istream
	/// to allow read operations for the LZMA SDK
	/// </summary>
	class LzmaInStream : public LzmaSdk::IInStream
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LzmaInStream"/> class.
		/// </summary>
		LzmaInStream(std::shared_ptr<System::IInputFile> file) :
			_file(std::move(file))
		{
			if (_file == nullptr)
				throw std::runtime_error("Null file");
		}

		uint32_t Read(void* data, uint32_t size) override final
		{
			auto& stream = _file->GetInStream();
			stream.read((char*)data, size);
			if (stream.bad())
				throw std::runtime_error("Failed to write.");

			return size;
		}

		uint64_t Seek(int64_t offset, LzmaSdk::StreamSeekOrigin origin) override final
		{
			std::istream::seekdir dir;
			switch (origin)
			{
				case LzmaSdk::StreamSeekOrigin::Begin:
					dir = std::istream::beg;
					break;
				case LzmaSdk::StreamSeekOrigin::Current:
					dir = std::istream::cur;
					break;
				case LzmaSdk::StreamSeekOrigin::End:
					dir = std::istream::end;
					break;
				default:
					throw std::runtime_error("Unknown seek origin.");
			}

			auto& stream = _file->GetInStream();
			stream.seekg(offset, dir);
			if (stream.bad())
				throw std::runtime_error("Failed to write.");

			return stream.tellg();
		}

	private:
		std::shared_ptr<System::IInputFile> _file;
	};
}
