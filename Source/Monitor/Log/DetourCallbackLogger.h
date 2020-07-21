#pragma once 

namespace Monitor
{
	class DetourCallbackLogger : public IDetourCallback
	{
	public:
		DetourCallbackLogger(std::ostream& stream) :
			m_stream(stream)
		{
		}

		void OnShutdown() override final
		{
			m_stream << "Shutdown: " << std::endl;
		}

		void OnError(std::string_view message) override final
		{
			m_stream << "Error: " << message << std::endl;
		}

		// FileApi
		void OnAreFileApisANSI(bool result) override final
		{
			m_stream << "AreFileApisANSI: " << std::endl;
		}

		void OnCompareFileTime(int32_t result) override final
		{
			m_stream << "CompareFileTime: " << std::endl;
		}

		void OnCreateDirectoryA(std::string_view pathName, bool result) override final
		{
			m_stream << "CreateDirectoryA: " << pathName << std::endl;
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateDirectoryW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnCreateFile2(std::wstring_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateFile2: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnCreateFileA(std::string_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition, uint32_t flagsAndAttributes) override final
		{
			m_stream << "CreateFileA: " << fileName << std::endl;
		}

		void OnCreateFileW(std::wstring_view fileName, uint32_t desiredAccess, uint32_t sharedMode, uint32_t creationDisposition, uint32_t flagsAndAttributes) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateFileW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnDefineDosDeviceW(uint32_t flags, std::wstring_view deviceName, std::wstring_view targetPath, bool result) override final
		{
			m_stream << "DefineDosDeviceW: " << std::endl;
		}

		void OnDeleteFileA(std::string_view fileName, bool result) override final
		{
			m_stream << "DeleteFileA: " << fileName << std::endl;
		}

		void OnDeleteFileW(std::wstring_view fileName, bool result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "DeleteFileW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnDeleteVolumeMountPointW(std::wstring_view volumeMountPoint, bool result) override final
		{
			m_stream << "DeleteVolumeMountPointW: " << std::endl;
		}

		void OnFileTimeToLocalFileTime(bool result) override final
		{
			m_stream << "FileTimeToLocalFileTime: " << std::endl;
		}

		void OnFindClose(bool result) override final
		{
			m_stream << "FindClose: " << std::endl;
		}

		void OnFindCloseChangeNotification(bool result) override final
		{
			m_stream << "FindCloseChangeNotification: " << std::endl;
		}

		void OnFindFirstChangeNotificationA(std::string_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			m_stream << "FindFirstChangeNotificationA: " << std::endl;
		}

		void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool watchSubtree, uint32_t notifyFilter) override final
		{
			m_stream << "FindFirstChangeNotificationW: " << std::endl;
		}

		void OnFindFirstFileA(std::string_view fileName) override final
		{
			m_stream << "FindFirstFileA: " << std::endl;
		}

		void OnFindFirstFileW(std::wstring_view fileName) override final
		{
			m_stream << "FindFirstFileW: " << std::endl;
		}

		void OnFindFirstFileExA(std::string_view fileName) override final
		{
			m_stream << "FindFirstFileExA: " << std::endl;
		}

		void OnFindFirstFileExW(std::wstring_view fileName) override final
		{
			m_stream << "FindFirstFileExW: " << std::endl;
		}

		void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t flags) override final
		{
			m_stream << "FindFirstFileNameW: " << std::endl;
		}

		void OnFindFirstStreamW(std::wstring_view fileName) override final
		{
			m_stream << "FindFirstStreamW: " << std::endl;
		}

		void OnFindFirstVolumeW(std::wstring_view fileName) override final
		{
			m_stream << "FindFirstVolumeW: " << std::endl;
		}

		void OnFindNextChangeNotification(bool result) override final
		{
			m_stream << "FindNextChangeNotification: " << std::endl;
		}

		void OnFindNextFileA(bool result) override final
		{
			m_stream << "FindNextFileA: " << std::endl;
		}

		void OnFindNextFileW(bool result) override final
		{
			m_stream << "FindNextFileW: " << std::endl;
		}

		void OnFindNextFileNameW(bool result) override final
		{
			m_stream << "FindNextFileNameW: " << std::endl;
		}

		void OnFindNextStreamW(bool result) override final
		{
			m_stream << "FindNextStreamW: " << std::endl;
		}

		void OnFindNextVolumeW(bool result) override final
		{
			m_stream << "FindNextVolumeW: " << std::endl;
		}

		void OnFindVolumeClose(bool result) override final
		{
			m_stream << "FindVolumeClose: " << std::endl;
		}

		void OnFlushFileBuffers(bool result) override final
		{
			m_stream << "FlushFileBuffers: " << std::endl;
		}

		void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t result) override final
		{
			m_stream << "GetCompressedFileSizeA: " << fileName << std::endl;
		}

		void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetCompressedFileSizeW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool result) override final
		{
			m_stream << "GetDiskFreeSpaceA: " << std::endl;
		}

		void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool result) override final
		{
			m_stream << "GetDiskFreeSpaceW: " << std::endl;
		}

		void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool result) override final
		{
			m_stream << "GetDiskFreeSpaceExA: " << std::endl;
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool result) override final
		{
			m_stream << "GetDiskFreeSpaceExW: " << std::endl;
		}

		void OnGetDriveTypeA(std::string_view rootPathName, uint32_t result) override final
		{
			m_stream << "GetDriveTypeA: " << std::endl;
		}

		void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t result) override final
		{
			m_stream << "GetDriveTypeW: " << std::endl;
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t result) override final
		{
			m_stream << "GetFileAttributesA: " << fileName << std::endl;
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFileAttributesW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool result) override final
		{
			m_stream << "GetFileAttributesExA: " << fileName << std::endl;
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFileAttributesExW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetFileInformationByHandle(bool result) override final
		{
			m_stream << "GetFileInformationByHandle: " << std::endl;
		}

		void OnGetFileSize(uint32_t result) override final
		{
			m_stream << "GetFileSize: " << std::endl;
		}

		void OnGetFileSizeEx(uint32_t result) override final
		{
			m_stream << "GetFileSizeEx: " << std::endl;
		}

		void OnGetFileTime(bool result) override final
		{
			m_stream << "GetFileTime: " << std::endl;
		}

		void OnGetFileType(uint32_t result) override final
		{
			m_stream << "GetFileType: " << std::endl;
		}

		void OnGetFinalPathNameByHandleA(uint32_t result) override final
		{
			m_stream << "GetFinalPathNameByHandleA: " << std::endl;
		}

		void OnGetFinalPathNameByHandleW(uint32_t result) override final
		{
			m_stream << "GetFinalPathNameByHandleW: " << std::endl;
		}

		void OnGetFullPathNameA(std::string_view fileName, uint32_t result) override final
		{
			m_stream << "GetFullPathNameA: " << fileName << std::endl;
		}

		void OnGetFullPathNameW(std::wstring_view fileName, uint32_t result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFullPathNameW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetLogicalDrives(uint32_t result) override final
		{
			m_stream << "GetLogicalDrives: " << std::endl;
		}

		void OnGetLogicalDriveStringsW(std::wstring_view buffer, uint32_t result) override final
		{
			m_stream << "GetLogicalDriveStringsW: " << std::endl;
		}

		void OnGetLongPathNameA(std::string_view shortPath, std::string_view longPath, uint32_t result) override final
		{
			m_stream << "GetLongPathNameA: " << std::endl;
		}

		void OnGetLongPathNameW(std::wstring_view shortPath, std::wstring_view longPath, uint32_t result) override final
		{
			m_stream << "GetLongPathNameW: " << std::endl;
		}

		void OnGetShortPathNameW(std::wstring_view longPath, std::wstring_view shortPath, uint32_t result) override final
		{
			m_stream << "GetShortPathNameW: " << std::endl;
		}

		void OnGetTempFileNameA(std::string_view pathName, std::string_view prefixString, uint32_t unique, std::string_view tempFileName, uint32_t result) override final
		{
			m_stream << "GetTempFileNameA: " << std::endl;
		}

		void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view prefixString, uint32_t unique, std::wstring_view tempFileName, uint32_t result) override final
		{
			m_stream << "GetTempFileNameW: " << std::endl;
		}

		void OnGetTempPathA(std::string_view buffer, uint32_t result) override final
		{
			m_stream << "GetTempPathA: " << std::endl;
		}

		void OnGetTempPathW(std::wstring_view buffer, uint32_t result) override final
		{
			m_stream << "GetTempPathW: " << std::endl;
		}

		void OnGetVolumeInformationA(bool result) override final
		{
			m_stream << "GetVolumeInformationA: " << std::endl;
		}

		void OnGetVolumeInformationW(bool result) override final
		{
			m_stream << "GetVolumeInformationW: " << std::endl;
		}

		void OnGetVolumeInformationByHandleW(bool result) override final
		{
			m_stream << "GetVolumeInformationByHandleW: " << std::endl;
		}

		void OnGetVolumeNameForVolumeMountPointW(bool result) override final
		{
			m_stream << "GetVolumeNameForVolumeMountPointW: " << std::endl;
		}

		void OnGetVolumePathNamesForVolumeNameW(bool result) override final
		{
			m_stream << "GetVolumePathNamesForVolumeNameW: " << std::endl;
		}

		void OnGetVolumePathNameW(std::wstring_view filename, bool result) override final
		{
			m_stream << "GetVolumePathNameW: " << std::endl;
		}

		void OnLocalFileTimeToFileTime(bool result) override final
		{
			m_stream << "LocalFileTimeToFileTime: " << std::endl;
		}

		void OnLockFile(bool result) override final
		{
			m_stream << "LockFile: " << std::endl;
		}

		void OnLockFileEx(bool result) override final
		{
			m_stream << "LockFileEx: " << std::endl;
		}

		void OnQueryDosDeviceW(std::wstring_view deviceName, uint32_t result) override final
		{
			m_stream << "QueryDosDeviceW: " << std::endl;
		}

		void OnReadFile(bool result) override final
		{
			m_stream << "ReadFile: " << std::endl;
		}

		void OnReadFileEx(bool result) override final
		{
			m_stream << "ReadFileEx: " << std::endl;
		}

		void OnReadFileScatter(bool result) override final
		{
			m_stream << "ReadFileScatter: " << std::endl;
		}

		void OnRemoveDirectoryA(std::string_view pathName, bool result) override final
		{
			m_stream << "RemoveDirectoryA: " << pathName << std::endl;
		}

		void OnRemoveDirectoryW(std::wstring_view pathName, bool result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "RemoveDirectoryW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnSetEndOfFile(bool result) override final
		{
			m_stream << "SetEndOfFile: " << std::endl;
		}
		
		void OnSetFileApisToANSI() override final
		{
			m_stream << "SetFileApisToANSI: " << std::endl;
		}

		void OnSetFileApisToOEM() override final
		{
			m_stream << "SetFileApisToOEM: " << std::endl;
		}

		void OnSetFileAttributesA(std::string_view fileName, bool result) override final
		{
			m_stream << "SetFileAttributesA: " << fileName << std::endl;
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool result) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetFileAttributesW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnSetFileInformationByHandle(bool result) override final
		{
			m_stream << "SetFileInformationByHandle: " << std::endl;
		}

		void OnSetFileIoOverlappedRange(bool result) override final
		{
			m_stream << "SetFileIoOverlappedRange: " << std::endl;
		}

		void OnSetFilePointer(uint32_t result) override final
		{
			m_stream << "SetFilePointer: " << std::endl;
		}
		
		void OnSetFilePointerEx(bool result) override final
		{
			m_stream << "SetFilePointerEx: " << std::endl;
		}

		void OnSetFileTime(bool result) override final
		{
			m_stream << "SetFileTime: " << std::endl;
		}

		void OnSetFileValidData(bool result) override final
		{
			m_stream << "SetFileValidData: " << std::endl;
		}

		void OnUnlockFile(bool result) override final
		{
			m_stream << "UnlockFile: " << std::endl;
		}

		void OnUnlockFileEx(bool result) override final
		{
			m_stream << "UnlockFileEx: " << std::endl;
		}

		void OnWriteFile(bool result) override final
		{
			m_stream << "WriteFile: " << std::endl;
		}

		void OnWriteFileEx(bool result) override final
		{
			m_stream << "WriteFileEx: " << std::endl;
		}

		void OnWriteFileGather(bool result) override final
		{
			m_stream << "WriteFileGather: " << std::endl;
		}

		// LibLoaderApi
		void OnLoadLibraryA() override final
		{
			m_stream << "LoadLibraryA: " << std::endl;
		}

		void OnLoadLibraryW() override final
		{
			m_stream << "LoadLibraryW: " << std::endl;
		}

		void OnLoadLibraryExA() override final
		{
			m_stream << "LoadLibraryExA: " << std::endl;
		}

		void OnLoadLibraryExW(std::wstring_view libFileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "LoadLibraryExW: " << converter.to_bytes(libFileName.data()) << std::endl;
		}

		// ProcessEnv
		void OnSearchPathA() override final
		{
			m_stream << "SearchPathA: " << std::endl;
		}

		void OnSearchPathW() override final
		{
			m_stream << "SearchPathW: " << std::endl;
		}

		// ProcessThreadsApi
		void OnCreateProcessA() override final
		{
			m_stream << "CreateProcessA: " << std::endl;
		}

		void OnCreateProcessW() override final
		{
			m_stream << "CreateProcessW: " << std::endl;
		}

		void OnCreateProcessAsUserA() override final
		{
			m_stream << "CreateProcessAsUserA: " << std::endl;
		}

		void OnCreateProcessAsUserW() override final
		{
			m_stream << "CreateProcessAsUserW: " << std::endl;
		}

		void OnExitProcess() override final
		{
			m_stream << "ExitProcess: " << std::endl;
		}

		// UndocumentedApi
		void OnPrivCopyFileExA() override final
		{
			m_stream << "PrivCopyFileExA: " << std::endl;
		}

		void OnPrivCopyFileExW() override final
		{
			m_stream << "PrivCopyFileExW: " << std::endl;
		}

		// WinBase
		void OnCopyFileA() override final
		{
			m_stream << "CopyFileA: " << std::endl;
		}

		void OnCopyFileW() override final
		{
			m_stream << "CopyFileW: " << std::endl;
		}

		void OnCopyFile2() override final
		{
			m_stream << "CopyFile2: " << std::endl;
		}

		void OnCopyFileExA() override final
		{
			m_stream << "CopyFileExA: " << std::endl;
		}

		void OnCopyFileExW() override final
		{
			m_stream << "CopyFileExW: " << std::endl;
		}

		void OnCopyFileTransactedA() override final
		{
			m_stream << "CopyFileTransactedA: " << std::endl;
		}

		void OnCopyFileTransactedW() override final
		{
			m_stream << "CopyFileTransactedW: " << std::endl;
		}

		void OnCreateDirectoryExA() override final
		{
			m_stream << "CreateDirectoryExA: " << std::endl;
		}

		void OnCreateDirectoryExW() override final
		{
			m_stream << "CreateDirectoryExW: " << std::endl;
		}

		void OnCreateDirectoryTransactedA() override final
		{
			m_stream << "CreateDirectoryTransactedA: " << std::endl;
		}

		void OnCreateDirectoryTransactedW() override final
		{
			m_stream << "CreateDirectoryTransactedW: " << std::endl;
		}

		void OnCreateFileTransactedA() override final
		{
			m_stream << "CreateFileTransactedA: " << std::endl;
		}

		void OnCreateFileTransactedW() override final
		{
			m_stream << "CreateFileTransactedW: " << std::endl;
		}

		void OnCreateHardLinkA() override final
		{
			m_stream << "CreateHardLinkA: " << std::endl;
		}

		void OnCreateHardLinkW() override final
		{
			m_stream << "CreateHardLinkW: " << std::endl;
		}

		void OnCreateHardLinkTransactedA() override final
		{
			m_stream << "CreateHardLinkTransactedA: " << std::endl;
		}

		void OnCreateHardLinkTransactedW() override final
		{
			m_stream << "CreateHardLinkTransactedW: " << std::endl;
		}

		void OnCreateProcessWithLogonW() override final
		{
			m_stream << "CreateProcessWithLogonW: " << std::endl;
		}

		void OnCreateProcessWithTokenW() override final
		{
			m_stream << "CreateProcessWithTokenW: " << std::endl;
		}

		void OnCreateSymbolicLinkA() override final
		{
			m_stream << "CreateSymbolicLinkA: " << std::endl;
		}

		void OnCreateSymbolicLinkW() override final
		{
			m_stream << "CreateSymbolicLinkW: " << std::endl;
		}

		void OnCreateSymbolicLinkTransactedA() override final
		{
			m_stream << "CreateSymbolicLinkTransactedA: " << std::endl;
		}

		void OnCreateSymbolicLinkTransactedW() override final
		{
			m_stream << "CreateSymbolicLinkTransactedW: " << std::endl;
		}

		void OnDecryptFileA() override final
		{
			m_stream << "DecryptFileA: " << std::endl;
		}
		
		void OnDecryptFileW() override final
		{
			m_stream << "DecryptFileW: " << std::endl;
		}

		void OnDeleteFileTransactedA() override final
		{
			m_stream << "DeleteFileTransactedA: " << std::endl;
		}

		void OnDeleteFileTransactedW() override final
		{
			m_stream << "DeleteFileTransactedW: " << std::endl;
		}

		void OnEncryptFileA() override final
		{
			m_stream << "EncryptFileA: " << std::endl;
		}

		void OnEncryptFileW() override final
		{
			m_stream << "EncryptFileW: " << std::endl;
		}

		void OnFileEncryptionStatusA() override final
		{
			m_stream << "FileEncryptionStatusA: " << std::endl;
		}

		void OnFileEncryptionStatusW() override final
		{
			m_stream << "FileEncryptionStatusW: " << std::endl;
		}

		void OnFindFirstFileNameTransactedW() override final
		{
			m_stream << "FindFirstFileNameTransactedW: " << std::endl;
		}

		void OnFindFirstFileTransactedA() override final
		{
			m_stream << "FindFirstFileTransactedA: " << std::endl;
		}

		void OnFindFirstFileTransactedW() override final
		{
			m_stream << "FindFirstFileTransactedW: " << std::endl;
		}

		void OnFindFirstStreamTransactedW() override final
		{
			m_stream << "FindFirstStreamTransactedW: " << std::endl;
		}

		void OnGetBinaryTypeA() override final
		{
			m_stream << "GetBinaryTypeA: " << std::endl;
		}

		void OnGetBinaryTypeW() override final
		{
			m_stream << "GetBinaryTypeW: " << std::endl;
		}

		void OnGetCompressedFileSizeTransactedA() override final
		{
			m_stream << "GetCompressedFileSizeTransactedA: " << std::endl;
		}

		void OnGetCompressedFileSizeTransactedW() override final
		{
			m_stream << "GetCompressedFileSizeTransactedW: " << std::endl;
		}

		void OnGetDllDirectoryA() override final
		{
			m_stream << "GetDllDirectoryA: " << std::endl;
		}

		void OnGetDllDirectoryW() override final
		{
			m_stream << "GetDllDirectoryW: " << std::endl;
		}

		void OnGetFileAttributesTransactedA() override final
		{
			m_stream << "GetFileAttributesTransactedA: " << std::endl;
		}

		void OnGetFileAttributesTransactedW() override final
		{
			m_stream << "GetFileAttributesTransactedW: " << std::endl;
		}

		void OnGetFileBandwidthReservation() override final
		{
			m_stream << "GetFileBandwidthReservation: " << std::endl;
		}
		
		void OnGetFileInformationByHandleEx() override final
		{
			m_stream << "GetFileInformationByHandleEx: " << std::endl;
		}

		void OnGetFileSecurityA() override final
		{
			m_stream << "GetFileSecurityA: " << std::endl;
		}

		void OnGetFullPathNameTransactedA() override final
		{
			m_stream << "GetFullPathNameTransactedA: " << std::endl;
		}

		void OnGetFullPathNameTransactedW() override final
		{
			m_stream << "GetFullPathNameTransactedW: " << std::endl;
		}

		void OnGetLongPathNameTransactedA() override final
		{
			m_stream << "GetLongPathNameTransactedA: " << std::endl;
		}

		void OnGetLongPathNameTransactedW() override final
		{
			m_stream << "GetLongPathNameTransactedW: " << std::endl;
		}

		void OnGetQueuedCompletionStatus() override final
		{
			m_stream << "GetQueuedCompletionStatus: " << std::endl;
		}

		void OnGetQueuedCompletionStatusEx() override final
		{
			m_stream << "GetQueuedCompletionStatusEx: " << std::endl;
		}

		void OnGetShortPathNameA() override final
		{
			m_stream << "GetShortPathNameA: " << std::endl;
		}

		void OnLoadModule() override final
		{
			m_stream << "LoadModule: " << std::endl;
		}

		void OnLoadPackagedLibrary() override final
		{
			m_stream << "LoadPackagedLibrary: " << std::endl;
		}

		void OnMoveFileA() override final
		{
			m_stream << "MoveFileA: " << std::endl;
		}

		void OnMoveFileW() override final
		{
			m_stream << "MoveFileW: " << std::endl;
		}

		void OnMoveFileExA() override final
		{
			m_stream << "MoveFileExA: " << std::endl;
		}

		void OnMoveFileExW() override final
		{
			m_stream << "MoveFileExW: " << std::endl;
		}

		void OnMoveFileTransactedA() override final
		{
			m_stream << "MoveFileTransactedA: " << std::endl;
		}

		void OnMoveFileTransactedW() override final
		{
			m_stream << "MoveFileTransactedW: " << std::endl;
		}

		void OnMoveFileWithProgressA() override final
		{
			m_stream << "MoveFileWithProgressA: " << std::endl;
		}

		void OnMoveFileWithProgressW() override final
		{
			m_stream << "MoveFileWithProgressW: " << std::endl;
		}

		void OnOpenEncryptedFileRawA() override final
		{
			m_stream << "OpenEncryptedFileRawA: " << std::endl;
		}

		void OnOpenEncryptedFileRawW() override final
		{
			m_stream << "OpenEncryptedFileRawW: " << std::endl;
		}

		void OnOpenFile() override final
		{
			m_stream << "OpenFile: " << std::endl;
		}

		void OnOpenFileById() override final
		{
			m_stream << "OpenFileById: " << std::endl;
		}

		void OnReadEncryptedFileRaw() override final
		{
			m_stream << "ReadEncryptedFileRaw: " << std::endl;
		}

		void OnRemoveDirectoryTransactedA() override final
		{
			m_stream << "RemoveDirectoryTransactedA: " << std::endl;
		}

		void OnRemoveDirectoryTransactedW() override final
		{
			m_stream << "RemoveDirectoryTransactedW: " << std::endl;
		}

		void OnReOpenFile() override final
		{
			m_stream << "ReOpenFile: " << std::endl;
		}

		void OnReplaceFileA() override final
		{
			m_stream << "ReplaceFileA: " << std::endl;
		}

		void OnReplaceFileW() override final
		{
			m_stream << "ReplaceFileW: " << std::endl;
		}

		void OnSetCurrentDirectoryA() override final
		{
			m_stream << "SetCurrentDirectoryA: " << std::endl;
		}

		void OnSetCurrentDirectoryW() override final
		{
			m_stream << "SetCurrentDirectoryW: " << std::endl;
		}

		void OnSetDllDirectoryA() override final
		{
			m_stream << "SetDllDirectoryA: " << std::endl;
		}

		void OnSetDllDirectoryW() override final
		{
			m_stream << "SetDllDirectoryW: " << std::endl;
		}

		void OnSetFileAttributesTransactedA() override final
		{
			m_stream << "SetFileAttributesTransactedA: " << std::endl;
		}

		void OnSetFileAttributesTransactedW() override final
		{
			m_stream << "SetFileAttributesTransactedW: " << std::endl;
		}

		void OnSetFileBandwidthReservation() override final
		{
			m_stream << "SetFileBandwidthReservation: " << std::endl;
		}

		void OnSetFileCompletionNotificationModes() override final
		{
			m_stream << "SetFileCompletionNotificationModes: " << std::endl;
		}

		void OnSetFileSecurityA() override final
		{
			m_stream << "SetFileSecurityA: " << std::endl;
		}

		void OnSetFileShortNameA() override final
		{
			m_stream << "SetFileShortNameA: " << std::endl;
		}

		void OnSetFileShortNameW() override final
		{
			m_stream << "SetFileShortNameW: " << std::endl;
		}

		void OnSetSearchPathMode() override final
		{
			m_stream << "SetSearchPathMode: " << std::endl;
		}

		void OnWriteEncryptedFileRaw() override final
		{
			m_stream << "WriteEncryptedFileRaw: " << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
