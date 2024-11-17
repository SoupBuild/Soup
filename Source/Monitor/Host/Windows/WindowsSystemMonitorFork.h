#pragma once 

namespace Monitor::Windows
{
	class WindowsSystemMonitorFork : public IWindowsSystemMonitor
	{
	private:
		std::shared_ptr<IWindowsSystemMonitor> _monitor1;
		std::shared_ptr<IWindowsSystemMonitor> _monitor2;

	public:
		WindowsSystemMonitorFork(
			std::shared_ptr<IWindowsSystemMonitor> monitor1,
			std::shared_ptr<IDetourCallback> monitor2) :
			_monitor1(std::move(monitor1)),
			_monitor2(std::move(monitor2))
		{
		}

		void OnInitialize() override final
		{
			_monitor1->OnInitialize();
			_monitor2->OnInitialize();
		}

		void OnShutdown(bool hadError) override final
		{
			_monitor1->OnShutdown(hadError);
			_monitor2->OnShutdown(hadError);
		}

		void OnError(std::string_view message) override final
		{
			_monitor1->OnError(message);
			_monitor2->OnError(message);
		}

		// FileApi
		void OnAreFileApisANSI(bool result) override final
		{
			_monitor1->OnAreFileApisANSI(result);
			_monitor2->OnAreFileApisANSI(result);
		}

		void OnCompareFileTime(int32_t result) override final
		{
			_monitor1->OnCompareFileTime(result);
			_monitor2->OnCompareFileTime(result);
		}

		void OnCreateDirectoryA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryA(pathName, result, wasBlocked);
			_monitor2->OnCreateDirectoryA(pathName, result, wasBlocked);
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryW(pathName, result, wasBlocked);
			_monitor2->OnCreateDirectoryW(pathName, result, wasBlocked);
		}

		void OnCreateFile2(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
			_monitor2->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
		}

		void OnCreateFileA(
			std::string_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint32_t flagsAndAttributes,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
			_monitor2->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
		}

		void OnCreateFileW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint32_t flagsAndAttributes,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
			_monitor2->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
		}

		void OnDefineDosDeviceW(uint32_t flags, std::wstring_view deviceName, std::wstring_view targetPath, bool result) override final
		{
			_monitor1->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
			_monitor2->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
		}

		void OnDeleteFileA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnDeleteFileA(fileName, result, wasBlocked);
			_monitor2->OnDeleteFileA(fileName, result, wasBlocked);
		}

		void OnDeleteFileW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnDeleteFileW(fileName, result, wasBlocked);
			_monitor2->OnDeleteFileW(fileName, result, wasBlocked);
		}

		void OnDeleteVolumeMountPointW(std::wstring_view volumeMountPoint, bool result) override final
		{
			_monitor1->OnDeleteVolumeMountPointW(volumeMountPoint, result);
			_monitor2->OnDeleteVolumeMountPointW(volumeMountPoint, result);
		}

		void OnFileTimeToLocalFileTime(bool result) override final
		{
			_monitor1->OnFileTimeToLocalFileTime(result);
			_monitor2->OnFileTimeToLocalFileTime(result);
		}

		void OnFindClose(bool result) override final
		{
			_monitor1->OnFindClose(result);
			_monitor2->OnFindClose(result);
		}

		void OnFindCloseChangeNotification(bool result) override final
		{
			_monitor1->OnFindCloseChangeNotification(result);
			_monitor2->OnFindCloseChangeNotification(result);
		}

		void OnFindFirstChangeNotificationA(std::string_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			_monitor1->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
			_monitor2->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
		}

		void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			_monitor1->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
			_monitor2->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
		}

		void OnFindFirstFileA(std::string_view fileName) override final
		{
			_monitor1->OnFindFirstFileA(fileName);
			_monitor2->OnFindFirstFileA(fileName);
		}

		void OnFindFirstFileW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstFileW(fileName);
			_monitor2->OnFindFirstFileW(fileName);
		}

		void OnFindFirstFileExA(std::string_view fileName) override final
		{
			_monitor1->OnFindFirstFileExA(fileName);
			_monitor2->OnFindFirstFileExA(fileName);
		}

		void OnFindFirstFileExW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstFileExW(fileName);
			_monitor2->OnFindFirstFileExW(fileName);
		}

		void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t flags) override final
		{
			_monitor1->OnFindFirstFileNameW(fileName, flags);
			_monitor2->OnFindFirstFileNameW(fileName, flags);
		}

		void OnFindFirstStreamW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstStreamW(fileName);
			_monitor2->OnFindFirstStreamW(fileName);
		}

		void OnFindFirstVolumeW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstVolumeW(fileName);
			_monitor2->OnFindFirstVolumeW(fileName);
		}

		void OnFindNextChangeNotification(bool result) override final
		{
			_monitor1->OnFindNextChangeNotification(result);
			_monitor2->OnFindNextChangeNotification(result);
		}

		void OnFindNextFileA(bool result) override final
		{
			_monitor1->OnFindNextFileA(result);
			_monitor2->OnFindNextFileA(result);
		}

		void OnFindNextFileW(bool result) override final
		{
			_monitor1->OnFindNextFileW(result);
			_monitor2->OnFindNextFileW(result);
		}

		void OnFindNextFileNameW(bool result) override final
		{
			_monitor1->OnFindNextFileNameW(result);
			_monitor2->OnFindNextFileNameW(result);
		}

		void OnFindNextStreamW(bool result) override final
		{
			_monitor1->OnFindNextStreamW(result);
			_monitor2->OnFindNextStreamW(result);
		}

		void OnFindNextVolumeW(bool result) override final
		{
			_monitor1->OnFindNextVolumeW(result);
			_monitor2->OnFindNextVolumeW(result);
		}

		void OnFindVolumeClose(bool result) override final
		{
			_monitor1->OnFindVolumeClose(result);
			_monitor2->OnFindVolumeClose(result);
		}

		void OnFlushFileBuffers(bool result) override final
		{
			_monitor1->OnFlushFileBuffers(result);
			_monitor2->OnFlushFileBuffers(result);
		}

		void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetCompressedFileSizeA(fileName, result);
			_monitor2->OnGetCompressedFileSizeA(fileName, result);
		}

		void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetCompressedFileSizeW(fileName, result);
			_monitor2->OnGetCompressedFileSizeW(fileName, result);
		}

		void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool result) override final
		{
			_monitor1->OnGetDiskFreeSpaceA(rootPathName, result);
			_monitor2->OnGetDiskFreeSpaceA(rootPathName, result);
		}

		void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool result) override final
		{
			_monitor1->OnGetDiskFreeSpaceW(rootPathName, result);
			_monitor2->OnGetDiskFreeSpaceW(rootPathName, result);
		}

		void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool result) override final
		{
			_monitor1->OnGetDiskFreeSpaceExA(directoryName, result);
			_monitor2->OnGetDiskFreeSpaceExA(directoryName, result);
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool result) override final
		{
			_monitor1->OnGetDiskFreeSpaceExW(directoryName, result);
			_monitor2->OnGetDiskFreeSpaceExW(directoryName, result);
		}

		void OnGetDriveTypeA(std::string_view rootPathName, uint32_t result) override final
		{
			_monitor1->OnGetDriveTypeA(rootPathName, result);
			_monitor2->OnGetDriveTypeA(rootPathName, result);
		}

		void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t result) override final
		{
			_monitor1->OnGetDriveTypeW(rootPathName, result);
			_monitor2->OnGetDriveTypeW(rootPathName, result);
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesA(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesW(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesW(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesExA(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesExA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesExW(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesExW(fileName, result, wasBlocked);
		}

		void OnGetFileInformationByHandle(bool result) override final
		{
			_monitor1->OnGetFileInformationByHandle(result);
			_monitor2->OnGetFileInformationByHandle(result);
		}

		void OnGetFileSize(uint32_t result) override final
		{
			_monitor1->OnGetFileSize(result);
			_monitor2->OnGetFileSize(result);
		}

		void OnGetFileSizeEx(uint32_t result) override final
		{
			_monitor1->OnGetFileSizeEx(result);
			_monitor2->OnGetFileSizeEx(result);
		}

		void OnGetFileTime(bool result) override final
		{
			_monitor1->OnGetFileTime(result);
			_monitor2->OnGetFileTime(result);
		}

		void OnGetFileType(uint32_t result) override final
		{
			_monitor1->OnGetFileType(result);
			_monitor2->OnGetFileType(result);
		}

		void OnGetFinalPathNameByHandleA(uint32_t result) override final
		{
			_monitor1->OnGetFinalPathNameByHandleA(result);
			_monitor2->OnGetFinalPathNameByHandleA(result);
		}

		void OnGetFinalPathNameByHandleW(uint32_t result) override final
		{
			_monitor1->OnGetFinalPathNameByHandleW(result);
			_monitor2->OnGetFinalPathNameByHandleW(result);
		}

		void OnGetFullPathNameA(std::string_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetFullPathNameA(fileName, result);
			_monitor2->OnGetFullPathNameA(fileName, result);
		}

		void OnGetFullPathNameW(std::wstring_view fileName, std::wstring_view buffer, uint32_t result) override final
		{
			_monitor1->OnGetFullPathNameW(fileName, buffer, result);
			_monitor2->OnGetFullPathNameW(fileName, buffer, result);
		}

		void OnGetLogicalDrives(uint32_t result) override final
		{
			_monitor1->OnGetLogicalDrives(result);
			_monitor2->OnGetLogicalDrives(result);
		}

		void OnGetLogicalDriveStringsW(std::wstring_view buffer, uint32_t result) override final
		{
			_monitor1->OnGetLogicalDriveStringsW(buffer, result);
			_monitor2->OnGetLogicalDriveStringsW(buffer, result);
		}

		void OnGetLongPathNameA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			_monitor1->OnGetLongPathNameA(longPath, shortPath, result);
			_monitor2->OnGetLongPathNameA(longPath, shortPath, result);
		}

		void OnGetLongPathNameW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			_monitor1->OnGetLongPathNameW(longPath, shortPath, result);
			_monitor2->OnGetLongPathNameW(longPath, shortPath, result);
		}

		void OnGetShortPathNameW(std::wstring_view longPath, std::wstring_view shortPath, uint32_t result) override final
		{
			_monitor1->OnGetShortPathNameW(longPath, shortPath, result);
			_monitor2->OnGetShortPathNameW(longPath, shortPath, result);
		}

		void OnGetTempFileNameA(std::string_view pathName, std::string_view prefixString, uint32_t unique, std::string_view tempFileName, uint32_t result) override final
		{
			_monitor1->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
			_monitor2->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
		}

		void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view prefixString, uint32_t unique, std::wstring_view tempFileName, uint32_t result) override final
		{
			_monitor1->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
			_monitor2->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
		}

		void OnGetTempPathA(std::string_view buffer, uint32_t result) override final
		{
			_monitor1->OnGetTempPathA(buffer, result);
			_monitor2->OnGetTempPathA(buffer, result);
		}

		void OnGetTempPathW(std::wstring_view buffer, uint32_t result) override final
		{
			_monitor1->OnGetTempPathW(buffer, result);
			_monitor2->OnGetTempPathW(buffer, result);
		}

		void OnGetVolumeInformationA(bool result) override final
		{
			_monitor1->OnGetVolumeInformationA(result);
			_monitor2->OnGetVolumeInformationA(result);
		}

		void OnGetVolumeInformationW(bool result) override final
		{
			_monitor1->OnGetVolumeInformationW(result);
			_monitor2->OnGetVolumeInformationW(result);
		}

		void OnGetVolumeInformationByHandleW(bool result) override final
		{
			_monitor1->OnGetVolumeInformationByHandleW(result);
			_monitor2->OnGetVolumeInformationByHandleW(result);
		}

		void OnGetVolumeNameForVolumeMountPointW(bool result) override final
		{
			_monitor1->OnGetVolumeNameForVolumeMountPointW(result);
			_monitor2->OnGetVolumeNameForVolumeMountPointW(result);
		}

		void OnGetVolumePathNamesForVolumeNameW(bool result) override final
		{
			_monitor1->OnGetVolumePathNamesForVolumeNameW(result);
			_monitor2->OnGetVolumePathNamesForVolumeNameW(result);
		}

		void OnGetVolumePathNameW(std::wstring_view filename, bool result) override final
		{
			_monitor1->OnGetVolumePathNameW(filename, result);
			_monitor2->OnGetVolumePathNameW(filename, result);
		}

		void OnLocalFileTimeToFileTime(bool result) override final
		{
			_monitor1->OnLocalFileTimeToFileTime(result);
			_monitor2->OnLocalFileTimeToFileTime(result);
		}

		void OnLockFile(bool result) override final
		{
			_monitor1->OnLockFile(result);
			_monitor2->OnLockFile(result);
		}

		void OnLockFileEx(bool result) override final
		{
			_monitor1->OnLockFileEx(result);
			_monitor2->OnLockFileEx(result);
		}

		void OnQueryDosDeviceW(std::wstring_view deviceName, uint32_t result) override final
		{
			_monitor1->OnQueryDosDeviceW(deviceName, result);
			_monitor2->OnQueryDosDeviceW(deviceName, result);
		}

		void OnReadFile(bool result) override final
		{
			_monitor1->OnReadFile(result);
			_monitor2->OnReadFile(result);
		}

		void OnReadFileEx(bool result) override final
		{
			_monitor1->OnReadFileEx(result);
			_monitor2->OnReadFileEx(result);
		}

		void OnReadFileScatter(bool result) override final
		{
			_monitor1->OnReadFileScatter(result);
			_monitor2->OnReadFileScatter(result);
		}

		void OnRemoveDirectoryA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnRemoveDirectoryA(pathName, result, wasBlocked);
			_monitor2->OnRemoveDirectoryA(pathName, result, wasBlocked);
		}

		void OnRemoveDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnRemoveDirectoryW(pathName, result, wasBlocked);
			_monitor2->OnRemoveDirectoryW(pathName, result, wasBlocked);
		}

		void OnSetEndOfFile(bool result) override final
		{
			_monitor1->OnSetEndOfFile(result);
			_monitor2->OnSetEndOfFile(result);
		}
		
		void OnSetFileApisToANSI() override final
		{
			_monitor1->OnSetFileApisToANSI();
			_monitor2->OnSetFileApisToANSI();
		}

		void OnSetFileApisToOEM() override final
		{
			_monitor1->OnSetFileApisToOEM();
			_monitor2->OnSetFileApisToOEM();
		}

		void OnSetFileAttributesA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnSetFileAttributesA(fileName, result, wasBlocked);
			_monitor2->OnSetFileAttributesA(fileName, result, wasBlocked);
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnSetFileAttributesW(fileName, result, wasBlocked);
			_monitor2->OnSetFileAttributesW(fileName, result, wasBlocked);
		}

		void OnSetFileInformationByHandle(bool result) override final
		{
			_monitor1->OnSetFileInformationByHandle(result);
			_monitor2->OnSetFileInformationByHandle(result);
		}

		void OnSetFileIoOverlappedRange(bool result) override final
		{
			_monitor1->OnSetFileIoOverlappedRange(result);
			_monitor2->OnSetFileIoOverlappedRange(result);
		}

		void OnSetFilePointer(uint32_t result) override final
		{
			_monitor1->OnSetFilePointer(result);
			_monitor2->OnSetFilePointer(result);
		}
		
		void OnSetFilePointerEx(bool result) override final
		{
			_monitor1->OnSetFilePointerEx(result);
			_monitor2->OnSetFilePointerEx(result);
		}

		void OnSetFileTime(bool result) override final
		{
			_monitor1->OnSetFileTime(result);
			_monitor2->OnSetFileTime(result);
		}

		void OnSetFileValidData(bool result) override final
		{
			_monitor1->OnSetFileValidData(result);
			_monitor2->OnSetFileValidData(result);
		}

		void OnUnlockFile(bool result) override final
		{
			_monitor1->OnUnlockFile(result);
			_monitor2->OnUnlockFile(result);
		}

		void OnUnlockFileEx(bool result) override final
		{
			_monitor1->OnUnlockFileEx(result);
			_monitor2->OnUnlockFileEx(result);
		}

		void OnWriteFile(bool result) override final
		{
			_monitor1->OnWriteFile(result);
			_monitor2->OnWriteFile(result);
		}

		void OnWriteFileEx(bool result) override final
		{
			_monitor1->OnWriteFileEx(result);
			_monitor2->OnWriteFileEx(result);
		}

		void OnWriteFileGather(bool result) override final
		{
			_monitor1->OnWriteFileGather(result);
			_monitor2->OnWriteFileGather(result);
		}

		// LibLoaderApi
		void OnLoadLibraryA(std::string_view libFileName) override final
		{
			_monitor1->OnLoadLibraryA(libFileName);
			_monitor2->OnLoadLibraryA(libFileName);
		}

		void OnLoadLibraryW(std::wstring_view libFileName) override final
		{
			_monitor1->OnLoadLibraryW(libFileName);
			_monitor2->OnLoadLibraryW(libFileName);
		}

		void OnLoadLibraryExA(std::string_view libFileName) override final
		{
			_monitor1->OnLoadLibraryExA(libFileName);
			_monitor2->OnLoadLibraryExA(libFileName);
		}

		void OnLoadLibraryExW(std::wstring_view libFileName) override final
		{
			_monitor1->OnLoadLibraryExW(libFileName);
			_monitor2->OnLoadLibraryExW(libFileName);
		}

		// ProcessEnv
		void OnSearchPathA(std::string_view path, std::string_view fileName, std::string_view extension, uint32_t result) override final
		{
			_monitor1->OnSearchPathA(path, fileName, extension, result);
			_monitor2->OnSearchPathA(path, fileName, extension, result);
		}

		void OnSearchPathW(std::wstring_view path, std::wstring_view fileName, std::wstring_view extension, uint32_t result) override final
		{
			_monitor1->OnSearchPathW(path, fileName, extension, result);
			_monitor2->OnSearchPathW(path, fileName, extension, result);
		}

		// ProcessThreadsApi
		void OnCreateProcessA(bool wasDetoured, std::string_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessA(wasDetoured, applicationName, result);
			_monitor2->OnCreateProcessA(wasDetoured, applicationName, result);
		}

		void OnCreateProcessW(bool wasDetoured, std::wstring_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessW(wasDetoured, applicationName, result);
			_monitor2->OnCreateProcessW(wasDetoured, applicationName, result);
		}

		void OnCreateProcessAsUserA(std::string_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessAsUserA(applicationName, result);
			_monitor2->OnCreateProcessAsUserA(applicationName, result);
		}

		void OnCreateProcessAsUserW(std::wstring_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessAsUserW(applicationName, result);
			_monitor2->OnCreateProcessAsUserW(applicationName, result);
		}

		void OnExitProcess(uint32_t exitCode) override final
		{
			_monitor1->OnExitProcess(exitCode);
			_monitor2->OnExitProcess(exitCode);
		}

		// UndocumentedApi
		void OnPrivCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnPrivCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
		}

		// WinBase
		void OnCopyFileA(std::string_view existingFileName, std::string_view newFileName, bool failIfExists, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
			_monitor2->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
		}

		void OnCopyFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool failIfExists, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
			_monitor2->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
		}

		void OnCopyFile2(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileTransactedA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCreateDirectoryExA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
			_monitor2->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryExW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
			_monitor2->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryTransactedA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
			_monitor2->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryTransactedW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) override final
		{
			_monitor1->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
			_monitor2->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateFileTransactedA(
			std::string_view fileName,
			uint32_t desiredAccess, 
			uint32_t shareMode,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
			_monitor2->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
		}

		void OnCreateFileTransactedW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t shareMode,
			uint64_t result,
			bool wasBlocked) override final
		{
			_monitor1->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
			_monitor2->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
		}

		void OnCreateHardLinkA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			_monitor1->OnCreateHardLinkA(fileName, existingFileName, result);
			_monitor2->OnCreateHardLinkA(fileName, existingFileName, result);
		}

		void OnCreateHardLinkW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			_monitor1->OnCreateHardLinkW(fileName, existingFileName, result);
			_monitor2->OnCreateHardLinkW(fileName, existingFileName, result);
		}

		void OnCreateHardLinkTransactedA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			_monitor1->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
			_monitor2->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
		}

		void OnCreateHardLinkTransactedW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			_monitor1->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
			_monitor2->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
		}

		void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessWithLogonW(applicationName, result);
			_monitor2->OnCreateProcessWithLogonW(applicationName, result);
		}

		void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool result) override final
		{
			_monitor1->OnCreateProcessWithTokenW(applicationName, result);
			_monitor2->OnCreateProcessWithTokenW(applicationName, result);
		}

		void OnCreateSymbolicLinkA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			_monitor1->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
			_monitor2->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			_monitor1->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
			_monitor2->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkTransactedA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			_monitor1->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
			_monitor2->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkTransactedW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			_monitor1->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
			_monitor2->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
		}

		void OnDecryptFileA(std::string_view fileName, bool result) override final
		{
			_monitor1->OnDecryptFileA(fileName, result);
			_monitor2->OnDecryptFileA(fileName, result);
		}
		
		void OnDecryptFileW(std::wstring_view fileName, bool result) override final
		{
			_monitor1->OnDecryptFileW(fileName, result);
			_monitor2->OnDecryptFileW(fileName, result);
		}

		void OnDeleteFileTransactedA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnDeleteFileTransactedA(fileName, result, wasBlocked);
			_monitor2->OnDeleteFileTransactedA(fileName, result, wasBlocked);
		}

		void OnDeleteFileTransactedW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnDeleteFileTransactedW(fileName, result, wasBlocked);
			_monitor2->OnDeleteFileTransactedW(fileName, result, wasBlocked);
		}

		void OnEncryptFileA(std::string_view fileName, bool result) override final
		{
			_monitor1->OnEncryptFileA(fileName, result);
			_monitor2->OnEncryptFileA(fileName, result);
		}

		void OnEncryptFileW(std::wstring_view fileName, bool result) override final
		{
			_monitor1->OnEncryptFileW(fileName, result);
			_monitor2->OnEncryptFileW(fileName, result);
		}

		void OnFileEncryptionStatusA(std::string_view fileName, bool result) override final
		{
			_monitor1->OnFileEncryptionStatusA(fileName, result);
			_monitor2->OnFileEncryptionStatusA(fileName, result);
		}

		void OnFileEncryptionStatusW(std::wstring_view fileName, bool result) override final
		{
			_monitor1->OnFileEncryptionStatusW(fileName, result);
			_monitor2->OnFileEncryptionStatusW(fileName, result);
		}

		void OnFindFirstFileNameTransactedW(std::wstring_view fileName, bool result) override final
		{
			_monitor1->OnFindFirstFileNameTransactedW(fileName, result);
			_monitor2->OnFindFirstFileNameTransactedW(fileName, result);
		}

		void OnFindFirstFileTransactedA(std::string_view fileName) override final
		{
			_monitor1->OnFindFirstFileTransactedA(fileName);
			_monitor2->OnFindFirstFileTransactedA(fileName);
		}

		void OnFindFirstFileTransactedW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstFileTransactedW(fileName);
			_monitor2->OnFindFirstFileTransactedW(fileName);
		}

		void OnFindFirstStreamTransactedW(std::wstring_view fileName) override final
		{
			_monitor1->OnFindFirstStreamTransactedW(fileName);
			_monitor2->OnFindFirstStreamTransactedW(fileName);
		}

		void OnGetBinaryTypeA(std::string_view applicationName, bool result) override final
		{
			_monitor1->OnGetBinaryTypeA(applicationName, result);
			_monitor2->OnGetBinaryTypeA(applicationName, result);
		}

		void OnGetBinaryTypeW(std::wstring_view applicationName, bool result) override final
		{
			_monitor1->OnGetBinaryTypeW(applicationName, result);
			_monitor2->OnGetBinaryTypeW(applicationName, result);
		}

		void OnGetCompressedFileSizeTransactedA(std::string_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetCompressedFileSizeTransactedA(fileName, result);
			_monitor2->OnGetCompressedFileSizeTransactedA(fileName, result);
		}

		void OnGetCompressedFileSizeTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetCompressedFileSizeTransactedW(fileName, result);
			_monitor2->OnGetCompressedFileSizeTransactedW(fileName, result);
		}

		void OnGetDllDirectoryA(uint32_t result) override final
		{
			_monitor1->OnGetDllDirectoryA(result);
			_monitor2->OnGetDllDirectoryA(result);
		}

		void OnGetDllDirectoryW(uint32_t result) override final
		{
			_monitor1->OnGetDllDirectoryW(result);
			_monitor2->OnGetDllDirectoryW(result);
		}

		void OnGetEnvironmentVariableA(std::string_view name, uint32_t result) override final
		{
			_monitor1->OnGetEnvironmentVariableA(name, result);
			_monitor2->OnGetEnvironmentVariableA(name, result);
		}

		void OnGetEnvironmentVariableW(std::wstring_view name, uint32_t result) override final
		{
			_monitor1->OnGetEnvironmentVariableW(name, result);
			_monitor2->OnGetEnvironmentVariableW(name, result);
		}

		void OnGetFileAttributesTransactedA(std::string_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesTransactedW(std::wstring_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
			_monitor2->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
		}

		void OnGetFileBandwidthReservation(bool result) override final
		{
			_monitor1->OnGetFileBandwidthReservation(result);
			_monitor2->OnGetFileBandwidthReservation(result);
		}
		
		void OnGetFileInformationByHandleEx(bool result) override final
		{
			_monitor1->OnGetFileInformationByHandleEx(result);
			_monitor2->OnGetFileInformationByHandleEx(result);
		}

		void OnGetFileSecurityA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnGetFileSecurityA(fileName, result, wasBlocked);
			_monitor2->OnGetFileSecurityA(fileName, result, wasBlocked);
		}

		void OnGetFullPathNameTransactedA(std::string_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetFullPathNameTransactedA(fileName, result);
			_monitor2->OnGetFullPathNameTransactedA(fileName, result);
		}

		void OnGetFullPathNameTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			_monitor1->OnGetFullPathNameTransactedW(fileName, result);
			_monitor2->OnGetFullPathNameTransactedW(fileName, result);
		}

		void OnGetLongPathNameTransactedA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			_monitor1->OnGetLongPathNameTransactedA(shortPath, longPath, result);
			_monitor2->OnGetLongPathNameTransactedA(shortPath, longPath, result);
		}

		void OnGetLongPathNameTransactedW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			_monitor1->OnGetLongPathNameTransactedW(shortPath, longPath, result);
			_monitor2->OnGetLongPathNameTransactedW(shortPath, longPath, result);
		}

		void OnGetQueuedCompletionStatus(bool result) override final
		{
			_monitor1->OnGetQueuedCompletionStatus(result);
			_monitor2->OnGetQueuedCompletionStatus(result);
		}

		void OnGetQueuedCompletionStatusEx(bool result) override final
		{
			_monitor1->OnGetQueuedCompletionStatusEx(result);
			_monitor2->OnGetQueuedCompletionStatusEx(result);
		}

		void OnGetShortPathNameA(std::string_view longPath, std::string_view shortPath, uint32_t result) override final
		{
			_monitor1->OnGetShortPathNameA(longPath, shortPath, result);
			_monitor2->OnGetShortPathNameA(longPath, shortPath, result);
		}

		void OnLoadModule(std::string_view moduleName, uint32_t result) override final
		{
			_monitor1->OnLoadModule(moduleName, result);
			_monitor2->OnLoadModule(moduleName, result);
		}

		void OnLoadPackagedLibrary(std::string_view libFileName) override final
		{
			_monitor1->OnLoadPackagedLibrary(libFileName);
			_monitor2->OnLoadPackagedLibrary(libFileName);
		}

		void OnMoveFileA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnMoveFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
			_monitor2->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnMoveFileExA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileTransactedA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileWithProgressA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileWithProgressW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_monitor1->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
			_monitor2->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnOpenEncryptedFileRawA(std::string_view fileName, uint32_t flags, uint32_t result) override final
		{
			_monitor1->OnOpenEncryptedFileRawA(fileName, flags, result);
			_monitor2->OnOpenEncryptedFileRawA(fileName, flags, result);
		}

		void OnOpenEncryptedFileRawW(std::wstring_view fileName, uint32_t flags, uint32_t result) override final
		{
			_monitor1->OnOpenEncryptedFileRawW(fileName, flags, result);
			_monitor2->OnOpenEncryptedFileRawW(fileName, flags, result);
		}

		void OnOpenFile(std::string_view fileName, bool wasBlocked) override final
		{
			_monitor1->OnOpenFile(fileName, wasBlocked);
			_monitor2->OnOpenFile(fileName, wasBlocked);
		}

		void OnOpenFileById() override final
		{
			_monitor1->OnOpenFileById();
			_monitor2->OnOpenFileById();
		}

		void OnReadEncryptedFileRaw(uint32_t result) override final
		{
			_monitor1->OnReadEncryptedFileRaw(result);
			_monitor2->OnReadEncryptedFileRaw(result);
		}

		void OnRemoveDirectoryTransactedA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
			_monitor2->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
		}

		void OnRemoveDirectoryTransactedW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_monitor1->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
			_monitor2->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
		}

		void OnReOpenFile() override final
		{
			_monitor1->OnReOpenFile();
			_monitor2->OnReOpenFile();
		}

		void OnReplaceFileA(std::string_view replacedFileName, std::string_view replacementFileName, std::string_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			_monitor1->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
			_monitor2->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
		}

		void OnReplaceFileW(std::wstring_view replacedFileName, std::wstring_view replacementFileName, std::wstring_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			_monitor1->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
			_monitor2->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
		}

		void OnSetCurrentDirectoryA(std::string_view pathName, bool result) override final
		{
			_monitor1->OnSetCurrentDirectoryA(pathName, result);
			_monitor2->OnSetCurrentDirectoryA(pathName, result);
		}

		void OnSetCurrentDirectoryW(std::wstring_view pathName, bool result) override final
		{
			_monitor1->OnSetCurrentDirectoryW(pathName, result);
			_monitor2->OnSetCurrentDirectoryW(pathName, result);
		}

		void OnSetDllDirectoryA(std::string_view pathName, bool result) override final
		{
			_monitor1->OnSetDllDirectoryA(pathName, result);
			_monitor2->OnSetDllDirectoryA(pathName, result);
		}

		void OnSetDllDirectoryW(std::wstring_view pathName, bool result) override final
		{
			_monitor1->OnSetDllDirectoryW(pathName, result);
			_monitor2->OnSetDllDirectoryW(pathName, result);
		}

		void OnSetFileAttributesTransactedA(std::string_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) override final
		{
			_monitor1->OnSetFileAttributesTransactedA(pathName, fileAttributes, result, wasBlocked);
			_monitor2->OnSetFileAttributesTransactedA(pathName, fileAttributes, result, wasBlocked);
		}

		void OnSetFileAttributesTransactedW(std::wstring_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) override final
		{
			_monitor1->OnSetFileAttributesTransactedW(pathName, fileAttributes, result, wasBlocked);
			_monitor2->OnSetFileAttributesTransactedW(pathName, fileAttributes, result, wasBlocked);
		}

		void OnSetFileBandwidthReservation(bool result) override final
		{
			_monitor1->OnSetFileBandwidthReservation(result);
			_monitor2->OnSetFileBandwidthReservation(result);
		}

		void OnSetFileCompletionNotificationModes(bool result) override final
		{
			_monitor1->OnSetFileCompletionNotificationModes(result);
			_monitor2->OnSetFileCompletionNotificationModes(result);
		}

		void OnSetFileSecurityA(std::string_view fileName, bool result) override final
		{
			_monitor1->OnSetFileSecurityA(fileName, result);
			_monitor2->OnSetFileSecurityA(fileName, result);
		}

		void OnSetFileShortNameA(std::string_view shortName, bool result) override final
		{
			_monitor1->OnSetFileShortNameA(shortName, result);
			_monitor2->OnSetFileShortNameA(shortName, result);
		}

		void OnSetFileShortNameW(std::wstring_view shortName, bool result) override final
		{
			_monitor1->OnSetFileShortNameW(shortName, result);
			_monitor2->OnSetFileShortNameW(shortName, result);
		}

		void OnSetSearchPathMode(uint32_t flags, bool result) override final
		{
			_monitor1->OnSetSearchPathMode(flags, result);
			_monitor2->OnSetSearchPathMode(flags, result);
		}

		void OnWriteEncryptedFileRaw(uint32_t result) override final
		{
			_monitor1->OnWriteEncryptedFileRaw(result);
			_monitor2->OnWriteEncryptedFileRaw(result);
		}
	};
}
