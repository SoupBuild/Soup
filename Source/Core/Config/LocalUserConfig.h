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
			_msvcRootPath(std::nullopt),
			_clangToolPath(std::nullopt),
			_windowsSDKIncludesPath(std::nullopt),
			_windowsSDKLibrariesPath(std::nullopt)
		{
		}

		LocalUserConfig(
			std::string runtimeCompiler,
			std::optional<std::string> msvcRootPath,
			std::optional<std::string> clangToolPath,
			std::optional<std::string> windowsSDKIncludesPath,
			std::optional<std::string> windowsSDKLibrariesPath) :
			_runtimeCompiler(std::move(runtimeCompiler)),
			_msvcRootPath(std::move(msvcRootPath)),
			_clangToolPath(std::move(clangToolPath)),
			_windowsSDKIncludesPath(std::move(windowsSDKIncludesPath)),
			_windowsSDKLibrariesPath(std::move(windowsSDKLibrariesPath))
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
		/// Gets the MSVC root path
		/// </summary>
		bool HasMSVCRootPath() const
		{
			return _msvcRootPath.has_value();
		}

		const std::string& GetMSVCRootPath() const
		{
			if (!HasMSVCRootPath())
				throw std::runtime_error("No MSVC Root Path.");
			return _msvcRootPath.value();
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

		/// <summary>
		/// Gets the Windows SDK Includes path
		/// </summary>
		bool HasWindowsSDKIncludesPath() const
		{
			return _windowsSDKIncludesPath.has_value();
		}

		const std::string& GetWindowsSDKIncludesPath() const
		{
			if (!HasWindowsSDKIncludesPath())
				throw std::runtime_error("No Windows SDK Includes Path.");
			return _windowsSDKIncludesPath.value();
		}

		/// <summary>
		/// Gets the Windows SDK Libraries path
		/// </summary>
		bool HasWindowsSDKLibrariesPath() const
		{
			return _windowsSDKLibrariesPath.has_value();
		}

		const std::string& GetWindowsSDKLibrariesPath() const
		{
			if (!HasWindowsSDKLibrariesPath())
				throw std::runtime_error("No Windows SDK Libraries Path.");
			return _windowsSDKLibrariesPath.value();
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const LocalUserConfig& rhs) const
		{
			return _runtimeCompiler == rhs._runtimeCompiler &&
				_msvcRootPath == rhs._msvcRootPath &&
				_clangToolPath == rhs._clangToolPath &&
				_windowsSDKIncludesPath == rhs._windowsSDKIncludesPath &&
				_windowsSDKLibrariesPath == rhs._windowsSDKLibrariesPath;
		}

		bool operator !=(const LocalUserConfig& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::string _runtimeCompiler;
		std::optional<std::string> _msvcRootPath;
		std::optional<std::string> _clangToolPath;
		std::optional<std::string> _windowsSDKIncludesPath;
		std::optional<std::string> _windowsSDKLibrariesPath;
	};
}
