// <copyright file="WindowsDynamicLibrary.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// A platform specific dynamic library
	/// </summary>
	export class WindowsDynamicLibrary : public ILibrary
	{
	public:
		WindowsDynamicLibrary(HINSTANCE handle) :
			_handle(handle)
		{
		}

		WindowsDynamicLibrary(WindowsDynamicLibrary&& other) :
			_handle(std::move(other._handle))
		{
			other._handle = nullptr;
		}

		WindowsDynamicLibrary(WindowsDynamicLibrary& other) = delete;

		~WindowsDynamicLibrary()
		{
			// Free the DLL module.
			if (_handle != nullptr)
			{
				auto result = FreeLibrary(_handle);
				(result);
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
			for (auto i = 0u; i < exports->NumberOfNames; i++)
				callback((char*)_handle + (size_t)names[i]);
		}

		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		void* GetFunction(std::string_view name) override final
		{
			// If the handle is valid, try to get the function address.
			if (_handle == nullptr)
				throw "ERROR Failed to get library handle";

			auto processAddress = GetProcAddress(_handle, name.data());
 
			// Verify the address is valid
			if (processAddress == nullptr)
				throw GetLastError();

			return (void*)processAddress;
		}

	private:
		HINSTANCE _handle;
	};
}
