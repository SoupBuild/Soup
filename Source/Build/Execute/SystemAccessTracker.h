#pragma once 

namespace Soup::Build
{
	class SystemAccessTracker : public Monitor::IDetourCallback
	{
	public:
		SystemAccessTracker() :
			m_input(),
			m_output()
		{
		}

		void GetInputFiles()
		{
			for (auto& file : m_input)
			{
				Log::HighPriority("Input: " + file);
			}
		}

		void GetOutputFiles()
		{
			for (auto& file : m_output)
			{
				Log::HighPriority("Output: " + file);
			}
		}

		void OnShutdown() override final
		{
		}

		void OnError(std::string_view message) override final
		{
			Log::Error("SystemAccessTracker Error: " + std::string(message));
		}

		// FileApi
		void OnAreFileApisANSI(bool result) override final
		{
		}

		void OnCompareFileTime(int32_t result) override final
		{
		}

		void OnCreateDirectoryA(std::string_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateFile2(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition) override final
		{
			bool isRead = (desiredAccess & GENERIC_READ) != 0;
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName);
			}
			else
			{
				TouchFileRead(fileName);
			}
		}

		void OnCreateFileA(
			std::string_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint32_t flagsAndAttributes) override final
		{
			bool isRead = (desiredAccess & GENERIC_READ) != 0;
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName);
			}
			else
			{
				TouchFileRead(fileName);
			}
		}

		void OnCreateFileW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint32_t flagsAndAttributes) override final
		{
			bool isRead = (desiredAccess & GENERIC_READ) != 0;
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName);
			}
			else
			{
				TouchFileRead(fileName);
			}
		}

		void OnDefineDosDeviceW(uint32_t flags, std::wstring_view deviceName, std::wstring_view targetPath, bool result) override final
		{
		}

		void OnDeleteFileA(std::string_view fileName, bool result) override final
		{
			TouchFileDelete(fileName);
		}

		void OnDeleteFileW(std::wstring_view fileName, bool result) override final
		{
			TouchFileDelete(fileName);
		}

		void OnDeleteVolumeMountPointW(std::wstring_view volumeMountPoint, bool result) override final
		{
		}

		void OnFileTimeToLocalFileTime(bool result) override final
		{
		}

		void OnFindClose(bool result) override final
		{
		}

		void OnFindCloseChangeNotification(bool result) override final
		{
		}

		void OnFindFirstChangeNotificationA(std::string_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstFileA(std::string_view fileName) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstFileW(std::wstring_view fileName) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstFileExA(std::string_view fileName) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstFileExW(std::wstring_view fileName) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t flags) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstStreamW(std::wstring_view fileName) override final
		{
			// TODO: Track search paths
		}

		void OnFindFirstVolumeW(std::wstring_view fileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFindNextChangeNotification(bool result) override final
		{
		}

		void OnFindNextFileA(bool result) override final
		{
		}

		void OnFindNextFileW(bool result) override final
		{
		}

		void OnFindNextFileNameW(bool result) override final
		{
		}

		void OnFindNextStreamW(bool result) override final
		{
		}

		void OnFindNextVolumeW(bool result) override final
		{
		}

		void OnFindVolumeClose(bool result) override final
		{
		}

		void OnFlushFileBuffers(bool result) override final
		{
		}

		void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool result) override final
		{
		}

		void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool result) override final
		{
		}

		void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool result) override final
		{
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool result) override final
		{
		}

		void OnGetDriveTypeA(std::string_view rootPathName, uint32_t result) override final
		{
		}

		void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t result) override final
		{
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t result) override final
		{
			TouchFileRead(fileName);
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result) override final
		{
			TouchFileRead(fileName);
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool result) override final
		{
			TouchFileRead(fileName);
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool result) override final
		{
			TouchFileRead(fileName);
		}

		void OnGetFileInformationByHandle(bool result) override final
		{
		}

		void OnGetFileSize(uint32_t result) override final
		{
		}

		void OnGetFileSizeEx(uint32_t result) override final
		{
		}

		void OnGetFileTime(bool result) override final
		{
		}

		void OnGetFileType(uint32_t result) override final
		{
		}

		void OnGetFinalPathNameByHandleA(uint32_t result) override final
		{
		}

		void OnGetFinalPathNameByHandleW(uint32_t result) override final
		{
		}

		void OnGetFullPathNameA(std::string_view fileName, uint32_t result) override final
		{
		}

		void OnGetFullPathNameW(std::wstring_view fileName, std::wstring_view buffer, uint32_t result) override final
		{
		}

		void OnGetLogicalDrives(uint32_t result) override final
		{
		}

		void OnGetLogicalDriveStringsW(std::wstring_view buffer, uint32_t result) override final
		{
		}

		void OnGetLongPathNameA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetLongPathNameW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetShortPathNameW(std::wstring_view longPath, std::wstring_view shortPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetTempFileNameA(std::string_view pathName, std::string_view prefixString, uint32_t unique, std::string_view tempFileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view prefixString, uint32_t unique, std::wstring_view tempFileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetTempPathA(std::string_view buffer, uint32_t result) override final
		{
		}

		void OnGetTempPathW(std::wstring_view buffer, uint32_t result) override final
		{
		}

		void OnGetVolumeInformationA(bool result) override final
		{
		}

		void OnGetVolumeInformationW(bool result) override final
		{
		}

		void OnGetVolumeInformationByHandleW(bool result) override final
		{
		}

		void OnGetVolumeNameForVolumeMountPointW(bool result) override final
		{
		}

		void OnGetVolumePathNamesForVolumeNameW(bool result) override final
		{
		}

		void OnGetVolumePathNameW(std::wstring_view filename, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnLocalFileTimeToFileTime(bool result) override final
		{
		}

		void OnLockFile(bool result) override final
		{
		}

		void OnLockFileEx(bool result) override final
		{
		}

		void OnQueryDosDeviceW(std::wstring_view deviceName, uint32_t result) override final
		{
		}

		void OnReadFile(bool result) override final
		{
		}

		void OnReadFileEx(bool result) override final
		{
		}

		void OnReadFileScatter(bool result) override final
		{
		}

		void OnRemoveDirectoryA(std::string_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnRemoveDirectoryW(std::wstring_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetEndOfFile(bool result) override final
		{
		}
		
		void OnSetFileApisToANSI() override final
		{
		}

		void OnSetFileApisToOEM() override final
		{
		}

		void OnSetFileAttributesA(std::string_view fileName, bool result) override final
		{
			TouchFileWrite(fileName);
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool result) override final
		{
			TouchFileWrite(fileName);
		}

		void OnSetFileInformationByHandle(bool result) override final
		{
		}

		void OnSetFileIoOverlappedRange(bool result) override final
		{
		}

		void OnSetFilePointer(uint32_t result) override final
		{
		}
		
		void OnSetFilePointerEx(bool result) override final
		{
		}

		void OnSetFileTime(bool result) override final
		{
		}

		void OnSetFileValidData(bool result) override final
		{
		}

		void OnUnlockFile(bool result) override final
		{
		}

		void OnUnlockFileEx(bool result) override final
		{
		}

		void OnWriteFile(bool result) override final
		{
		}

		void OnWriteFileEx(bool result) override final
		{
		}

		void OnWriteFileGather(bool result) override final
		{
		}

		// LibLoaderApi
		void OnLoadLibraryA(std::string_view libFileName) override final
		{
		}

		void OnLoadLibraryW(std::wstring_view libFileName) override final
		{
		}

		void OnLoadLibraryExA(std::string_view libFileName) override final
		{
		}

		void OnLoadLibraryExW(std::wstring_view libFileName) override final
		{
		}

		// ProcessEnv
		void OnSearchPathA(std::string_view path, std::string_view fileName, std::string_view extension, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSearchPathW(std::wstring_view path, std::wstring_view fileName, std::wstring_view extension, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		// ProcessThreadsApi
		void OnCreateProcessA(std::string_view applicationName, bool result) override final
		{
		}

		void OnCreateProcessW(std::wstring_view applicationName, bool result) override final
		{
		}

		void OnCreateProcessAsUserA(std::string_view applicationName, bool result) override final
		{
		}

		void OnCreateProcessAsUserW(std::wstring_view applicationName, bool result) override final
		{
		}

		void OnExitProcess(uint32_t exitCode) override final
		{
		}

		// UndocumentedApi
		void OnPrivCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnPrivCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		// WinBase
		void OnCopyFileA(std::string_view existingFileName, std::string_view newFileName, bool failIfExists, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool failIfExists, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFile2(std::wstring_view existingFileName, std::wstring_view newFileName) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFileTransactedA(std::string_view existingFileName, std::string_view newFileName, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCopyFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result) override final
		{
			TouchFileRead(existingFileName);
			TouchFileWrite(newFileName);
		}

		void OnCreateDirectoryExA(std::string_view templateDirectory, std::string_view newDirectory, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateDirectoryExW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateDirectoryTransactedA(std::string_view templateDirectory, std::string_view newDirectory, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateDirectoryTransactedW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateFileTransactedA(std::string_view fileName, uint32_t desiredAccess, uint32_t shareMode) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateFileTransactedW(std::wstring_view fileName, uint32_t desiredAccess, uint32_t shareMode) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateHardLinkA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateHardLinkW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateHardLinkTransactedA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateHardLinkTransactedW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateSymbolicLinkA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateSymbolicLinkW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateSymbolicLinkTransactedA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnCreateSymbolicLinkTransactedW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnDecryptFileA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}
		
		void OnDecryptFileW(std::wstring_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnDeleteFileTransactedA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnDeleteFileTransactedW(std::wstring_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnEncryptFileA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnEncryptFileW(std::wstring_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFileEncryptionStatusA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFileEncryptionStatusW(std::wstring_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFindFirstFileNameTransactedW(std::wstring_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFindFirstFileTransactedA(std::string_view fileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFindFirstFileTransactedW(std::wstring_view fileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnFindFirstStreamTransactedW(std::wstring_view fileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetBinaryTypeA(std::string_view applicationName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetBinaryTypeW(std::wstring_view applicationName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetCompressedFileSizeTransactedA(std::string_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetCompressedFileSizeTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetDllDirectoryA(uint32_t result) override final
		{
		}

		void OnGetDllDirectoryW(uint32_t result) override final
		{
		}

		void OnGetFileAttributesTransactedA(std::string_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetFileAttributesTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetFileBandwidthReservation(bool result) override final
		{
		}
		
		void OnGetFileInformationByHandleEx(bool result) override final
		{
		}

		void OnGetFileSecurityA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetFullPathNameTransactedA(std::string_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetFullPathNameTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetLongPathNameTransactedA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetLongPathNameTransactedW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnGetQueuedCompletionStatus(bool result) override final
		{
		}

		void OnGetQueuedCompletionStatusEx(bool result) override final
		{
		}

		void OnGetShortPathNameA(std::string_view longPath, std::string_view shortPath, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnLoadModule(std::string_view moduleName, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnLoadPackagedLibrary(std::string_view libFileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileA(std::string_view existingFileName, std::string_view newFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileExA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileTransactedA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileWithProgressA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnMoveFileWithProgressW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnOpenEncryptedFileRawA(std::string_view fileName, uint32_t flags, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnOpenEncryptedFileRawW(std::wstring_view fileName, uint32_t flags, uint32_t result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnOpenFile(std::string_view fileName) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnOpenFileById() override final
		{
		}

		void OnReadEncryptedFileRaw(uint32_t result) override final
		{
		}

		void OnRemoveDirectoryTransactedA(std::string_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnRemoveDirectoryTransactedW(std::wstring_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnReOpenFile() override final
		{
		}

		void OnReplaceFileA(std::string_view replacedFileName, std::string_view replacementFileName, std::string_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnReplaceFileW(std::wstring_view replacedFileName, std::wstring_view replacementFileName, std::wstring_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetCurrentDirectoryA(std::string_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetCurrentDirectoryW(std::wstring_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetDllDirectoryA(std::string_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetDllDirectoryW(std::wstring_view pathName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileAttributesTransactedA(std::string_view pathName, uint32_t fileAttributes, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileAttributesTransactedW(std::wstring_view pathName, uint32_t fileAttributes, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileBandwidthReservation(bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileCompletionNotificationModes(bool result) override final
		{
		}

		void OnSetFileSecurityA(std::string_view fileName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileShortNameA(std::string_view shortName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetFileShortNameW(std::wstring_view shortName, bool result) override final
		{
			throw std::runtime_error("Not implemented");
		}

		void OnSetSearchPathMode(uint32_t flags, bool result) override final
		{
		}

		void OnWriteEncryptedFileRaw(uint32_t result) override final
		{
		}

	private:
		void TouchFileRead(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileRead(converter.to_bytes(fileName.data()));
		}

		void TouchFileRead(std::string_view fileName)
		{
			// Verify not a pipe name
			if (!fileName.starts_with("\\\\.\\pipe\\"))
			{
				auto filePath = Path(fileName);
				m_input.insert(filePath.ToString());
			}
		}

		void TouchFileWrite(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileWrite(converter.to_bytes(fileName.data()));
		}

		void TouchFileWrite(std::string_view fileName)
		{
			// Verify not a pipe name
			if (!fileName.starts_with("\\\\.\\pipe\\"))
			{
				auto filePath = Path(fileName);
				m_output.insert(filePath.ToString());
			}
		}

		void TouchFileDelete(std::string_view fileName)
		{
			auto filePath = Path(fileName);
			TouchFileDelete(filePath);
		}

		void TouchFileDelete(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			auto filePath = Path(converter.to_bytes(fileName.data()));
			TouchFileDelete(filePath);
		}

		void TouchFileDelete(Path& filePath)
		{
			// If this was an output file extract it as it was a transient file
			// TODO: May want to track if we created the file
			auto extractOutputResult = m_output.extract(filePath.ToString());
			auto extractInputResult = m_input.extract(filePath.ToString());
		}

		std::set<std::string> m_input;
		std::set<std::string> m_output;
	};
}
