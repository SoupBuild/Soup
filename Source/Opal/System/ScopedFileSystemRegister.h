// <copyright file="ScopedFileSystemRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IFileSystem.h"

namespace Opal::System
{
	/// <summary>
	/// A scopped file system registration helper
	/// </summary>
	export class ScopedFileSystemRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedFileSystemRegister'/> class.
		/// </summary>
		ScopedFileSystemRegister(std::shared_ptr<IFileSystem> listener)
		{
			IFileSystem::Register(std::move(listener));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedFileSystemRegister'/> class.
		/// </summary>
		~ScopedFileSystemRegister()
		{
			IFileSystem::Register(nullptr);
		}
	};
}
