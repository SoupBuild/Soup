// <copyright file="LocalUserConfig.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The local user configuration
	/// </summary>
	export class LocalUserConfig
	{
	public:
		LocalUserConfig() :
			_runtimeCompiler(),
			_msvcToolPath(std::nullopt),
			_clangToolPath(std::nullopt)
		{
		}

		LocalUserConfig(
			std::string runtimeCompiler,
			std::optional<std::string> msvcToolPath,
			std::optional<std::string> clangToolPath) :
			_runtimeCompiler(std::move(runtimeCompiler)),
			_msvcToolPath(std::move(msvcToolPath)),
			_clangToolPath(std::move(clangToolPath))
		{
		}

		/// <summary>
		/// Gets the runtime compiler
		/// </summary>
		const std::string& GetRuntimeCompiler() const
		{
			return _runtimeCompiler;
		}

		/// <summary>
		/// Gets the MSVC tool path
		/// </summary>
		bool HasMSVCToolPath() const
		{
			return _msvcToolPath.has_value();
		}

		const std::string& GetMSVCToolPath() const
		{
			if (!HasMSVCToolPath())
				throw std::runtime_error("No MSVC Tool Path.");
			return _msvcToolPath.value();
		}

		/// <summary>
		/// Gets the Clang tool path
		/// </summary>
		bool HasClangToolPath() const
		{
			return _clangToolPath.has_value();
		}

		const std::string& GetClangToolPath() const
		{
			if (!HasClangToolPath())
				throw std::runtime_error("No Clang Tool Path.");
			return _clangToolPath.value();
		}

	private:
		std::string _runtimeCompiler;
		std::optional<std::string> _msvcToolPath;
		std::optional<std::string> _clangToolPath;
	};
}
