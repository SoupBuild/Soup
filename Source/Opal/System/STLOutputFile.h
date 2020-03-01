// <copyright file="STLOutputFile.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IOutputFile.h"

namespace Opal::System
{

	/// <summary>
	/// The standard library output file implementation
	/// </summary>
	class STLOutputFile : public IOutputFile
	{
	public:
		STLOutputFile(std::ofstream stream) :
			_stream(std::move(stream))
		{
		}

		/// <summary>
		/// Gets the current directory for the running processes
		/// </summary>
		std::ostream& GetStream() override final
		{
			return _stream;
		}

		/// <summary>
		/// Gets a value indicating whether the directory/file exists
		/// </summary>
		void Close() override final
		{
			_stream.close();
		}

	private:
		std::ofstream _stream;
	};
}
