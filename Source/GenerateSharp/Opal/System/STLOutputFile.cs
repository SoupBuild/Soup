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
		STLOutputFile(std::fstream stream) :
			_stream(std::move(stream))
		{
		}

		/// <summary>
		/// Get the file stream
		/// </summary>
		std::ostream& GetOutStream() override final
		{
			return _stream;
		}

		/// <summary>
		/// Close the file stream
		/// </summary>
		void Close() override final
		{
			_stream.close();
		}

	private:
		std::fstream _stream;
	};
}
