// <copyright file="MockFileSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"

namespace Opal::System
{
	export class MockFile
	{
	public:
		MockFile(std::stringstream content, std::time_t lastWriteTime) :
			Content(std::move(content)),
			LastWriteTime(lastWriteTime)
		{
		}

		MockFile(std::stringstream content) :
			Content(std::move(content)),
			LastWriteTime()
		{
		}

		MockFile(std::time_t lastWriteTime) :
			Content(),
			LastWriteTime(lastWriteTime)
		{
		}

		MockFile() :
			Content(),
			LastWriteTime()
		{
		}

		std::stringstream Content;
		std::time_t LastWriteTime;
	};
	
	class MockOutputFile : public IOutputFile
	{
	public:
		MockOutputFile(std::shared_ptr<MockFile> file) :
			_file(std::move(file))
		{
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		std::ostream& GetOutStream() override final
		{
			return _file->Content;
		}

		/// <summary>
		/// Close the file stream
		/// </summary>
		void Close() override final
		{
			// Ignore for now
		}
	
	private:
		std::shared_ptr<MockFile> _file;
	};
	
	class MockInputFile : public IInputFile
	{
	public:
		MockInputFile(std::shared_ptr<MockFile> file) :
			_file(std::move(file))
		{
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		std::istream& GetInStream() override final
		{
			return _file->Content;
		}

		/// <summary>
		/// Close the file stream
		/// </summary>
		void Close() override final
		{
			// Ignore for now
		}
	
	private:
		std::shared_ptr<MockFile> _file;
	};
}
