#pragma once 

namespace Monitor
{
	export class ForkCallbackLogger : public IDetourCallback
	{
	public:
		ForkCallbackLogger(std::shared_ptr<IDetourCallback> callback1, std::shared_ptr<IDetourCallback> callback2) :
			_callback1(std::move(callback1)),
			_callback2(std::move(callback2))
		{
		}

		void OnInitialize() override final
		{
			_callback1->OnInitialize();
			_callback2->OnInitialize();
		}

		void OnShutdown() override final
		{
			_callback1->OnShutdown();
			_callback2->OnShutdown();
		}

		void OnError(std::string_view message) override final
		{
			_callback1->OnError(message);
			_callback2->OnError(message);
		}

		// FileApi
		void OnAreFileApisANSI(bool result) override final
		{
			_callback1->OnAreFileApisANSI(result);
			_callback2->OnAreFileApisANSI(result);
		}

		void OnCompareFileTime(int32_t result) override final
		{
			_callback1->OnCompareFileTime(result);
			_callback2->OnCompareFileTime(result);
		}

		void OnCreateDirectoryA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryA(pathName, result, wasBlocked);
			_callback2->OnCreateDirectoryA(pathName, result, wasBlocked);
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryW(pathName, result, wasBlocked);
			_callback2->OnCreateDirectoryW(pathName, result, wasBlocked);
		}

		void OnCreateFile2(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t sharedMode,
			uint32_t creationDisposition,
			uint64_t result,
			bool wasBlocked) override final
		{
			_callback1->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
			_callback2->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
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
			_callback1->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
			_callback2->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
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
			_callback1->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
			_callback2->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
		}

		void OnDefineDosDeviceW(uint32_t flags, std::wstring_view deviceName, std::wstring_view targetPath, bool result) override final
		{
			_callback1->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
			_callback2->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
		}

		void OnDeleteFileA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnDeleteFileA(fileName, result, wasBlocked);
			_callback2->OnDeleteFileA(fileName, result, wasBlocked);
		}

		void OnDeleteFileW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnDeleteFileW(fileName, result, wasBlocked);
			_callback2->OnDeleteFileW(fileName, result, wasBlocked);
		}

		void OnDeleteVolumeMountPointW(std::wstring_view volumeMountPoint, bool result) override final
		{
			_callback1->OnDeleteVolumeMountPointW(volumeMountPoint, result);
			_callback2->OnDeleteVolumeMountPointW(volumeMountPoint, result);
		}

		void OnFileTimeToLocalFileTime(bool result) override final
		{
			_callback1->OnFileTimeToLocalFileTime(result);
			_callback2->OnFileTimeToLocalFileTime(result);
		}

		void OnFindClose(bool result) override final
		{
			_callback1->OnFindClose(result);
			_callback2->OnFindClose(result);
		}

		void OnFindCloseChangeNotification(bool result) override final
		{
			_callback1->OnFindCloseChangeNotification(result);
			_callback2->OnFindCloseChangeNotification(result);
		}

		void OnFindFirstChangeNotificationA(std::string_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			_callback1->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
			_callback2->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
		}

		void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			_callback1->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
			_callback2->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
		}

		void OnFindFirstFileA(std::string_view fileName) override final
		{
			_callback1->OnFindFirstFileA(fileName);
			_callback2->OnFindFirstFileA(fileName);
		}

		void OnFindFirstFileW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstFileW(fileName);
			_callback2->OnFindFirstFileW(fileName);
		}

		void OnFindFirstFileExA(std::string_view fileName) override final
		{
			_callback1->OnFindFirstFileExA(fileName);
			_callback2->OnFindFirstFileExA(fileName);
		}

		void OnFindFirstFileExW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstFileExW(fileName);
			_callback2->OnFindFirstFileExW(fileName);
		}

		void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t flags) override final
		{
			_callback1->OnFindFirstFileNameW(fileName, flags);
			_callback2->OnFindFirstFileNameW(fileName, flags);
		}

		void OnFindFirstStreamW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstStreamW(fileName);
			_callback2->OnFindFirstStreamW(fileName);
		}

		void OnFindFirstVolumeW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstVolumeW(fileName);
			_callback2->OnFindFirstVolumeW(fileName);
		}

		void OnFindNextChangeNotification(bool result) override final
		{
			_callback1->OnFindNextChangeNotification(result);
			_callback2->OnFindNextChangeNotification(result);
		}

		void OnFindNextFileA(bool result) override final
		{
			_callback1->OnFindNextFileA(result);
			_callback2->OnFindNextFileA(result);
		}

		void OnFindNextFileW(bool result) override final
		{
			_callback1->OnFindNextFileW(result);
			_callback2->OnFindNextFileW(result);
		}

		void OnFindNextFileNameW(bool result) override final
		{
			_callback1->OnFindNextFileNameW(result);
			_callback2->OnFindNextFileNameW(result);
		}

		void OnFindNextStreamW(bool result) override final
		{
			_callback1->OnFindNextStreamW(result);
			_callback2->OnFindNextStreamW(result);
		}

		void OnFindNextVolumeW(bool result) override final
		{
			_callback1->OnFindNextVolumeW(result);
			_callback2->OnFindNextVolumeW(result);
		}

		void OnFindVolumeClose(bool result) override final
		{
			_callback1->OnFindVolumeClose(result);
			_callback2->OnFindVolumeClose(result);
		}

		void OnFlushFileBuffers(bool result) override final
		{
			_callback1->OnFlushFileBuffers(result);
			_callback2->OnFlushFileBuffers(result);
		}

		void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t result) override final
		{
			_callback1->OnGetCompressedFileSizeA(fileName, result);
			_callback2->OnGetCompressedFileSizeA(fileName, result);
		}

		void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t result) override final
		{
			_callback1->OnGetCompressedFileSizeW(fileName, result);
			_callback2->OnGetCompressedFileSizeW(fileName, result);
		}

		void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool result) override final
		{
			_callback1->OnGetDiskFreeSpaceA(rootPathName, result);
			_callback2->OnGetDiskFreeSpaceA(rootPathName, result);
		}

		void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool result) override final
		{
			_callback1->OnGetDiskFreeSpaceW(rootPathName, result);
			_callback2->OnGetDiskFreeSpaceW(rootPathName, result);
		}

		void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool result) override final
		{
			_callback1->OnGetDiskFreeSpaceExA(directoryName, result);
			_callback2->OnGetDiskFreeSpaceExA(directoryName, result);
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool result) override final
		{
			_callback1->OnGetDiskFreeSpaceExW(directoryName, result);
			_callback2->OnGetDiskFreeSpaceExW(directoryName, result);
		}

		void OnGetDriveTypeA(std::string_view rootPathName, uint32_t result) override final
		{
			_callback1->OnGetDriveTypeA(rootPathName, result);
			_callback2->OnGetDriveTypeA(rootPathName, result);
		}

		void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t result) override final
		{
			_callback1->OnGetDriveTypeW(rootPathName, result);
			_callback2->OnGetDriveTypeW(rootPathName, result);
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesA(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesW(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesW(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesExA(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesExA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesExW(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesExW(fileName, result, wasBlocked);
		}

		void OnGetFileInformationByHandle(bool result) override final
		{
			_callback1->OnGetFileInformationByHandle(result);
			_callback2->OnGetFileInformationByHandle(result);
		}

		void OnGetFileSize(uint32_t result) override final
		{
			_callback1->OnGetFileSize(result);
			_callback2->OnGetFileSize(result);
		}

		void OnGetFileSizeEx(uint32_t result) override final
		{
			_callback1->OnGetFileSizeEx(result);
			_callback2->OnGetFileSizeEx(result);
		}

		void OnGetFileTime(bool result) override final
		{
			_callback1->OnGetFileTime(result);
			_callback2->OnGetFileTime(result);
		}

		void OnGetFileType(uint32_t result) override final
		{
			_callback1->OnGetFileType(result);
			_callback2->OnGetFileType(result);
		}

		void OnGetFinalPathNameByHandleA(uint32_t result) override final
		{
			_callback1->OnGetFinalPathNameByHandleA(result);
			_callback2->OnGetFinalPathNameByHandleA(result);
		}

		void OnGetFinalPathNameByHandleW(uint32_t result) override final
		{
			_callback1->OnGetFinalPathNameByHandleW(result);
			_callback2->OnGetFinalPathNameByHandleW(result);
		}

		void OnGetFullPathNameA(std::string_view fileName, uint32_t result) override final
		{
			_callback1->OnGetFullPathNameA(fileName, result);
			_callback2->OnGetFullPathNameA(fileName, result);
		}

		void OnGetFullPathNameW(std::wstring_view fileName, std::wstring_view buffer, uint32_t result) override final
		{
			_callback1->OnGetFullPathNameW(fileName, buffer, result);
			_callback2->OnGetFullPathNameW(fileName, buffer, result);
		}

		void OnGetLogicalDrives(uint32_t result) override final
		{
			_callback1->OnGetLogicalDrives(result);
			_callback2->OnGetLogicalDrives(result);
		}

		void OnGetLogicalDriveStringsW(std::wstring_view buffer, uint32_t result) override final
		{
			_callback1->OnGetLogicalDriveStringsW(buffer, result);
			_callback2->OnGetLogicalDriveStringsW(buffer, result);
		}

		void OnGetLongPathNameA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			_callback1->OnGetLongPathNameA(longPath, shortPath, result);
			_callback2->OnGetLongPathNameA(longPath, shortPath, result);
		}

		void OnGetLongPathNameW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			_callback1->OnGetLongPathNameW(longPath, shortPath, result);
			_callback2->OnGetLongPathNameW(longPath, shortPath, result);
		}

		void OnGetShortPathNameW(std::wstring_view longPath, std::wstring_view shortPath, uint32_t result) override final
		{
			_callback1->OnGetShortPathNameW(longPath, shortPath, result);
			_callback2->OnGetShortPathNameW(longPath, shortPath, result);
		}

		void OnGetTempFileNameA(std::string_view pathName, std::string_view prefixString, uint32_t unique, std::string_view tempFileName, uint32_t result) override final
		{
			_callback1->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
			_callback2->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
		}

		void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view prefixString, uint32_t unique, std::wstring_view tempFileName, uint32_t result) override final
		{
			_callback1->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
			_callback2->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
		}

		void OnGetTempPathA(std::string_view buffer, uint32_t result) override final
		{
			_callback1->OnGetTempPathA(buffer, result);
			_callback2->OnGetTempPathA(buffer, result);
		}

		void OnGetTempPathW(std::wstring_view buffer, uint32_t result) override final
		{
			_callback1->OnGetTempPathW(buffer, result);
			_callback2->OnGetTempPathW(buffer, result);
		}

		void OnGetVolumeInformationA(bool result) override final
		{
			_callback1->OnGetVolumeInformationA(result);
			_callback2->OnGetVolumeInformationA(result);
		}

		void OnGetVolumeInformationW(bool result) override final
		{
			_callback1->OnGetVolumeInformationW(result);
			_callback2->OnGetVolumeInformationW(result);
		}

		void OnGetVolumeInformationByHandleW(bool result) override final
		{
			_callback1->OnGetVolumeInformationByHandleW(result);
			_callback2->OnGetVolumeInformationByHandleW(result);
		}

		void OnGetVolumeNameForVolumeMountPointW(bool result) override final
		{
			_callback1->OnGetVolumeNameForVolumeMountPointW(result);
			_callback2->OnGetVolumeNameForVolumeMountPointW(result);
		}

		void OnGetVolumePathNamesForVolumeNameW(bool result) override final
		{
			_callback1->OnGetVolumePathNamesForVolumeNameW(result);
			_callback2->OnGetVolumePathNamesForVolumeNameW(result);
		}

		void OnGetVolumePathNameW(std::wstring_view filename, bool result) override final
		{
			_callback1->OnGetVolumePathNameW(filename, result);
			_callback2->OnGetVolumePathNameW(filename, result);
		}

		void OnLocalFileTimeToFileTime(bool result) override final
		{
			_callback1->OnLocalFileTimeToFileTime(result);
			_callback2->OnLocalFileTimeToFileTime(result);
		}

		void OnLockFile(bool result) override final
		{
			_callback1->OnLockFile(result);
			_callback2->OnLockFile(result);
		}

		void OnLockFileEx(bool result) override final
		{
			_callback1->OnLockFileEx(result);
			_callback2->OnLockFileEx(result);
		}

		void OnQueryDosDeviceW(std::wstring_view deviceName, uint32_t result) override final
		{
			_callback1->OnQueryDosDeviceW(deviceName, result);
			_callback2->OnQueryDosDeviceW(deviceName, result);
		}

		void OnReadFile(bool result) override final
		{
			_callback1->OnReadFile(result);
			_callback2->OnReadFile(result);
		}

		void OnReadFileEx(bool result) override final
		{
			_callback1->OnReadFileEx(result);
			_callback2->OnReadFileEx(result);
		}

		void OnReadFileScatter(bool result) override final
		{
			_callback1->OnReadFileScatter(result);
			_callback2->OnReadFileScatter(result);
		}

		void OnRemoveDirectoryA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnRemoveDirectoryA(pathName, result, wasBlocked);
			_callback2->OnRemoveDirectoryA(pathName, result, wasBlocked);
		}

		void OnRemoveDirectoryW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnRemoveDirectoryW(pathName, result, wasBlocked);
			_callback2->OnRemoveDirectoryW(pathName, result, wasBlocked);
		}

		void OnSetEndOfFile(bool result) override final
		{
			_callback1->OnSetEndOfFile(result);
			_callback2->OnSetEndOfFile(result);
		}
		
		void OnSetFileApisToANSI() override final
		{
			_callback1->OnSetFileApisToANSI();
			_callback2->OnSetFileApisToANSI();
		}

		void OnSetFileApisToOEM() override final
		{
			_callback1->OnSetFileApisToOEM();
			_callback2->OnSetFileApisToOEM();
		}

		void OnSetFileAttributesA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnSetFileAttributesA(fileName, result, wasBlocked);
			_callback2->OnSetFileAttributesA(fileName, result, wasBlocked);
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnSetFileAttributesW(fileName, result, wasBlocked);
			_callback2->OnSetFileAttributesW(fileName, result, wasBlocked);
		}

		void OnSetFileInformationByHandle(bool result) override final
		{
			_callback1->OnSetFileInformationByHandle(result);
			_callback2->OnSetFileInformationByHandle(result);
		}

		void OnSetFileIoOverlappedRange(bool result) override final
		{
			_callback1->OnSetFileIoOverlappedRange(result);
			_callback2->OnSetFileIoOverlappedRange(result);
		}

		void OnSetFilePointer(uint32_t result) override final
		{
			_callback1->OnSetFilePointer(result);
			_callback2->OnSetFilePointer(result);
		}
		
		void OnSetFilePointerEx(bool result) override final
		{
			_callback1->OnSetFilePointerEx(result);
			_callback2->OnSetFilePointerEx(result);
		}

		void OnSetFileTime(bool result) override final
		{
			_callback1->OnSetFileTime(result);
			_callback2->OnSetFileTime(result);
		}

		void OnSetFileValidData(bool result) override final
		{
			_callback1->OnSetFileValidData(result);
			_callback2->OnSetFileValidData(result);
		}

		void OnUnlockFile(bool result) override final
		{
			_callback1->OnUnlockFile(result);
			_callback2->OnUnlockFile(result);
		}

		void OnUnlockFileEx(bool result) override final
		{
			_callback1->OnUnlockFileEx(result);
			_callback2->OnUnlockFileEx(result);
		}

		void OnWriteFile(bool result) override final
		{
			_callback1->OnWriteFile(result);
			_callback2->OnWriteFile(result);
		}

		void OnWriteFileEx(bool result) override final
		{
			_callback1->OnWriteFileEx(result);
			_callback2->OnWriteFileEx(result);
		}

		void OnWriteFileGather(bool result) override final
		{
			_callback1->OnWriteFileGather(result);
			_callback2->OnWriteFileGather(result);
		}

		// LibLoaderApi
		void OnLoadLibraryA(std::string_view libFileName) override final
		{
			_callback1->OnLoadLibraryA(libFileName);
			_callback2->OnLoadLibraryA(libFileName);
		}

		void OnLoadLibraryW(std::wstring_view libFileName) override final
		{
			_callback1->OnLoadLibraryW(libFileName);
			_callback2->OnLoadLibraryW(libFileName);
		}

		void OnLoadLibraryExA(std::string_view libFileName) override final
		{
			_callback1->OnLoadLibraryExA(libFileName);
			_callback2->OnLoadLibraryExA(libFileName);
		}

		void OnLoadLibraryExW(std::wstring_view libFileName) override final
		{
			_callback1->OnLoadLibraryExW(libFileName);
			_callback2->OnLoadLibraryExW(libFileName);
		}

		// ProcessEnv
		void OnSearchPathA(std::string_view path, std::string_view fileName, std::string_view extension, uint32_t result) override final
		{
			_callback1->OnSearchPathA(path, fileName, extension, result);
			_callback2->OnSearchPathA(path, fileName, extension, result);
		}

		void OnSearchPathW(std::wstring_view path, std::wstring_view fileName, std::wstring_view extension, uint32_t result) override final
		{
			_callback1->OnSearchPathW(path, fileName, extension, result);
			_callback2->OnSearchPathW(path, fileName, extension, result);
		}

		// ProcessThreadsApi
		void OnCreateProcessA(bool wasDetoured, std::string_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessA(wasDetoured, applicationName, result);
			_callback2->OnCreateProcessA(wasDetoured, applicationName, result);
		}

		void OnCreateProcessW(bool wasDetoured, std::wstring_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessW(wasDetoured, applicationName, result);
			_callback2->OnCreateProcessW(wasDetoured, applicationName, result);
		}

		void OnCreateProcessAsUserA(std::string_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessAsUserA(applicationName, result);
			_callback2->OnCreateProcessAsUserA(applicationName, result);
		}

		void OnCreateProcessAsUserW(std::wstring_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessAsUserW(applicationName, result);
			_callback2->OnCreateProcessAsUserW(applicationName, result);
		}

		void OnExitProcess(uint32_t exitCode) override final
		{
			_callback1->OnExitProcess(exitCode);
			_callback2->OnExitProcess(exitCode);
		}

		// UndocumentedApi
		void OnPrivCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnPrivCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
		}

		// WinBase
		void OnCopyFileA(std::string_view existingFileName, std::string_view newFileName, bool failIfExists, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
			_callback2->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
		}

		void OnCopyFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool failIfExists, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
			_callback2->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
		}

		void OnCopyFile2(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint64_t result,
			bool wasBlocked) override final
		{
			_callback1->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileTransactedA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCopyFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnCreateDirectoryExA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
			_callback2->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryExW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
			_callback2->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryTransactedA(std::string_view templateDirectory, std::string_view newDirectory, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
			_callback2->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateDirectoryTransactedW(std::wstring_view templateDirectory, std::wstring_view newDirectory, bool result, bool wasBlocked) override final
		{
			_callback1->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
			_callback2->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
		}

		void OnCreateFileTransactedA(
			std::string_view fileName,
			uint32_t desiredAccess, 
			uint32_t shareMode,
			uint64_t result,
			bool wasBlocked) override final
		{
			_callback1->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
			_callback2->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
		}

		void OnCreateFileTransactedW(
			std::wstring_view fileName,
			uint32_t desiredAccess,
			uint32_t shareMode,
			uint64_t result,
			bool wasBlocked) override final
		{
			_callback1->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
			_callback2->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
		}

		void OnCreateHardLinkA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			_callback1->OnCreateHardLinkA(fileName, existingFileName, result);
			_callback2->OnCreateHardLinkA(fileName, existingFileName, result);
		}

		void OnCreateHardLinkW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			_callback1->OnCreateHardLinkW(fileName, existingFileName, result);
			_callback2->OnCreateHardLinkW(fileName, existingFileName, result);
		}

		void OnCreateHardLinkTransactedA(std::string_view fileName, std::string_view existingFileName, bool result) override final
		{
			_callback1->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
			_callback2->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
		}

		void OnCreateHardLinkTransactedW(std::wstring_view fileName, std::wstring_view existingFileName, bool result) override final
		{
			_callback1->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
			_callback2->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
		}

		void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessWithLogonW(applicationName, result);
			_callback2->OnCreateProcessWithLogonW(applicationName, result);
		}

		void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool result) override final
		{
			_callback1->OnCreateProcessWithTokenW(applicationName, result);
			_callback2->OnCreateProcessWithTokenW(applicationName, result);
		}

		void OnCreateSymbolicLinkA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			_callback1->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
			_callback2->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			_callback1->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
			_callback2->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkTransactedA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t flags, bool result) override final
		{
			_callback1->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
			_callback2->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
		}

		void OnCreateSymbolicLinkTransactedW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t flags, bool result) override final
		{
			_callback1->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
			_callback2->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
		}

		void OnDecryptFileA(std::string_view fileName, bool result) override final
		{
			_callback1->OnDecryptFileA(fileName, result);
			_callback2->OnDecryptFileA(fileName, result);
		}
		
		void OnDecryptFileW(std::wstring_view fileName, bool result) override final
		{
			_callback1->OnDecryptFileW(fileName, result);
			_callback2->OnDecryptFileW(fileName, result);
		}

		void OnDeleteFileTransactedA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnDeleteFileTransactedA(fileName, result, wasBlocked);
			_callback2->OnDeleteFileTransactedA(fileName, result, wasBlocked);
		}

		void OnDeleteFileTransactedW(std::wstring_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnDeleteFileTransactedW(fileName, result, wasBlocked);
			_callback2->OnDeleteFileTransactedW(fileName, result, wasBlocked);
		}

		void OnEncryptFileA(std::string_view fileName, bool result) override final
		{
			_callback1->OnEncryptFileA(fileName, result);
			_callback2->OnEncryptFileA(fileName, result);
		}

		void OnEncryptFileW(std::wstring_view fileName, bool result) override final
		{
			_callback1->OnEncryptFileW(fileName, result);
			_callback2->OnEncryptFileW(fileName, result);
		}

		void OnFileEncryptionStatusA(std::string_view fileName, bool result) override final
		{
			_callback1->OnFileEncryptionStatusA(fileName, result);
			_callback2->OnFileEncryptionStatusA(fileName, result);
		}

		void OnFileEncryptionStatusW(std::wstring_view fileName, bool result) override final
		{
			_callback1->OnFileEncryptionStatusW(fileName, result);
			_callback2->OnFileEncryptionStatusW(fileName, result);
		}

		void OnFindFirstFileNameTransactedW(std::wstring_view fileName, bool result) override final
		{
			_callback1->OnFindFirstFileNameTransactedW(fileName, result);
			_callback2->OnFindFirstFileNameTransactedW(fileName, result);
		}

		void OnFindFirstFileTransactedA(std::string_view fileName) override final
		{
			_callback1->OnFindFirstFileTransactedA(fileName);
			_callback2->OnFindFirstFileTransactedA(fileName);
		}

		void OnFindFirstFileTransactedW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstFileTransactedW(fileName);
			_callback2->OnFindFirstFileTransactedW(fileName);
		}

		void OnFindFirstStreamTransactedW(std::wstring_view fileName) override final
		{
			_callback1->OnFindFirstStreamTransactedW(fileName);
			_callback2->OnFindFirstStreamTransactedW(fileName);
		}

		void OnGetBinaryTypeA(std::string_view applicationName, bool result) override final
		{
			_callback1->OnGetBinaryTypeA(applicationName, result);
			_callback2->OnGetBinaryTypeA(applicationName, result);
		}

		void OnGetBinaryTypeW(std::wstring_view applicationName, bool result) override final
		{
			_callback1->OnGetBinaryTypeW(applicationName, result);
			_callback2->OnGetBinaryTypeW(applicationName, result);
		}

		void OnGetCompressedFileSizeTransactedA(std::string_view fileName, uint32_t result) override final
		{
			_callback1->OnGetCompressedFileSizeTransactedA(fileName, result);
			_callback2->OnGetCompressedFileSizeTransactedA(fileName, result);
		}

		void OnGetCompressedFileSizeTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			_callback1->OnGetCompressedFileSizeTransactedW(fileName, result);
			_callback2->OnGetCompressedFileSizeTransactedW(fileName, result);
		}

		void OnGetDllDirectoryA(uint32_t result) override final
		{
			_callback1->OnGetDllDirectoryA(result);
			_callback2->OnGetDllDirectoryA(result);
		}

		void OnGetDllDirectoryW(uint32_t result) override final
		{
			_callback1->OnGetDllDirectoryW(result);
			_callback2->OnGetDllDirectoryW(result);
		}

		void OnGetFileAttributesTransactedA(std::string_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
		}

		void OnGetFileAttributesTransactedW(std::wstring_view fileName, uint32_t result, bool wasBlocked) override final
		{
			_callback1->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
			_callback2->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
		}

		void OnGetFileBandwidthReservation(bool result) override final
		{
			_callback1->OnGetFileBandwidthReservation(result);
			_callback2->OnGetFileBandwidthReservation(result);
		}
		
		void OnGetFileInformationByHandleEx(bool result) override final
		{
			_callback1->OnGetFileInformationByHandleEx(result);
			_callback2->OnGetFileInformationByHandleEx(result);
		}

		void OnGetFileSecurityA(std::string_view fileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnGetFileSecurityA(fileName, result, wasBlocked);
			_callback2->OnGetFileSecurityA(fileName, result, wasBlocked);
		}

		void OnGetFullPathNameTransactedA(std::string_view fileName, uint32_t result) override final
		{
			_callback1->OnGetFullPathNameTransactedA(fileName, result);
			_callback2->OnGetFullPathNameTransactedA(fileName, result);
		}

		void OnGetFullPathNameTransactedW(std::wstring_view fileName, uint32_t result) override final
		{
			_callback1->OnGetFullPathNameTransactedW(fileName, result);
			_callback2->OnGetFullPathNameTransactedW(fileName, result);
		}

		void OnGetLongPathNameTransactedA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			_callback1->OnGetLongPathNameTransactedA(shortPath, longPath, result);
			_callback2->OnGetLongPathNameTransactedA(shortPath, longPath, result);
		}

		void OnGetLongPathNameTransactedW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			_callback1->OnGetLongPathNameTransactedW(shortPath, longPath, result);
			_callback2->OnGetLongPathNameTransactedW(shortPath, longPath, result);
		}

		void OnGetQueuedCompletionStatus(bool result) override final
		{
			_callback1->OnGetQueuedCompletionStatus(result);
			_callback2->OnGetQueuedCompletionStatus(result);
		}

		void OnGetQueuedCompletionStatusEx(bool result) override final
		{
			_callback1->OnGetQueuedCompletionStatusEx(result);
			_callback2->OnGetQueuedCompletionStatusEx(result);
		}

		void OnGetShortPathNameA(std::string_view longPath, std::string_view shortPath, uint32_t result) override final
		{
			_callback1->OnGetShortPathNameA(longPath, shortPath, result);
			_callback2->OnGetShortPathNameA(longPath, shortPath, result);
		}

		void OnLoadModule(std::string_view moduleName, uint32_t result) override final
		{
			_callback1->OnLoadModule(moduleName, result);
			_callback2->OnLoadModule(moduleName, result);
		}

		void OnLoadPackagedLibrary(std::string_view libFileName) override final
		{
			_callback1->OnLoadPackagedLibrary(libFileName);
			_callback2->OnLoadPackagedLibrary(libFileName);
		}

		void OnMoveFileA(std::string_view existingFileName, std::string_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
		}

		void OnMoveFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
			_callback2->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
		}

		void OnMoveFileExA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileTransactedA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileWithProgressA(std::string_view existingFileName, std::string_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnMoveFileWithProgressW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t flags, bool result, bool wasBlocked) override final
		{
			_callback1->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
			_callback2->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
		}

		void OnOpenEncryptedFileRawA(std::string_view fileName, uint32_t flags, uint32_t result) override final
		{
			_callback1->OnOpenEncryptedFileRawA(fileName, flags, result);
			_callback2->OnOpenEncryptedFileRawA(fileName, flags, result);
		}

		void OnOpenEncryptedFileRawW(std::wstring_view fileName, uint32_t flags, uint32_t result) override final
		{
			_callback1->OnOpenEncryptedFileRawW(fileName, flags, result);
			_callback2->OnOpenEncryptedFileRawW(fileName, flags, result);
		}

		void OnOpenFile(std::string_view fileName, bool wasBlocked) override final
		{
			_callback1->OnOpenFile(fileName, wasBlocked);
			_callback2->OnOpenFile(fileName, wasBlocked);
		}

		void OnOpenFileById() override final
		{
			_callback1->OnOpenFileById();
			_callback2->OnOpenFileById();
		}

		void OnReadEncryptedFileRaw(uint32_t result) override final
		{
			_callback1->OnReadEncryptedFileRaw(result);
			_callback2->OnReadEncryptedFileRaw(result);
		}

		void OnRemoveDirectoryTransactedA(std::string_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
			_callback2->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
		}

		void OnRemoveDirectoryTransactedW(std::wstring_view pathName, bool result, bool wasBlocked) override final
		{
			_callback1->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
			_callback2->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
		}

		void OnReOpenFile() override final
		{
			_callback1->OnReOpenFile();
			_callback2->OnReOpenFile();
		}

		void OnReplaceFileA(std::string_view replacedFileName, std::string_view replacementFileName, std::string_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			_callback1->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
			_callback2->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
		}

		void OnReplaceFileW(std::wstring_view replacedFileName, std::wstring_view replacementFileName, std::wstring_view backupFileName, uint32_t replaceFlags, bool result) override final
		{
			_callback1->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
			_callback2->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
		}

		void OnSetCurrentDirectoryA(std::string_view pathName, bool result) override final
		{
			_callback1->OnSetCurrentDirectoryA(pathName, result);
			_callback2->OnSetCurrentDirectoryA(pathName, result);
		}

		void OnSetCurrentDirectoryW(std::wstring_view pathName, bool result) override final
		{
			_callback1->OnSetCurrentDirectoryW(pathName, result);
			_callback2->OnSetCurrentDirectoryW(pathName, result);
		}

		void OnSetDllDirectoryA(std::string_view pathName, bool result) override final
		{
			_callback1->OnSetDllDirectoryA(pathName, result);
			_callback2->OnSetDllDirectoryA(pathName, result);
		}

		void OnSetDllDirectoryW(std::wstring_view pathName, bool result) override final
		{
			_callback1->OnSetDllDirectoryW(pathName, result);
			_callback2->OnSetDllDirectoryW(pathName, result);
		}

		void OnSetFileAttributesTransactedA(std::string_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) override final
		{
			_callback1->OnSetFileAttributesTransactedA(pathName, fileAttributes, result, wasBlocked);
			_callback2->OnSetFileAttributesTransactedA(pathName, fileAttributes, result, wasBlocked);
		}

		void OnSetFileAttributesTransactedW(std::wstring_view pathName, uint32_t fileAttributes, bool result, bool wasBlocked) override final
		{
			_callback1->OnSetFileAttributesTransactedW(pathName, fileAttributes, result, wasBlocked);
			_callback2->OnSetFileAttributesTransactedW(pathName, fileAttributes, result, wasBlocked);
		}

		void OnSetFileBandwidthReservation(bool result) override final
		{
			_callback1->OnSetFileBandwidthReservation(result);
			_callback2->OnSetFileBandwidthReservation(result);
		}

		void OnSetFileCompletionNotificationModes(bool result) override final
		{
			_callback1->OnSetFileCompletionNotificationModes(result);
			_callback2->OnSetFileCompletionNotificationModes(result);
		}

		void OnSetFileSecurityA(std::string_view fileName, bool result) override final
		{
			_callback1->OnSetFileSecurityA(fileName, result);
			_callback2->OnSetFileSecurityA(fileName, result);
		}

		void OnSetFileShortNameA(std::string_view shortName, bool result) override final
		{
			_callback1->OnSetFileShortNameA(shortName, result);
			_callback2->OnSetFileShortNameA(shortName, result);
		}

		void OnSetFileShortNameW(std::wstring_view shortName, bool result) override final
		{
			_callback1->OnSetFileShortNameW(shortName, result);
			_callback2->OnSetFileShortNameW(shortName, result);
		}

		void OnSetSearchPathMode(uint32_t flags, bool result) override final
		{
			_callback1->OnSetSearchPathMode(flags, result);
			_callback2->OnSetSearchPathMode(flags, result);
		}

		void OnWriteEncryptedFileRaw(uint32_t result) override final
		{
			_callback1->OnWriteEncryptedFileRaw(result);
			_callback2->OnWriteEncryptedFileRaw(result);
		}

	private:
		std::shared_ptr<IDetourCallback> _callback1;
		std::shared_ptr<IDetourCallback> _callback2;
	};
}
