// <copyright file="BuiltInLanguagePackage.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	#ifdef SOUP_BUILD
	export
	#endif
	struct BuiltInLanguagePackage
	{
		BuiltInLanguagePackage(
			std::string languageSafeName,
			std::string extensionName,
			SemanticVersion extensionVersion,
			std::vector<Path> extensionFiles) :
			LanguageSafeName(std::move(languageSafeName)),
			ExtensionName(std::move(extensionName)),
			ExtensionVersion(extensionVersion),
			ExtensionFiles(std::move(extensionFiles))
		{
		}

		std::string LanguageSafeName;
		std::string ExtensionName;
		SemanticVersion ExtensionVersion;
		std::vector<Path> ExtensionFiles;
	};
}
