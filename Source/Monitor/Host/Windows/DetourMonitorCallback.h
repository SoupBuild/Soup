#pragma once
#include "IDetourCallback.h"

namespace Monitor::Windows
{
	/// The monitor callback wrapper that maps Windows Detour events to the shared events
	class DetourMonitorCallback : public IDetourCallback
	{
	private:
		std::shared_ptr<IMonitorCallback> _callback;

	public:
		DetourMonitorCallback(
			std::shared_ptr<IMonitorCallback> callback) :
			_callback(std::move(callback))
		{
		}

		void OnInitialize() override final
		{
			Log::Diag("DetourMonitorCallback::OnInitialize");
		}

		void OnShutdown() override final
		{
			Log::Diag("DetourMonitorCallback::OnShutdown");
		}

		void OnError(std::string_view message) override final
		{
			Log::Error("DetourMonitorCallback::Error - " + std::string(message));
		}

		// FileApi
		void OnAreFileApisANSI(bool /*result*/) override final
		{
		}

		void OnCompareFileTime(int32_t /*result*/) override final
		{
		}

		void OnCreateDirectoryA(std::string_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileWrite(pathName, wasBlocked);
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileWrite(pathName, wasBlocked);
		}

		void OnCreateFile2(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName, wasBlocked);
			}
			else
			{
				bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
				TouchFileRead(fileName, exists, wasBlocked);
			}
		}

		void OnCreateFileA(
			std::string_view fileName,
			uint32_t desiredAccess,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint32_t flagsAndAttributes,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			bool isDeleteOnClose = (flagsAndAttributes & FILE_FLAG_DELETE_ON_CLOSE) != 0;

			// If the file will be deleted it is a transient file
			if (isDeleteOnClose && !wasBlocked)
			{
				TouchFileDeleteOnClose(fileName);
			}

			if (isWrite)
			{
				TouchFileWrite(fileName, wasBlocked);
			}
			else
			{
				bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
				TouchFileRead(fileName, exists, wasBlocked);
			}
		}

		void OnCreateFileW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint32_t flagsAndAttributes,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			bool isDeleteOnClose = (flagsAndAttributes & FILE_FLAG_DELETE_ON_CLOSE) != 0;

			// If the file will be deleted it is a transient file
			if (isDeleteOnClose && !wasBlocked)
			{
				TouchFileDeleteOnClose(fileName);
			}

			if (isWrite)
			{
				TouchFileWrite(fileName, wasBlocked);
			}
			else
			{
				bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
				TouchFileRead(fileName, exists, wasBlocked);
			}
		}

		void OnDefineDosDeviceW(uint32_t /*flags*/, std::wstring_view /*deviceName*/, std::wstring_view /*targetPath*/, bool /*result*/) override final
		{
		}

		void OnDeleteFileA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileDelete(fileName, wasBlocked);
		}

		void OnDeleteFileW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileDelete(fileName, wasBlocked);
		}

		void OnDeleteVolumeMountPointW(std::wstring_view /*volumeMountPoint*/, bool /*result*/) override final
		{
		}

		void OnFileTimeToLocalFileTime(bool /*result*/) override final
		{
		}

		void OnFindClose(bool /*result*/) override final
		{
		}

		void OnFindCloseChangeNotification(bool /*result*/) override final
		{
		}

		void OnFindFirstChangeNotificationA(std::string_view /*pathName*/, bool /*watchSubtree*/, uint32_t /*notifyFilter*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstChangeNotificationW(std::wstring_view /*pathName*/, bool /*watchSubtree*/, uint32_t /*notifyFilter*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstFileA(std::string_view /*fileName*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstFileW(std::wstring_view /*fileName*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstFileExA(std::string_view /*fileName*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstFileExW(std::wstring_view /*fileName*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstFileNameW(std::wstring_view /*fileName*/, uint32_t /*flags*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstStreamW(std::wstring_view /*fileName*/) override final
		{
			// TODO: Track search /*path*/s
		}

		void OnFindFirstVolumeW(std::wstring_view /*fileName*/) override final
		{
		}

		void OnFindNextChangeNotification(bool /*result*/) override final
		{
		}

		void OnFindNextFileA(bool /*result*/) override final
		{
		}

		void OnFindNextFileW(bool /*result*/) override final
		{
		}

		void OnFindNextFileNameW(bool /*result*/) override final
		{
		}

		void OnFindNextStreamW(bool /*result*/) override final
		{
		}

		void OnFindNextVolumeW(bool /*result*/) override final
		{
		}

		void OnFindVolumeClose(bool /*result*/) override final
		{
		}

		void OnFlushFileBuffers(bool /*result*/) override final
		{
		}

		void OnGetCompressedFileSizeA(std::string_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetCompressedFileSizeA Not implemented");
		}

		void OnGetCompressedFileSizeW(std::wstring_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetCompressedFileSizeW Not implemented");
		}

		void OnGetDiskFreeSpaceA(std::string_view  /*rootPathName*/, bool /*result*/) override final
		{
		}

		void OnGetDiskFreeSpaceW(std::wstring_view  /*rootPathName*/, bool /*result*/) override final
		{
		}

		void OnGetDiskFreeSpaceExA(std::string_view /*directoryName*/, bool /*result*/) override final
		{
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view /*directoryName*/, bool /*result*/) override final
		{
		}

		void OnGetDriveTypeA(std::string_view  /*rootPathName*/, uint32_t /*result*/) override final
		{
		}

		void OnGetDriveTypeW(std::wstring_view  /*rootPathName*/, uint32_t /*result*/) override final
		{
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t result, bool wasBlocked) override final
		{
			auto exists = result != INVALID_FILE_ATTRIBUTES;
			TouchFileRead(fileName, exists, wasBlocked);
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result, bool wasBlocked) override final
		{
			auto exists = result != INVALID_FILE_ATTRIBUTES;
			TouchFileRead(fileName, exists, wasBlocked);
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			TouchFileRead(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			TouchFileRead(fileName, result, wasBlocked);
		}

		void OnGetFileInformationByHandle(bool /*result*/) override final
		{
		}

		void OnGetFileSize(uint32_t /*result*/) override final
		{
		}

		void OnGetFileSizeEx(uint32_t /*result*/) override final
		{
		}

		void OnGetFileTime(bool /*result*/) override final
		{
		}

		void OnGetFileType(uint32_t /*result*/) override final
		{
		}

		void OnGetFinalPathNameByHandleA(uint32_t /*result*/) override final
		{
		}

		void OnGetFinalPathNameByHandleW(uint32_t /*result*/) override final
		{
		}

		void OnGetFullPathNameA(std::string_view /*fileName*/, uint32_t /*result*/) override final
		{
		}

		void OnGetFullPathNameW(std::wstring_view /*fileName*/, std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
		}

		void OnGetLogicalDrives(uint32_t /*result*/) override final
		{
		}

		void OnGetLogicalDriveStringsW(std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
		}

		void OnGetLongPathNameA(std::string_view /*shortPath*/, std::string_view /*longPath*/, uint32_t /*result*/) override final
		{
		}

		void OnGetLongPathNameW(std::wstring_view /*shortPath*/, std::wstring_view /*longPath*/, uint32_t /*result*/) override final
		{
		}

		void OnGetShortPathNameW(std::wstring_view /*longPath*/, std::wstring_view /*shortPath*/, uint32_t /*result*/) override final
		{
		}

		void OnGetTempFileNameA(
			std::string_view /*pathName*/,
			std::string_view /*prefixString*/,
			uint32_t /*unique*/,
			std::string_view /*tempFileName*/,
			uint32_t /*result*/) override final
		{
		}

		void OnGetTempFileNameW(
			std::wstring_view /*pathName*/,
			std::wstring_view /*prefixString*/,
			uint32_t /*unique*/,
			std::wstring_view /*tempFileName*/,
			uint32_t /*result*/) override final
		{
		}

		void OnGetTempPathA(std::string_view /*buffer*/, uint32_t /*result*/) override final
		{
		}

		void OnGetTempPathW(std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
		}

		void OnGetVolumeInformationA(bool /*result*/) override final
		{
		}

		void OnGetVolumeInformationW(bool /*result*/) override final
		{
		}

		void OnGetVolumeInformationByHandleW(bool /*result*/) override final
		{
		}

		void OnGetVolumeNameForVolumeMountPointW(bool /*result*/) override final
		{
		}

		void OnGetVolumePathNamesForVolumeNameW(bool /*result*/) override final
		{
		}

		void OnGetVolumePathNameW(std::wstring_view /*fileName*/, bool /*result*/) override final
		{
		}

		void OnLocalFileTimeToFileTime(bool /*result*/) override final
		{
		}

		void OnLockFile(bool /*result*/) override final
		{
		}

		void OnLockFileEx(bool /*result*/) override final
		{
		}

		void OnQueryDosDeviceW(std::wstring_view /*deviceName*/, uint32_t /*result*/) override final
		{
		}

		void OnReadFile(bool /*result*/) override final
		{
		}

		void OnReadFileEx(bool /*result*/) override final
		{
		}

		void OnReadFileScatter(bool /*result*/) override final
		{
		}

		void OnRemoveDirectoryA(std::string_view /*pathName*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnRemoveDirectoryA Not implemented");
		}

		void OnRemoveDirectoryW(std::wstring_view /*pathName*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnRemoveDirectoryW Not implemented");
		}

		void OnSetEndOfFile(bool /*result*/) override final
		{
		}
		
		void OnSetFileApisToANSI() override final
		{
		}

		void OnSetFileApisToOEM() override final
		{
		}

		void OnSetFileAttributesA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileWrite(fileName, wasBlocked);
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileWrite(fileName, wasBlocked);
		}

		void OnSetFileInformationByHandle(bool /*result*/) override final
		{
		}

		void OnSetFileIoOverlappedRange(bool /*result*/) override final
		{
		}

		void OnSetFilePointer(uint32_t /*result*/) override final
		{
		}
		
		void OnSetFilePointerEx(bool /*result*/) override final
		{
		}

		void OnSetFileTime(bool /*result*/) override final
		{
		}

		void OnSetFileValidData(bool /*result*/) override final
		{
		}

		void OnUnlockFile(bool /*result*/) override final
		{
		}

		void OnUnlockFileEx(bool /*result*/) override final
		{
		}

		void OnWriteFile(bool /*result*/) override final
		{
		}

		void OnWriteFileEx(bool /*result*/) override final
		{
		}

		void OnWriteFileGather(bool /*result*/) override final
		{
		}

		// LibLoaderApi
		void OnLoadLibraryA(std::string_view /*libFileName*/) override final
		{
		}

		void OnLoadLibraryW(std::wstring_view /*libFileName*/) override final
		{
		}

		void OnLoadLibraryExA(std::string_view /*libFileName*/) override final
		{
		}

		void OnLoadLibraryExW(std::wstring_view /*libFileName*/) override final
		{
		}

		// ProcessEnv
		void OnSearchPathA(std::string_view path, std::string_view fileName, std::string_view /*extension*/, uint32_t /*result*/) override final
		{
			SearchPath(path, fileName);
		}

		void OnSearchPathW(std::wstring_view path, std::wstring_view fileName, std::wstring_view /*extension*/, uint32_t /*result*/) override final
		{
			SearchPath(path, fileName);
		}

		// ProcessThreadsApi
		void OnCreateProcessA(bool wasDetoured, std::string_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(wasDetoured, applicationName);
		}

		void OnCreateProcessW(bool wasDetoured, std::wstring_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(wasDetoured, applicationName);
		}

		void OnCreateProcessAsUserA(std::string_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(false, applicationName);
		}

		void OnCreateProcessAsUserW(std::wstring_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(false, applicationName);
		}

		void OnExitProcess(uint32_t exitCode) override final
		{
			Log::Diag("DetourMonitorCallback::OnExitProcess - " + std::to_string(exitCode));
		}

		// UndocumentedApi
		void OnPrivCopyFileExA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnPrivCopyFileExW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		// WinBase
		void OnCopyFileA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool /*failIfExists*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFileW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool /*failIfExists*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFile2(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
			TouchFileRead(existingFileName, exists, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFileExA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFileExW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFileTransactedA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCopyFileTransactedW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnCreateDirectoryExA(
			std::string_view /*templateDirectory*/,
			std::string_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			TouchFileWrite(newDirectory, wasBlocked);
		}

		void OnCreateDirectoryExW(
			std::wstring_view /*templateDirectory*/,
			std::wstring_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			TouchFileWrite(newDirectory, wasBlocked);
		}

		void OnCreateDirectoryTransactedA(
			std::string_view /*templateDirectory*/,
			std::string_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			TouchFileWrite(newDirectory, wasBlocked);
		}

		void OnCreateDirectoryTransactedW(
			std::wstring_view /*templateDirectory*/,
			std::wstring_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			TouchFileWrite(newDirectory, wasBlocked);
		}

		void OnCreateFileTransactedA(
			std::string_view fileName,
			uint32_t desiredAccess,
			uint32_t /*shareMode*/,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName, wasBlocked);
			}
			else
			{
				bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
				TouchFileRead(fileName, exists, wasBlocked);
			}
		}

		void OnCreateFileTransactedW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t /*shareMode*/,
			uint64_t result,
			bool wasBlocked) override final
		{
			bool isWrite = (desiredAccess & GENERIC_WRITE) != 0;
			if (isWrite)
			{
				TouchFileWrite(fileName, wasBlocked);
			}
			else
			{
				bool exists = result != (uint64_t)INVALID_HANDLE_VALUE;
				TouchFileRead(fileName, exists, wasBlocked);
			}
		}

		void OnCreateHardLinkA(std::string_view /*fileName*/, std::string_view /*existingFileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateHardLinkA Not implemented");
		}

		void OnCreateHardLinkW(std::wstring_view /*fileName*/, std::wstring_view /*existingFileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateHardLinkW Not implemented");
		}

		void OnCreateHardLinkTransactedA(std::string_view /*fileName*/, std::string_view /*existingFileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateHardLinkTransactedA Not implemented");
		}

		void OnCreateHardLinkTransactedW(std::wstring_view /*fileName*/, std::wstring_view /*existingFileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateHardLinkTransactedW Not implemented");
		}

		void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(false, applicationName);
		}

		void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool /*result*/) override final
		{
			OnCreateProcess(false, applicationName);
		}

		void OnCreateSymbolicLinkA(std::string_view /*symlinkFileName*/, std::string_view /*targetFileName*/, uint32_t /*flags*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateSymbolicLinkA Not implemented");
		}

		void OnCreateSymbolicLinkW(std::wstring_view /*symlinkFileName*/, std::wstring_view /*targetFileName*/, uint32_t /*flags*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateSymbolicLinkW Not implemented");
		}

		void OnCreateSymbolicLinkTransactedA(std::string_view /*symlinkFileName*/, std::string_view /*targetFileName*/, uint32_t /*flags*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateSymbolicLinkTransactedA Not implemented");
		}

		void OnCreateSymbolicLinkTransactedW(std::wstring_view /*symlinkFileName*/, std::wstring_view /*targetFileName*/, uint32_t /*flags*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnCreateSymbolicLinkTransactedW Not implemented");
		}

		void OnDecryptFileA(std::string_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnDecryptFileA Not implemented");
		}
		
		void OnDecryptFileW(std::wstring_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnDecryptFileW Not implemented");
		}

		void OnDeleteFileTransactedA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileDelete(fileName, wasBlocked);
		}

		void OnDeleteFileTransactedW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			TouchFileDelete(fileName, wasBlocked);
		}

		void OnEncryptFileA(std::string_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnEncryptFileA Not implemented");
		}

		void OnEncryptFileW(std::wstring_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnEncryptFileW Not implemented");
		}

		void OnFileEncryptionStatusA(std::string_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnFileEncryptionStatusA Not implemented");
		}

		void OnFileEncryptionStatusW(std::wstring_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnFileEncryptionStatusW Not implemented");
		}

		void OnFindFirstFileNameTransactedW(std::wstring_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnFindFirstFileNameTransactedW Not implemented");
		}

		void OnFindFirstFileTransactedA(std::string_view /*fileName*/) override final
		{
			throw std::runtime_error("OnFindFirstFileTransactedA Not implemented");
		}

		void OnFindFirstFileTransactedW(std::wstring_view /*fileName*/) override final
		{
			throw std::runtime_error("OnFindFirstFileTransactedW Not implemented");
		}

		void OnFindFirstStreamTransactedW(std::wstring_view /*fileName*/) override final
		{
			throw std::runtime_error("OnFindFirstStreamTransactedW Not implemented");
		}

		void OnGetBinaryTypeA(std::string_view /*applicationName*/, bool /*result*/) override final
		{
		}

		void OnGetBinaryTypeW(std::wstring_view /*applicationName*/, bool /*result*/) override final
		{
		}

		void OnGetCompressedFileSizeTransactedA(std::string_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetCompressedFileSizeTransactedA Not implemented");
		}

		void OnGetCompressedFileSizeTransactedW(std::wstring_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetCompressedFileSizeTransactedW Not implemented");
		}

		void OnGetDllDirectoryA(uint32_t /*result*/) override final
		{
		}

		void OnGetDllDirectoryW(uint32_t /*result*/) override final
		{
		}

		void OnGetEnvironmentVariableA(std::string_view /*name*/, uint32_t /*result*/) override final
		{
		}

		void OnGetEnvironmentVariableW(std::wstring_view /*name*/, uint32_t /*result*/) override final
		{
		}

		void OnGetFileAttributesTransactedA(
			std::string_view fileName,
			uint32_t result,
			bool wasBlocked) override final
		{
			auto exists = result != INVALID_FILE_ATTRIBUTES;
			TouchFileRead(fileName, exists, wasBlocked);
		}

		void OnGetFileAttributesTransactedW(
			std::wstring_view fileName,
			uint32_t result,
			bool wasBlocked) override final
		{
			auto exists = result != INVALID_FILE_ATTRIBUTES;
			TouchFileRead(fileName, exists, wasBlocked);
		}

		void OnGetFileBandwidthReservation(bool /*result*/) override final
		{
		}
		
		void OnGetFileInformationByHandleEx(bool /*result*/) override final
		{
		}

		void OnGetFileSecurityA(
			std::string_view fileName,
			bool result,
			bool wasBlocked) override final
		{
			TouchFileRead(fileName, result, wasBlocked);
		}

		void OnGetFullPathNameTransactedA(std::string_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetFullPathNameTransactedA Not implemented");
		}

		void OnGetFullPathNameTransactedW(std::wstring_view /*fileName*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnGetFullPathNameTransactedW Not implemented");
		}

		void OnGetLongPathNameTransactedA(std::string_view /*shortPath*/, std::string_view /*longPath*/, uint32_t /*result*/) override final
		{
		}

		void OnGetLongPathNameTransactedW(std::wstring_view /*shortPath*/, std::wstring_view /*longPath*/, uint32_t /*result*/) override final
		{
		}

		void OnGetQueuedCompletionStatus(bool /*result*/) override final
		{
		}

		void OnGetQueuedCompletionStatusEx(bool /*result*/) override final
		{
		}

		void OnGetShortPathNameA(std::string_view /*longPath*/, std::string_view /*shortPath*/, uint32_t /*result*/) override final
		{
		}

		void OnLoadModule(std::string_view /*moduleName*/, uint32_t /*result*/) override final
		{
		}

		void OnLoadPackagedLibrary(std::string_view /*libFileName*/) override final
		{
		}

		void OnMoveFileA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileExA(
			std::string_view existingFileName,
			std::string_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileExW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileTransactedA(
			std::string_view existingFileName,
			std::string_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileTransactedW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileWithProgressA(
			std::string_view existingFileName,
			std::string_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnMoveFileWithProgressW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint32_t /*flags*/,
			bool result,
			bool wasBlocked) override final
		{
			// TODO: May want to check the last error for ERROR_FILE_NOT_FOUND
			TouchFileRead(existingFileName, result, wasBlocked);
			TouchFileWrite(newFileName, wasBlocked);
		}

		void OnOpenEncryptedFileRawA(std::string_view /*fileName*/, uint32_t /*flags*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnOpenEncryptedFileRawA Not implemented");
		}

		void OnOpenEncryptedFileRawW(std::wstring_view /*fileName*/, uint32_t /*flags*/, uint32_t /*result*/) override final
		{
			throw std::runtime_error("OnOpenEncryptedFileRawW Not implemented");
		}

		void OnOpenFile(std::string_view /*fileName*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnOpenFile Not implemented");
		}

		void OnOpenFileById() override final
		{
			throw std::runtime_error("OnOpenFileById Not implemented");
		}

		void OnReadEncryptedFileRaw(uint32_t /*result*/) override final
		{
		}

		void OnRemoveDirectoryTransactedA(std::string_view /*pathName*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnRemoveDirectoryTransactedA Not implemented");
		}

		void OnRemoveDirectoryTransactedW(std::wstring_view /*pathName*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnRemoveDirectoryTransactedW Not implemented");
		}

		void OnReOpenFile() override final
		{
			throw std::runtime_error("OnReOpenFile Not implemented");
		}

		void OnReplaceFileA(
			std::string_view /*replacedFileName*/,
			std::string_view /*replacementFileName*/,
			std::string_view /*backupFileName*/,
			uint32_t /*replaceFlags*/,
			bool /*result*/) override final
		{
			throw std::runtime_error("OnReplaceFileA Not implemented");
		}

		void OnReplaceFileW(
			std::wstring_view /*replacedFileName*/,
			std::wstring_view /*replacementFileName*/,
			std::wstring_view /*backupFileName*/,
			uint32_t /*replaceFlags*/,
			bool /*result*/) override final
		{
			throw std::runtime_error("OnReplaceFileW Not implemented");
		}

		void OnSetCurrentDirectoryA(std::string_view /*pathName*/, bool /*result*/) override final
		{
			// Will need to update /*path*/s for files when changed
			throw std::runtime_error("OnSetCurrentDirectoryA Not implemented");
		}

		void OnSetCurrentDirectoryW(std::wstring_view /*pathName*/, bool /*result*/) override final
		{
			// Will need to update /*path*/s for files when changed
			throw std::runtime_error("OnSetCurrentDirectoryW Not implemented");
		}

		void OnSetDllDirectoryA(std::string_view /*pathName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnSetDllDirectoryA Not implemented");
		}

		void OnSetDllDirectoryW(std::wstring_view /*pathName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnSetDllDirectoryW Not implemented");
		}

		void OnSetFileAttributesTransactedA(std::string_view /*pathName*/, uint32_t /*fileAttributes*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnSetFileAttributesTransactedA Not implemented");
		}

		void OnSetFileAttributesTransactedW(std::wstring_view /*pathName*/, uint32_t /*fileAttributes*/, bool /*result*/, bool /*wasBlocked*/) override final
		{
			throw std::runtime_error("OnSetFileAttributesTransactedW Not implemented");
		}

		void OnSetFileBandwidthReservation(bool /*result*/) override final
		{
			throw std::runtime_error("OnSetFileBandwidthReservation Not implemented");
		}

		void OnSetFileCompletionNotificationModes(bool /*result*/) override final
		{
		}

		void OnSetFileSecurityA(std::string_view /*fileName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnSetFileSecurityA Not implemented");
		}

		void OnSetFileShortNameA(std::string_view /*shortName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnSetFileShortNameA Not implemented");
		}

		void OnSetFileShortNameW(std::wstring_view /*shortName*/, bool /*result*/) override final
		{
			throw std::runtime_error("OnSetFileShortNameW Not implemented");
		}

		void OnSetSearchPathMode(uint32_t /*flags*/, bool /*result*/) override final
		{
		}

		void OnWriteEncryptedFileRaw(uint32_t /*result*/) override final
		{
		}

	private:
		void OnCreateProcess(bool wasDetoured, std::wstring_view applicationName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			OnCreateProcess(wasDetoured, converter.to_bytes(applicationName.data()));
		}

		void OnCreateProcess(bool wasDetoured, std::string_view applicationName)
		{
			_callback->OnCreateProcess(applicationName, wasDetoured);
		}

		void TouchFileRead(std::wstring_view fileName, bool exists, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileRead(converter.to_bytes(fileName.data()), exists, wasBlocked);
		}

		void TouchFileRead(std::string_view fileName, bool exists, bool wasBlocked)
		{
			// Verify not a special file
			if (!IsSpecialFile(fileName))
			{
				_callback->TouchFileRead(Path(fileName), exists, wasBlocked);
			}
		}

		void TouchFileWrite(std::wstring_view fileName, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileWrite(converter.to_bytes(fileName.data()), wasBlocked);
		}

		void TouchFileWrite(std::string_view fileName, bool wasBlocked)
		{
			// Verify not a special file
			if (!IsSpecialFile(fileName))
			{
				_callback->TouchFileWrite(Path(fileName), wasBlocked);
			}
		}

		void TouchFileDelete(std::wstring_view fileName, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDelete(converter.to_bytes(fileName.data()), wasBlocked);
		}

		void TouchFileDelete(std::string_view fileName, bool wasBlocked)
		{
			_callback->TouchFileDelete(Path(fileName), wasBlocked);
		}

		void TouchFileDeleteOnClose(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDeleteOnClose(converter.to_bytes(fileName.data()));
		}

		void TouchFileDeleteOnClose(std::string_view fileName)
		{
			_callback->TouchFileDeleteOnClose(Path(fileName));
		}

		bool IsSpecialFile(std::string_view fileName)
		{
			// Check if the file name is a pipe or the standard input/output streams
			return fileName.starts_with("\\\\.\\") ||
				fileName == "CONIN$" ||
				fileName == "CONOUT$";
		}

		void SearchPath(std::wstring_view path, std::wstring_view filename)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			auto pathString = converter.to_bytes(path.data());
			auto filenameString = converter.to_bytes(filename.data());
			SearchPath(pathString, filenameString);
		}

		void SearchPath(std::string_view path, std::string_view filename)
		{
			_callback->SearchPath(path, filename);
		}
	};
}
