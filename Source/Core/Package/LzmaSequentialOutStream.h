// <copyright file="LzmaSequentialOutStream.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
	/// <summary>
	/// A output stream wrapper around the standard library ostream
	/// to allow write operations for the LZMA SDK
	/// </summary>
	class LzmaSequentialOutStream : public LzmaSdk::ISequentialOutStream
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LzmaSequentialOutStream"/> class.
		/// </summary>
		LzmaSequentialOutStream(std::shared_ptr<System::IOutputFile> file) :
			_file(std::move(file))
		{
			if (_file == nullptr)
				throw std::runtime_error("Null file");
		}

		uint32_t Write(const void* data, uint32_t size) override final
		{
			_file->GetStream().write((char*)data, size);
			if (_file->GetStream().bad())
				throw std::runtime_error("Failed to write.");

			return size;
		}

	private:
		std::shared_ptr<System::IOutputFile> _file;
	};
}
