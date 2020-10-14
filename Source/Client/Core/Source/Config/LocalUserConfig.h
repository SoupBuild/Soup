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
			_windowsSDKIncludePaths(std::nullopt),
			_windowsSDKLibraryPaths(std::nullopt)
		{
		}

		LocalUserConfig(
			std::string runtimeCompiler,
			std::optional<std::string> msvcRootPath,
			std::optional<std::string> clangToolPath,
			std::optional<std::vector<std::string>> windowsSDKIncludePaths,
			std::optional<std::vector<std::string>> windowsSDKLibraryPaths) :
			_runtimeCompiler(std::move(runtimeCompiler)),
			_msvcRootPath(std::move(msvcRootPath)),
			_clangToolPath(std::move(clangToolPath)),
			_windowsSDKIncludePaths(std::move(windowsSDKIncludePaths)),
			_windowsSDKLibraryPaths(std::move(windowsSDKLibraryPaths))
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
		/// Gets the Windows SDK Include paths
		/// </summary>
		bool HasWindowsSDKIncludePaths() const
		{
			return _windowsSDKIncludePaths.has_value();
		}

		const std::vector<std::string>& GetWindowsSDKIncludePaths() const
		{
			if (!HasWindowsSDKIncludePaths())
				throw std::runtime_error("No Windows SDK Include Paths.");
			return _windowsSDKIncludePaths.value();
		}

		/// <summary>
		/// Gets the Windows SDK Library paths
		/// </summary>
		bool HasWindowsSDKLibraryPaths() const
		{
			return _windowsSDKLibraryPaths.has_value();
		}

		const std::vector<std::string>& GetWindowsSDKLibraryPaths() const
		{
			if (!HasWindowsSDKLibraryPaths())
				throw std::runtime_error("No Windows SDK Library Paths.");
			return _windowsSDKLibraryPaths.value();
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const LocalUserConfig& rhs) const
		{
			return _runtimeCompiler == rhs._runtimeCompiler &&
				_msvcRootPath == rhs._msvcRootPath &&
				_clangToolPath == rhs._clangToolPath &&
				_windowsSDKIncludePaths == rhs._windowsSDKIncludePaths &&
				_windowsSDKLibraryPaths == rhs._windowsSDKLibraryPaths;
		}

		bool operator !=(const LocalUserConfig& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::string _runtimeCompiler;
		std::optional<std::string> _msvcRootPath;
		std::optional<std::string> _clangToolPath;
		std::optional<std::vector<std::string>> _windowsSDKIncludePaths;
		std::optional<std::vector<std::string>> _windowsSDKLibraryPaths;
	};
}
