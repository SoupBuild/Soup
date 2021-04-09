// <copyright file="ILibrary.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System
{
	/// <summary>
	/// The library interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	public interface ILibrary
	{
		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		// void* GetFunction(std::string_view name) = 0;

		/// <summary>
		/// Get a function by name and casted to the requested type
		/// </summary>
		//template <typename T>
		//T GetFunction(std::string_view name)
		//{
		//	auto typeProcessAddress = (T)GetFunction(name);

		//	// Verify the type matches
		//	if (typeProcessAddress == nullptr)
		//		throw "Failed to cast finction address.";

		//	return typeProcessAddress;
		//}
	}
}
