// <copyright file="LinkArguments.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The enumeration of link targets
	/// </summary>
	export enum class LinkTarget
	{
		/// <summary>
		/// Static Library
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// Executable
		/// </summary>
		Executable,
	};

	std::string ToString(LinkTarget value)
	{
		switch (value)
		{
			case LinkTarget::StaticLibrary:
				return "StaticLibrary";
			case LinkTarget::Executable:
				return "Executable";
			default:
				throw std::runtime_error("Unknown LinkTarget");
		}
	}

	/// <summary>
	/// The shared link arguments
	/// </summary>
	export class LinkArguments
	{
	public:
		/// <summary>
		/// Gets or sets the target file
		/// </summary>
		Path TargetFile;

		/// <summary>
		/// Gets or sets the target type
		/// </summary>
		LinkTarget TargetType;

		/// <summary>
		/// Gets or sets the root directory
		/// </summary>
		Path RootDirectory;

		/// <summary>
		/// Gets or sets the list of object files
		/// </summary>
		std::vector<Path> ObjectFiles;

		/// <summary>
		/// Gets or sets the list of library files
		/// </summary>
		std::vector<Path> LibraryFiles;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const LinkArguments& rhs) const
		{
			return TargetFile == rhs.TargetFile &&
				TargetType == rhs.TargetType &&
				RootDirectory == rhs.RootDirectory &&
				ObjectFiles == rhs.ObjectFiles &&
				LibraryFiles == rhs.LibraryFiles;
		}

		bool operator !=(const LinkArguments& rhs) const
		{
			return !(*this == rhs);
		}

		std::string ToString() const
		{
			auto stringBuilder = std::stringstream();
			stringBuilder << "[" <<
				TargetFile.ToString() << ", " <<
				::Soup::ToString(TargetType) << ", " <<
				RootDirectory.ToString() << ", [";

			for (auto& value : ObjectFiles)
				stringBuilder << value.ToString() << ", ";

			stringBuilder << "], [";

			for (auto& value : LibraryFiles)
				stringBuilder << value.ToString() << ", ";

			stringBuilder << "]";

			return stringBuilder.str();
		}
	};
}
