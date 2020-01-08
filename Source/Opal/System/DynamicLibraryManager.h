// <copyright file="DynamicLibraryManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// A platform specific dynamic library manager
	/// </summary>
	export class Library
	{
	public:
		Library(HINSTANCE handle) :
			_handle(handle)
		{
		}

		Library(Library&& other) :
			_handle(std::move(other._handle))
		{
			other._handle = nullptr;
		}

		Library(Library& other) = delete;

		~Library()
		{
			// Free the DLL module.
			if (_handle != nullptr)
			{
				auto result = FreeLibrary(_handle);
				// Ignore failures
				_handle = nullptr;
			}
		}

		typedef void (*StringCallback)(const char*);

		void EnumerateDLLFunctions(StringCallback callback)
		{
			// If the handle is valid, try to get the function address.
			if (_handle == nullptr)
				throw "ERROR Failed to get library handle";

			auto header = (PIMAGE_NT_HEADERS)((BYTE*)_handle + ((PIMAGE_DOS_HEADER)_handle)->e_lfanew);
			if (header->Signature != IMAGE_NT_SIGNATURE)
				throw "Signature is not IMAGE_NT_SIGNATURE";
			if (header->OptionalHeader.NumberOfRvaAndSizes == 0)
				throw "No Rva and sizes";

			auto exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)_handle + 
				header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
			if (exports->AddressOfNames == 0)
				throw "AddressOfNames is null";

			auto names = (BYTE**)((size_t)_handle + exports->AddressOfNames);
			for (int i = 0; i < exports->NumberOfNames; i++)
				callback((char*)_handle + (size_t)names[i]);
		}

		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		void* GetFunction(const char* name)
		{
			// If the handle is valid, try to get the function address.
			if (_handle == nullptr)
				throw "ERROR Failed to get library handle";

			auto processAddress = GetProcAddress(_handle, name); 
 
			// Verify the address is valid
			if (processAddress == nullptr)
				throw GetLastError();

			return (void*)processAddress;
		}

		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		template <typename T>
		T GetFunction(const char* name)
		{
			auto typeProcessAddress = (T)GetFunction(name);

			// Verify the type matches
			if (typeProcessAddress == nullptr)
				throw "Failed to cast finction address.";

			return typeProcessAddress;
		}

	private:
		HINSTANCE _handle;
	};

	/// <summary>
	/// A platform specific dynamic library manager
	/// </summary>
	export class DynamicLibraryManager
	{
	public:
		static Library LoadDynamicLibrary(const char* libraryName)
		{
			// Get a handle to the DLL module.
			auto libraryHandle = ::LoadLibrary(TEXT(libraryName));

			// If the handle is valid, try to get the function address.
			if (libraryHandle == nullptr)
			{
				auto lastError = GetLastError();
				throw "ERROR Failed to get library handle";
			}

			return Library(libraryHandle);
		}
	};
}
