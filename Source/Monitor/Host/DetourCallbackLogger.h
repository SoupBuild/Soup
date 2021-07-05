#pragma once 

namespace Monitor
{
	export class DetourCallbackLogger : public IDetourCallback
	{
	public:
		DetourCallbackLogger(std::ostream& stream) :
			m_stream(stream)
		{
		}

		void OnInitialize() override final
		{
			m_stream << "Initialize: " << std::endl;
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
		void OnAreFileApisANSI(bool /*result*/) override final
		{
			m_stream << "AreFileApisANSI: " << std::endl;
		}

		void OnCompareFileTime(int32_t /*result*/) override final
		{
			m_stream << "CompareFileTime: " << std::endl;
		}

		void OnCreateDirectoryA(std::string_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "CreateDirectoryA: " << pathName << " " << wasBlocked << std::endl;
		}

		void OnCreateDirectoryW(std::wstring_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateDirectoryW: " << converter.to_bytes(pathName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateFile2(
			std::wstring_view fileName,
			uint32_t /*desiredAccess*/,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateFile2: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateFileA(
			std::string_view fileName,
			uint32_t /*desiredAccess*/,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint32_t /*flagsAndAttributes*/,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CreateFileA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnCreateFileW(
			std::wstring_view fileName,
			uint32_t /*desiredAccess*/,
			uint32_t /*sharedMode*/,
			uint32_t /*creationDisposition*/,
			uint32_t /*flagsAndAttributes*/,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateFileW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnDefineDosDeviceW(uint32_t /*flags*/, std::wstring_view /*deviceName*/, std::wstring_view /*targetPath*/, bool /*result*/) override final
		{
			m_stream << "DefineDosDeviceW: " << std::endl;
		}

		void OnDeleteFileA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "DeleteFileA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnDeleteFileW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "DeleteFileW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnDeleteVolumeMountPointW(std::wstring_view /*volumeMountPoint*/, bool /*result*/) override final
		{
			m_stream << "DeleteVolumeMountPointW: " << std::endl;
		}

		void OnFileTimeToLocalFileTime(bool /*result*/) override final
		{
			m_stream << "FileTimeToLocalFileTime: " << std::endl;
		}

		void OnFindClose(bool /*result*/) override final
		{
			m_stream << "FindClose: " << std::endl;
		}

		void OnFindCloseChangeNotification(bool /*result*/) override final
		{
			m_stream << "FindCloseChangeNotification: " << std::endl;
		}

		void OnFindFirstChangeNotificationA(std::string_view pathName, bool /*watchSubtree*/, uint32_t /*notifyFilter*/) override final
		{
			m_stream << "FindFirstChangeNotificationA: " << pathName << std::endl;
		}

		void OnFindFirstChangeNotificationW(std::wstring_view pathName, bool /*watchSubtree*/, uint32_t /*notifyFilter*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstChangeNotificationW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnFindFirstFileA(std::string_view fileName) override final
		{
			m_stream << "FindFirstFileA: " << fileName << std::endl;
		}

		void OnFindFirstFileW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstFileW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstFileExA(std::string_view fileName) override final
		{
			m_stream << "FindFirstFileExA: " << fileName << std::endl;
		}

		void OnFindFirstFileExW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstFileExW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstFileNameW(std::wstring_view fileName, uint32_t /*flags*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstFileNameW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstStreamW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstStreamW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstVolumeW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstVolumeW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindNextChangeNotification(bool /*result*/) override final
		{
			m_stream << "FindNextChangeNotification: " << std::endl;
		}

		void OnFindNextFileA(bool /*result*/) override final
		{
			m_stream << "FindNextFileA: " << std::endl;
		}

		void OnFindNextFileW(bool /*result*/) override final
		{
			m_stream << "FindNextFileW: " << std::endl;
		}

		void OnFindNextFileNameW(bool /*result*/) override final
		{
			m_stream << "FindNextFileNameW: " << std::endl;
		}

		void OnFindNextStreamW(bool /*result*/) override final
		{
			m_stream << "FindNextStreamW: " << std::endl;
		}

		void OnFindNextVolumeW(bool /*result*/) override final
		{
			m_stream << "FindNextVolumeW: " << std::endl;
		}

		void OnFindVolumeClose(bool /*result*/) override final
		{
			m_stream << "FindVolumeClose: " << std::endl;
		}

		void OnFlushFileBuffers(bool /*result*/) override final
		{
			m_stream << "FlushFileBuffers: " << std::endl;
		}

		void OnGetCompressedFileSizeA(std::string_view fileName, uint32_t /*result*/) override final
		{
			m_stream << "GetCompressedFileSizeA: " << fileName << std::endl;
		}

		void OnGetCompressedFileSizeW(std::wstring_view fileName, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetCompressedFileSizeW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetDiskFreeSpaceA(std::string_view rootPathName, bool /*result*/) override final
		{
			m_stream << "GetDiskFreeSpaceA: " << rootPathName << std::endl;
		}

		void OnGetDiskFreeSpaceW(std::wstring_view rootPathName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetDiskFreeSpaceW: " << converter.to_bytes(rootPathName.data()) << std::endl;
		}

		void OnGetDiskFreeSpaceExA(std::string_view directoryName, bool /*result*/) override final
		{
			m_stream << "GetDiskFreeSpaceExA: " << directoryName << std::endl;
		}

		void OnGetDiskFreeSpaceExW(std::wstring_view directoryName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetDiskFreeSpaceExW: " << converter.to_bytes(directoryName.data()) << std::endl;
		}

		void OnGetDriveTypeA(std::string_view rootPathName, uint32_t /*result*/) override final
		{
			m_stream << "GetDriveTypeA: " << rootPathName << std::endl;
		}

		void OnGetDriveTypeW(std::wstring_view rootPathName, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetDriveTypeW: " << converter.to_bytes(rootPathName.data()) << std::endl;
		}

		void OnGetFileAttributesA(std::string_view fileName, uint32_t /*result*/, bool wasBlocked) override final
		{
			m_stream << "GetFileAttributesA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnGetFileAttributesW(std::wstring_view fileName, uint32_t /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFileAttributesW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnGetFileAttributesExA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "GetFileAttributesExA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnGetFileAttributesExW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFileAttributesExW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnGetFileInformationByHandle(bool /*result*/) override final
		{
			m_stream << "GetFileInformationByHandle: " << std::endl;
		}

		void OnGetFileSize(uint32_t /*result*/) override final
		{
			m_stream << "GetFileSize: " << std::endl;
		}

		void OnGetFileSizeEx(uint32_t /*result*/) override final
		{
			m_stream << "GetFileSizeEx: " << std::endl;
		}

		void OnGetFileTime(bool /*result*/) override final
		{
			m_stream << "GetFileTime: " << std::endl;
		}

		void OnGetFileType(uint32_t /*result*/) override final
		{
			m_stream << "GetFileType: " << std::endl;
		}

		void OnGetFinalPathNameByHandleA(uint32_t /*result*/) override final
		{
			m_stream << "GetFinalPathNameByHandleA: " << std::endl;
		}

		void OnGetFinalPathNameByHandleW(uint32_t /*result*/) override final
		{
			m_stream << "GetFinalPathNameByHandleW: " << std::endl;
		}

		void OnGetFullPathNameA(std::string_view fileName, uint32_t /*result*/) override final
		{
			m_stream << "GetFullPathNameA: " << fileName << std::endl;
		}

		void OnGetFullPathNameW(std::wstring_view fileName, std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFullPathNameW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetLogicalDrives(uint32_t /*result*/) override final
		{
			m_stream << "GetLogicalDrives: " << std::endl;
		}

		void OnGetLogicalDriveStringsW(std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
			m_stream << "GetLogicalDriveStringsW: " << std::endl;
		}

		void OnGetLongPathNameA(std::string_view /*shortPath*/, std::string_view /*longPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetLongPathNameA: " << std::endl;
		}

		void OnGetLongPathNameW(std::wstring_view /*shortPath*/, std::wstring_view /*longPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetLongPathNameW: " << std::endl;
		}

		void OnGetShortPathNameW(std::wstring_view /*longPath*/, std::wstring_view /*shortPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetShortPathNameW: " << std::endl;
		}

		void OnGetTempFileNameA(std::string_view /*pathName*/, std::string_view /*prefixString*/, uint32_t /*unique*/, std::string_view /*tempFileName*/, uint32_t /*result*/) override final
		{
			m_stream << "GetTempFileNameA: " << std::endl;
		}

		void OnGetTempFileNameW(std::wstring_view pathName, std::wstring_view /*prefixString*/, uint32_t /*unique*/, std::wstring_view /*tempFileName*/, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetTempFileNameW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnGetTempPathA(std::string_view /*buffer*/, uint32_t /*result*/) override final
		{
			m_stream << "GetTempPathA: " << std::endl;
		}

		void OnGetTempPathW(std::wstring_view /*buffer*/, uint32_t /*result*/) override final
		{
			m_stream << "GetTempPathW: " << std::endl;
		}

		void OnGetVolumeInformationA(bool /*result*/) override final
		{
			m_stream << "GetVolumeInformationA: " << std::endl;
		}

		void OnGetVolumeInformationW(bool /*result*/) override final
		{
			m_stream << "GetVolumeInformationW: " << std::endl;
		}

		void OnGetVolumeInformationByHandleW(bool /*result*/) override final
		{
			m_stream << "GetVolumeInformationByHandleW: " << std::endl;
		}

		void OnGetVolumeNameForVolumeMountPointW(bool /*result*/) override final
		{
			m_stream << "GetVolumeNameForVolumeMountPointW: " << std::endl;
		}

		void OnGetVolumePathNamesForVolumeNameW(bool /*result*/) override final
		{
			m_stream << "GetVolumePathNamesForVolumeNameW: " << std::endl;
		}

		void OnGetVolumePathNameW(std::wstring_view filename, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetVolumePathNameW: " << converter.to_bytes(filename.data()) << std::endl;
		}

		void OnLocalFileTimeToFileTime(bool /*result*/) override final
		{
			m_stream << "LocalFileTimeToFileTime: " << std::endl;
		}

		void OnLockFile(bool /*result*/) override final
		{
			m_stream << "LockFile: " << std::endl;
		}

		void OnLockFileEx(bool /*result*/) override final
		{
			m_stream << "LockFileEx: " << std::endl;
		}

		void OnQueryDosDeviceW(std::wstring_view /*deviceName*/, uint32_t /*result*/) override final
		{
			m_stream << "QueryDosDeviceW: " << std::endl;
		}

		void OnReadFile(bool /*result*/) override final
		{
			m_stream << "ReadFile: " << std::endl;
		}

		void OnReadFileEx(bool /*result*/) override final
		{
			m_stream << "ReadFileEx: " << std::endl;
		}

		void OnReadFileScatter(bool /*result*/) override final
		{
			m_stream << "ReadFileScatter: " << std::endl;
		}

		void OnRemoveDirectoryA(std::string_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "RemoveDirectoryA: " << pathName << " " << wasBlocked << std::endl;
		}

		void OnRemoveDirectoryW(std::wstring_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "RemoveDirectoryW: " << converter.to_bytes(pathName.data()) << " " << wasBlocked << std::endl;
		}

		void OnSetEndOfFile(bool /*result*/) override final
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

		void OnSetFileAttributesA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "SetFileAttributesA: " << fileName << " " << wasBlocked << std::endl;
		}
		
		void OnSetFileAttributesW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetFileAttributesW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnSetFileInformationByHandle(bool /*result*/) override final
		{
			m_stream << "SetFileInformationByHandle: " << std::endl;
		}

		void OnSetFileIoOverlappedRange(bool /*result*/) override final
		{
			m_stream << "SetFileIoOverlappedRange: " << std::endl;
		}

		void OnSetFilePointer(uint32_t /*result*/) override final
		{
			m_stream << "SetFilePointer: " << std::endl;
		}
		
		void OnSetFilePointerEx(bool /*result*/) override final
		{
			m_stream << "SetFilePointerEx: " << std::endl;
		}

		void OnSetFileTime(bool /*result*/) override final
		{
			m_stream << "SetFileTime: " << std::endl;
		}

		void OnSetFileValidData(bool /*result*/) override final
		{
			m_stream << "SetFileValidData: " << std::endl;
		}

		void OnUnlockFile(bool /*result*/) override final
		{
			m_stream << "UnlockFile: " << std::endl;
		}

		void OnUnlockFileEx(bool /*result*/) override final
		{
			m_stream << "UnlockFileEx: " << std::endl;
		}

		void OnWriteFile(bool /*result*/) override final
		{
			m_stream << "WriteFile: " << std::endl;
		}

		void OnWriteFileEx(bool /*result*/) override final
		{
			m_stream << "WriteFileEx: " << std::endl;
		}

		void OnWriteFileGather(bool /*result*/) override final
		{
			m_stream << "WriteFileGather: " << std::endl;
		}

		// LibLoaderApi
		void OnLoadLibraryA(std::string_view libFileName) override final
		{
			m_stream << "LoadLibraryA: " << libFileName << std::endl;
		}

		void OnLoadLibraryW(std::wstring_view libFileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "LoadLibraryW: " << converter.to_bytes(libFileName.data()) << std::endl;
		}

		void OnLoadLibraryExA(std::string_view libFileName) override final
		{
			m_stream << "LoadLibraryExA: " << libFileName << std::endl;
		}

		void OnLoadLibraryExW(std::wstring_view libFileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "LoadLibraryExW: " << converter.to_bytes(libFileName.data()) << std::endl;
		}

		// ProcessEnv
		void OnSearchPathA(std::string_view /*path*/, std::string_view fileName, std::string_view /*extension*/, uint32_t /*result*/) override final
		{
			m_stream << "SearchPathA: " << fileName << std::endl;
		}

		void OnSearchPathW(std::wstring_view /*path*/, std::wstring_view fileName, std::wstring_view /*extension*/, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SearchPathW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		// ProcessThreadsApi
		void OnCreateProcessA(bool /*wasDetoured*/, std::string_view applicationName, bool /*result*/) override final
		{
			m_stream << "CreateProcessA: " << applicationName << std::endl;
		}

		void OnCreateProcessW(bool /*wasDetoured*/, std::wstring_view applicationName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateProcessW: " << converter.to_bytes(applicationName.data()) << std::endl;
		}

		void OnCreateProcessAsUserA(std::string_view applicationName, bool /*result*/) override final
		{
			m_stream << "CreateProcessAsUserA: " << applicationName << std::endl;
		}

		void OnCreateProcessAsUserW(std::wstring_view applicationName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateProcessAsUserW: " << converter.to_bytes(applicationName.data()) << std::endl;
		}

		void OnExitProcess(uint32_t /*exitCode*/) override final
		{
			m_stream << "ExitProcess: " << std::endl;
		}

		// UndocumentedApi
		void OnPrivCopyFileExA(std::string_view existingFileName, std::string_view newFileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "PrivCopyFileExA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnPrivCopyFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "PrivCopyFileExW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		// WinBase
		void OnCopyFileA(std::string_view existingFileName, std::string_view newFileName, bool /*failIfExists*/, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "CopyFileA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnCopyFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool /*failIfExists*/, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CopyFileW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCopyFile2(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CopyFile2: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCopyFileExA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CopyFileExA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnCopyFileExW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CopyFileExW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCopyFileTransactedA(
			std::string_view existingFileName,
			std::string_view newFileName,
			bool /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CopyFileTransactedA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnCopyFileTransactedW(
			std::wstring_view existingFileName,
			std::wstring_view newFileName,
			bool /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CopyFileTransactedW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateDirectoryExA(
			std::string_view /*templateDirectory*/,
			std::string_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CreateDirectoryExA: " << newDirectory << " " << wasBlocked << std::endl;
		}

		void OnCreateDirectoryExW(
			std::wstring_view /*templateDirectory*/,
			std::wstring_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateDirectoryExW: " << converter.to_bytes(newDirectory.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateDirectoryTransactedA(
			std::string_view /*templateDirectory*/,
			std::string_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CreateDirectoryTransactedA: " << newDirectory << " " << wasBlocked << std::endl;
		}

		void OnCreateDirectoryTransactedW(
			std::wstring_view /*templateDirectory*/,
			std::wstring_view newDirectory,
			bool /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateDirectoryTransactedW: " << converter.to_bytes(newDirectory.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateFileTransactedA(
			std::string_view fileName,
			uint32_t /*desiredAccess*/, 
			uint32_t /*shareMode*/,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			m_stream << "CreateFileTransactedA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnCreateFileTransactedW(
			std::wstring_view fileName,
			uint32_t /*desiredAccess*/,
			uint32_t /*shareMode*/,
			uint64_t /*result*/,
			bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateFileTransactedW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnCreateHardLinkA(std::string_view fileName, std::string_view existingFileName, bool /*result*/) override final
		{
			m_stream << "CreateHardLinkA: " << fileName << " -> " << existingFileName << std::endl;
		}

		void OnCreateHardLinkW(std::wstring_view fileName, std::wstring_view existingFileName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateHardLinkW: " << converter.to_bytes(fileName.data()) << " -> " << converter.to_bytes(existingFileName.data()) << std::endl;
		}

		void OnCreateHardLinkTransactedA(std::string_view fileName, std::string_view existingFileName, bool /*result*/) override final
		{
			m_stream << "CreateHardLinkTransactedA: " << fileName << " -> " << existingFileName << std::endl;
		}

		void OnCreateHardLinkTransactedW(std::wstring_view fileName, std::wstring_view /*existingFileName*/, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateHardLinkTransactedW: " << converter.to_bytes(fileName.data()) << " -> " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnCreateProcessWithLogonW(std::wstring_view applicationName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateProcessWithLogonW: " << converter.to_bytes(applicationName.data()) << std::endl;
		}

		void OnCreateProcessWithTokenW(std::wstring_view applicationName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateProcessWithTokenW: " << converter.to_bytes(applicationName.data()) << std::endl;
		}

		void OnCreateSymbolicLinkA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t /*flags*/, bool /*result*/) override final
		{
			m_stream << "CreateSymbolicLinkA: " << symlinkFileName << " -> " << targetFileName << std::endl;
		}

		void OnCreateSymbolicLinkW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t /*flags*/, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateSymbolicLinkW: " << converter.to_bytes(symlinkFileName.data()) << " -> " << converter.to_bytes(targetFileName.data()) << std::endl;
		}

		void OnCreateSymbolicLinkTransactedA(std::string_view symlinkFileName, std::string_view targetFileName, uint32_t /*flags*/, bool /*result*/) override final
		{
			m_stream << "CreateSymbolicLinkTransactedA: " << symlinkFileName << " -> " << targetFileName << std::endl;
		}

		void OnCreateSymbolicLinkTransactedW(std::wstring_view symlinkFileName, std::wstring_view targetFileName, uint32_t /*flags*/, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "CreateSymbolicLinkTransactedW: " << converter.to_bytes(symlinkFileName.data()) << " -> " << converter.to_bytes(targetFileName.data()) << std::endl;
		}

		void OnDecryptFileA(std::string_view fileName, bool /*result*/) override final
		{
			m_stream << "DecryptFileA: " << fileName << std::endl;
		}
		
		void OnDecryptFileW(std::wstring_view fileName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "DecryptFileW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnDeleteFileTransactedA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "DeleteFileTransactedA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnDeleteFileTransactedW(std::wstring_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "DeleteFileTransactedW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnEncryptFileA(std::string_view fileName, bool /*result*/) override final
		{
			m_stream << "EncryptFileA: " << fileName << std::endl;
		}

		void OnEncryptFileW(std::wstring_view fileName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "EncryptFileW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFileEncryptionStatusA(std::string_view fileName, bool /*result*/) override final
		{
			m_stream << "FileEncryptionStatusA: " << fileName << std::endl;
		}

		void OnFileEncryptionStatusW(std::wstring_view fileName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FileEncryptionStatusW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstFileNameTransactedW(std::wstring_view fileName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstFileNameTransactedW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstFileTransactedA(std::string_view fileName) override final
		{
			m_stream << "FindFirstFileTransactedA: " << fileName << std::endl;
		}

		void OnFindFirstFileTransactedW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstFileTransactedW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnFindFirstStreamTransactedW(std::wstring_view fileName) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "FindFirstStreamTransactedW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetBinaryTypeA(std::string_view applicationName, bool /*result*/) override final
		{
			m_stream << "GetBinaryTypeA: " << applicationName << std::endl;
		}

		void OnGetBinaryTypeW(std::wstring_view applicationName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetBinaryTypeW: " << converter.to_bytes(applicationName.data()) << std::endl;
		}

		void OnGetCompressedFileSizeTransactedA(std::string_view fileName, uint32_t /*result*/) override final
		{
			m_stream << "GetCompressedFileSizeTransactedA: " << fileName << std::endl;
		}

		void OnGetCompressedFileSizeTransactedW(std::wstring_view fileName, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetCompressedFileSizeTransactedW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetDllDirectoryA(uint32_t /*result*/) override final
		{
			m_stream << "GetDllDirectoryA: " << std::endl;
		}

		void OnGetDllDirectoryW(uint32_t /*result*/) override final
		{
			m_stream << "GetDllDirectoryW: " << std::endl;
		}

		void OnGetEnvironmentVariableA(std::string_view name, uint32_t /*result*/) override final
		{
			m_stream << "GetEnvironmentVariableA: " << name << std::endl;
		}

		void OnGetEnvironmentVariableW(std::wstring_view name, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetEnvironmentVariableW: " << converter.to_bytes(name.data()) << std::endl;
		}

		void OnGetFileAttributesTransactedA(std::string_view fileName, uint32_t /*result*/, bool wasBlocked) override final
		{
			m_stream << "GetFileAttributesTransactedA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnGetFileAttributesTransactedW(std::wstring_view fileName, uint32_t /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFileAttributesTransactedW: " << converter.to_bytes(fileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnGetFileBandwidthReservation(bool /*result*/) override final
		{
			m_stream << "GetFileBandwidthReservation: " << std::endl;
		}
		
		void OnGetFileInformationByHandleEx(bool /*result*/) override final
		{
			m_stream << "GetFileInformationByHandleEx: " << std::endl;
		}

		void OnGetFileSecurityA(std::string_view fileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "GetFileSecurityA: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnGetFullPathNameTransactedA(std::string_view fileName, uint32_t /*result*/) override final
		{
			m_stream << "GetFullPathNameTransactedA: " << fileName << std::endl;
		}

		void OnGetFullPathNameTransactedW(std::wstring_view fileName, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "GetFullPathNameTransactedW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnGetLongPathNameTransactedA(std::string_view /*shortPath*/, std::string_view /*longPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetLongPathNameTransactedA: " << std::endl;
		}

		void OnGetLongPathNameTransactedW(std::wstring_view /*shortPath*/, std::wstring_view /*longPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetLongPathNameTransactedW: " << std::endl;
		}

		void OnGetQueuedCompletionStatus(bool /*result*/) override final
		{
			m_stream << "GetQueuedCompletionStatus: " << std::endl;
		}

		void OnGetQueuedCompletionStatusEx(bool /*result*/) override final
		{
			m_stream << "GetQueuedCompletionStatusEx: " << std::endl;
		}

		void OnGetShortPathNameA(std::string_view /*longPath*/, std::string_view /*shortPath*/, uint32_t /*result*/) override final
		{
			m_stream << "GetShortPathNameA: " << std::endl;
		}

		void OnLoadModule(std::string_view moduleName, uint32_t /*result*/) override final
		{
			m_stream << "LoadModule: " << moduleName << std::endl;
		}

		void OnLoadPackagedLibrary(std::string_view libFileName) override final
		{
			m_stream << "LoadPackagedLibrary: " << libFileName << std::endl;
		}

		void OnMoveFileA(std::string_view existingFileName, std::string_view newFileName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "MoveFileA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnMoveFileW(std::wstring_view existingFileName, std::wstring_view newFileName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "MoveFileW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnMoveFileExA(std::string_view existingFileName, std::string_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "MoveFileExA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnMoveFileExW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "MoveFileExW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnMoveFileTransactedA(std::string_view existingFileName, std::string_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "MoveFileTransactedA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnMoveFileTransactedW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "MoveFileTransactedW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnMoveFileWithProgressA(std::string_view existingFileName, std::string_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "MoveFileWithProgressA: " << existingFileName << " -> " << newFileName << " " << wasBlocked << std::endl;
		}

		void OnMoveFileWithProgressW(std::wstring_view existingFileName, std::wstring_view newFileName, uint32_t /*flags*/, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "MoveFileWithProgressW: " << converter.to_bytes(existingFileName.data()) << " -> " << converter.to_bytes(newFileName.data()) << " " << wasBlocked << std::endl;
		}

		void OnOpenEncryptedFileRawA(std::string_view fileName, uint32_t /*flags*/, uint32_t /*result*/) override final
		{
			m_stream << "OpenEncryptedFileRawA: " << fileName << std::endl;
		}

		void OnOpenEncryptedFileRawW(std::wstring_view fileName, uint32_t /*flags*/, uint32_t /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "OpenEncryptedFileRawW: " << converter.to_bytes(fileName.data()) << std::endl;
		}

		void OnOpenFile(std::string_view fileName, bool wasBlocked) override final
		{
			m_stream << "OpenFile: " << fileName << " " << wasBlocked << std::endl;
		}

		void OnOpenFileById() override final
		{
			m_stream << "OpenFileById: " << std::endl;
		}

		void OnReadEncryptedFileRaw(uint32_t /*result*/) override final
		{
			m_stream << "ReadEncryptedFileRaw: " << std::endl;
		}

		void OnRemoveDirectoryTransactedA(std::string_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "RemoveDirectoryTransactedA: " << pathName << " " << wasBlocked << std::endl;
		}

		void OnRemoveDirectoryTransactedW(std::wstring_view pathName, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "RemoveDirectoryTransactedW: " << converter.to_bytes(pathName.data()) << " " << wasBlocked << std::endl;
		}

		void OnReOpenFile() override final
		{
			m_stream << "ReOpenFile: " << std::endl;
		}

		void OnReplaceFileA(std::string_view replacedFileName, std::string_view replacementFileName, std::string_view backupFileName, uint32_t /*replaceFlags*/, bool /*result*/) override final
		{
			m_stream << "ReplaceFileA: " << replacedFileName << " - " << replacementFileName << " - " << backupFileName << std::endl;
		}

		void OnReplaceFileW(std::wstring_view replacedFileName, std::wstring_view replacementFileName, std::wstring_view backupFileName, uint32_t /*replaceFlags*/, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "ReplaceFileW: " << converter.to_bytes(replacedFileName.data()) << " - " << converter.to_bytes(replacementFileName.data()) << " - " << converter.to_bytes(backupFileName.data()) << std::endl;
		}

		void OnSetCurrentDirectoryA(std::string_view pathName, bool /*result*/) override final
		{
			m_stream << "SetCurrentDirectoryA: " << pathName << std::endl;
		}

		void OnSetCurrentDirectoryW(std::wstring_view pathName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetCurrentDirectoryW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnSetDllDirectoryA(std::string_view pathName, bool /*result*/) override final
		{
			m_stream << "SetDllDirectoryA: " << pathName << std::endl;
		}

		void OnSetDllDirectoryW(std::wstring_view pathName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetDllDirectoryW: " << converter.to_bytes(pathName.data()) << std::endl;
		}

		void OnSetFileAttributesTransactedA(std::string_view pathName, uint32_t /*fileAttributes*/, bool /*result*/, bool wasBlocked) override final
		{
			m_stream << "SetFileAttributesTransactedA: " << pathName << " " << wasBlocked << std::endl;
		}

		void OnSetFileAttributesTransactedW(std::wstring_view pathName, uint32_t /*fileAttributes*/, bool /*result*/, bool wasBlocked) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetFileAttributesTransactedW: " << converter.to_bytes(pathName.data()) << " " << wasBlocked << std::endl;
		}

		void OnSetFileBandwidthReservation(bool /*result*/) override final
		{
			m_stream << "SetFileBandwidthReservation: " << std::endl;
		}

		void OnSetFileCompletionNotificationModes(bool /*result*/) override final
		{
			m_stream << "SetFileCompletionNotificationModes: " << std::endl;
		}

		void OnSetFileSecurityA(std::string_view fileName, bool /*result*/) override final
		{
			m_stream << "SetFileSecurityA: " << fileName <<  std::endl;
		}

		void OnSetFileShortNameA(std::string_view shortName, bool /*result*/) override final
		{
			m_stream << "SetFileShortNameA: " << shortName << std::endl;
		}

		void OnSetFileShortNameW(std::wstring_view shortName, bool /*result*/) override final
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			m_stream << "SetFileShortNameW: " << converter.to_bytes(shortName.data()) << std::endl;
		}

		void OnSetSearchPathMode(uint32_t /*flags*/, bool /*result*/) override final
		{
			m_stream << "SetSearchPathMode: " << std::endl;
		}

		void OnWriteEncryptedFileRaw(uint32_t /*result*/) override final
		{
			m_stream << "WriteEncryptedFileRaw: " << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
