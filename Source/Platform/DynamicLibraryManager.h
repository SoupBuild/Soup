// <copyright file="DynamicLibraryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Platform
{
	/// <summary>
	/// A platform specific dynamic library manager
	/// </summary>
	export class DynamicLibraryManager
	{
	public:
		typedef int (*DynamicMethod)();

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		static int Execute(const char* libraryName, const char* processName)
		{
			// Get a handle to the DLL module.
			HINSTANCE libraryHandle = LoadLibrary(TEXT(libraryName));

			// If the handle is valid, try to get the function address.
			if (libraryHandle == nullptr)
				throw "ERROR Failed to get library handle";

			auto processAddress = (DynamicMethod)GetProcAddress(libraryHandle, processName); 
 
			// Verify the address is valid
			if (processAddress == nullptr)
				throw GetLastError();
			
			// Invoke the requested process
			auto result = processAddress();

			// Free the DLL module.
			if (!FreeLibrary(libraryHandle))
				throw "ERROR Failed to free";

			return result;
		}
	};
}
