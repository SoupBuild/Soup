// <copyright file="STLInputFile.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IInputFile.h"

namespace Opal::System
{

	/// <summary>
	/// The standard library input file implementation
	/// </summary>
	class STLInputFile : public IInputFile
	{
	public:
		STLInputFile(std::fstream stream) :
			_stream(std::move(stream))
		{
		}

		/// <summary>
		/// Gets the file stream
		/// </summary>
		std::istream& GetInStream() override final
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
