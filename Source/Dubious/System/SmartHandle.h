// <copyright file="SmartHandle.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Dubious::System
{
	struct SmartHandle
	{
		SmartHandle(HANDLE handle) :
			_handle(handle)
		{
		}

		SmartHandle() :
			_handle(INVALID_HANDLE_VALUE)
		{
		}

		~SmartHandle()
		{
			Close();
		}

		void Close()
		{
			if (_handle != INVALID_HANDLE_VALUE)
			{
				if (!CloseHandle(_handle))
					throw "TODO: Better error";
				_handle = INVALID_HANDLE_VALUE;
			}
		}

		HANDLE _handle;
	};
}
