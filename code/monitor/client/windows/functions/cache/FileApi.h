#pragma once

namespace Monitor::Windows::Functions::Cache::FileApi
{
	/// <summary>
	/// Determines whether the file I/O functions are using the ANSI or OEM character set code page.
	/// </summary>
	BOOL (WINAPI* AreFileApisANSI)() = ::AreFileApisANSI;

	/// <summary>
	/// Compares two file times.
	/// </summary>
	LONG (WINAPI* CompareFileTime)(
		const FILETIME *lpFileTime1,
		const FILETIME *lpFileTime2) = ::CompareFileTime;

	/// <summary>
	/// Creates a new directory.
	/// </summary>
	BOOL (WINAPI* CreateDirectoryA)(
		LPCSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateDirectoryA;
	BOOL (WINAPI* CreateDirectoryW)(
		LPCWSTR lpPathName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateDirectoryW;

	/// <summary>
	/// Creates or opens a file or I/O device.
	/// </summary>
	HANDLE (WINAPI* CreateFile2)(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwCreationDisposition,
		LPCREATEFILE2_EXTENDED_PARAMETERS pCreateExParams) = ::CreateFile2;

	/// <summary>
	/// Creates or opens a file or I/O device. The most commonly used I/O devices are as follows: 
	/// file, file stream, directory, physical disk, volume, console buffer, tape drive, 
	/// communications resource, mailslot, and pipe.
	/// </summary>
	HANDLE (WINAPI* CreateFileA)(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile) = ::CreateFileA;
	HANDLE (WINAPI* CreateFileW)(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile) = ::CreateFileW;

	/// <summary>
	/// Defines, redefines, or deletes MS-DOS device names.
	/// </summary>
	BOOL (WINAPI* DefineDosDeviceW)(
		DWORD dwFlags,
		LPCWSTR lpDeviceName,
		LPCWSTR lpTargetPath) = ::DefineDosDeviceW;

	/// <summary>
	/// Deletes an existing file.
	/// </summary>
	BOOL (WINAPI* DeleteFileA)(
		LPCSTR lpFileName) = ::DeleteFileA;
	BOOL (WINAPI* DeleteFileW)(
		LPCWSTR lpFileName) = ::DeleteFileW;

	/// <summary>
	/// Deletes a drive letter or mounted folder.
	/// </summary>
	BOOL (WINAPI* DeleteVolumeMountPointW)(
		LPCWSTR lpszVolumeMountPoint) = ::DeleteVolumeMountPointW;

	/// <summary>
	/// Converts a file time to a local file time.
	/// </summary>
	BOOL (WINAPI* FileTimeToLocalFileTime)(
		const FILETIME *lpFileTime,
		LPFILETIME lpLocalFileTime) = ::FileTimeToLocalFileTime;

	/// <summary>
	/// Closes a file search handle opened by the FindFirstFile, FindFirstFileEx, FindFirstFileNameW,
	/// FindFirstFileNameTransactedW, FindFirstFileTransacted, FindFirstStreamTransactedW, or FindFirstStreamW functions.
	/// </summary>
	BOOL (WINAPI* FindClose)(
		HANDLE hFindFile) = ::FindClose;

	/// <summary>
	/// Stops change notification handle monitoring.
	/// </summary>
	BOOL (WINAPI* FindCloseChangeNotification)(
		HANDLE hChangeHandle) = ::FindCloseChangeNotification;

	/// <summary>
	/// Creates a change notification handle and sets up initial change notification filter conditions.
	/// </summary>
	HANDLE (WINAPI* FindFirstChangeNotificationA)(
		LPCSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD dwNotifyFilter) = ::FindFirstChangeNotificationA;
	HANDLE (WINAPI* FindFirstChangeNotificationW)(
		LPCWSTR lpPathName,
		BOOL bWatchSubtree,
		DWORD ) = ::FindFirstChangeNotificationW;

	/// <summary>
	/// Searches a directory for a file or subdirectory with a name that matches a specific name (or partial name if wildcards are used).
	/// </summary>
	HANDLE (WINAPI* FindFirstFileA)(
		LPCSTR lpFileName,
		LPWIN32_FIND_DATAA lpFindFileData) = ::FindFirstFileA;
	HANDLE (WINAPI* FindFirstFileW)(
		LPCWSTR lpFileName,
		LPWIN32_FIND_DATAW lpFindFileData) = ::FindFirstFileW;

	/// <summary>
	/// Searches a directory for a file or subdirectory with a name and attributes that match those specified.
	/// </summary>
	HANDLE (WINAPI* FindFirstFileExA)(
		LPCSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags) = ::FindFirstFileExA;
	HANDLE (WINAPI* FindFirstFileExW)(
		LPCWSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags) = ::FindFirstFileExW;

	/// <summary>
	/// Creates an enumeration of all the hard links to the specified file. The FindFirstFileNameW function returns
	/// a handle to the enumeration that can be used on subsequent calls to the FindNextFileNameW function.
	/// </summary>
	HANDLE (WINAPI* FindFirstFileNameW)(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName) = ::FindFirstFileNameW;

	/// <summary>
	/// Enumerates the first stream with a ::$DATA stream type in the specified file or directory.
	/// </summary>
	HANDLE (WINAPI* FindFirstStreamW)(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags) = ::FindFirstStreamW;

	/// <summary>
	/// Retrieves the name of a volume on a computer.
	/// </summary>
	HANDLE (WINAPI* FindFirstVolumeW)(
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength) = ::FindFirstVolumeW;

	/// <summary>
	/// Requests that the operating system signal a change notification handle the next time it detects an appropriate change.
	/// </summary>
	BOOL (WINAPI* FindNextChangeNotification)(
		HANDLE hChangeHandle) = ::FindNextChangeNotification;

	/// <summary>
	/// Continues a file search from a previous call to the FindFirstFile, FindFirstFileEx, or FindFirstFileTransacted functions.
	/// </summary>
	BOOL (WINAPI* FindNextFileA)(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAA lpFindFileData) = ::FindNextFileA;
	BOOL (WINAPI* FindNextFileW)(
		HANDLE hFindFile,
		LPWIN32_FIND_DATAW lpFindFileData) = ::FindNextFileW;

	/// <summary>
	/// Continues enumerating the hard links to a file using the handle returned by a successful call to the
	/// FindFirstFileNameW function.
	/// </summary>
	BOOL (WINAPI* FindNextFileNameW)(
		HANDLE hFindStream,
		LPDWORD StringLength,
		PWSTR LinkName) = ::FindNextFileNameW;

	/// <summary>
	/// Continues a stream search started by a previous call to the FindFirstStreamW function.
	/// </summary>
	BOOL (WINAPI* FindNextStreamW)(
		HANDLE hFindStream,
		LPVOID lpFindStreamData) = ::FindNextStreamW;

	/// <summary>
	/// Continues a volume search started by a call to the FindFirstVolume function.
	/// </summary>
	BOOL (WINAPI* FindNextVolumeW)(
		HANDLE hFindVolume,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength) = ::FindNextVolumeW;

	/// <summary>
	/// Closes the specified volume search handle.
	/// </summary>
	BOOL (WINAPI* FindVolumeClose)(
		HANDLE hFindVolume) = ::FindVolumeClose;

	/// <summary>
	/// Flushes the buffers of a specified file and causes all buffered data to be written to a file.
	/// </summary>
	BOOL (WINAPI* FlushFileBuffers)(
		HANDLE hFile) = ::FlushFileBuffers;

	/// <summary>
	/// Retrieves the actual number of bytes of disk storage used to store a specified file.
	/// </summary>
	DWORD (WINAPI* GetCompressedFileSizeA)(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh) = ::GetCompressedFileSizeA;
	DWORD (WINAPI* GetCompressedFileSizeW)(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh) = ::GetCompressedFileSizeW;

	/// <summary>
	/// Retrieves information about the specified disk, including the amount of free space on the disk.
	/// </summary>
	BOOL (WINAPI* GetDiskFreeSpaceA)(
		LPCSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters) = ::GetDiskFreeSpaceA;
	BOOL (WINAPI* GetDiskFreeSpaceW)(
		LPCWSTR lpRootPathName,
		LPDWORD lpSectorsPerCluster,
		LPDWORD lpBytesPerSector,
		LPDWORD lpNumberOfFreeClusters,
		LPDWORD lpTotalNumberOfClusters) = ::GetDiskFreeSpaceW;

	/// <summary>
	/// Retrieves information about the amount of space that is available on a disk volume, which is the
	/// total amount of space, the total amount of free space, and the total amount of free space available
	/// to the user that is associated with the calling thread.
	/// </summary>
	BOOL (WINAPI* GetDiskFreeSpaceExA)(
		LPCSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes) = ::GetDiskFreeSpaceExA;
	BOOL (WINAPI* GetDiskFreeSpaceExW)(
		LPCWSTR lpDirectoryName,
		PULARGE_INTEGER lpFreeBytesAvailableToCaller,
		PULARGE_INTEGER lpTotalNumberOfBytes,
		PULARGE_INTEGER lpTotalNumberOfFreeBytes) = ::GetDiskFreeSpaceExW;

	/// <summary>
	/// Determines whether a disk drive is a removable, fixed, CD-ROM, RAM disk, or network drive.
	/// </summary>
	UINT (WINAPI* GetDriveTypeA)(
		LPCSTR lpRootPathName) = ::GetDriveTypeA;
	UINT (WINAPI* GetDriveTypeW)(
		LPCWSTR lpRootPathName) = ::GetDriveTypeW;

	/// <summary>
	/// Retrieves file system attributes for a specified file or directory.
	/// </summary>
	DWORD (WINAPI* GetFileAttributesA)(
		LPCSTR lpFileName) = ::GetFileAttributesA;
	DWORD (WINAPI* GetFileAttributesW)(
		LPCWSTR lpFileName) = ::GetFileAttributesW;

	/// <summary>
	/// Retrieves attributes for a specified file or directory.
	/// </summary>
	BOOL (WINAPI* GetFileAttributesExA)(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation) = ::GetFileAttributesExA;
	BOOL (WINAPI* GetFileAttributesExW)(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation) = ::GetFileAttributesExW;

	/// <summary>
	/// Retrieves file information for the specified file.
	/// </summary>
	BOOL (WINAPI* GetFileInformationByHandle)(
		HANDLE hFile,
		LPBY_HANDLE_FILE_INFORMATION lpFileInformation) = ::GetFileInformationByHandle;

	/// <summary>
	/// Retrieves the size of the specified file, in bytes.
	/// </summary>
	DWORD (WINAPI* GetFileSize)(
		HANDLE hFile,
		LPDWORD lpFileSizeHigh) = ::GetFileSize;

	/// <summary>
	/// Retrieves the size of the specified file.
	/// </summary>
	BOOL (WINAPI* GetFileSizeEx)(
		HANDLE hFile,
		PLARGE_INTEGER lpFileSize) = ::GetFileSizeEx;

	/// <summary>
	/// Retrieves the date and time that a file or directory was created, last accessed, and last modified.
	/// </summary>
	BOOL (WINAPI* GetFileTime)(
		HANDLE hFile,
		LPFILETIME lpCreationTime,
		LPFILETIME lpLastAccessTime,
		LPFILETIME lpLastWriteTime) = ::GetFileTime;

	/// <summary>
	/// Retrieves the file type of the specified file.
	/// </summary>
	DWORD (WINAPI* GetFileType)(
		HANDLE hFile) = ::GetFileType;

	/// <summary>
	/// Retrieves the final path for the specified file.
	/// </summary>
	DWORD (WINAPI* GetFinalPathNameByHandleA)(
		HANDLE hFile,
		LPSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags) = ::GetFinalPathNameByHandleA;
	DWORD (WINAPI* GetFinalPathNameByHandleW)(
		HANDLE hFile,
		LPWSTR lpszFilePath,
		DWORD cchFilePath,
		DWORD dwFlags) = ::GetFinalPathNameByHandleW;

	/// <summary>
	/// Retrieves the full path and file name of the specified file.
	/// </summary>
	DWORD (WINAPI* GetFullPathNameA)(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart) = ::GetFullPathNameA;
	DWORD (WINAPI* GetFullPathNameW)(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart) = ::GetFullPathNameW;

	/// <summary>
	/// Retrieves a bitmask representing the currently available disk drives.
	/// </summary>
	DWORD (WINAPI* GetLogicalDrives)() = ::GetLogicalDrives;
	
	/// <summary>
	/// Fills a buffer with strings that specify valid drives in the system.
	/// </summary>
	DWORD (WINAPI* GetLogicalDriveStringsW)(
		DWORD nBufferLength,
		LPWSTR lpBuffer) = ::GetLogicalDriveStringsW;

	/// <summary>
	/// Converts the specified path to its long form.
	/// </summary>
	DWORD (WINAPI* GetLongPathNameA)(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer) = ::GetLongPathNameA;
	DWORD (WINAPI* GetLongPathNameW)(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer) = ::GetLongPathNameW;

	/// <summary>
	/// Retrieves the short path form of the specified path.
	/// </summary>
	DWORD (WINAPI* GetShortPathNameW)(
		LPCWSTR lpszLongPath,
		LPWSTR lpszShortPath,
		DWORD cchBuffer) = ::GetShortPathNameW;

	/// <summary>
	/// Creates a name for a temporary file. If a unique file name is generated, an empty
	/// file is created and the handle to it is released; otherwise, only a file name is generated.
	/// </summary>
	UINT (WINAPI* GetTempFileNameA)(
		LPCSTR lpPathName,
		LPCSTR lpPrefixString,
		UINT uUnique,
		LPSTR lpTempFileName) = ::GetTempFileNameA;
	UINT (WINAPI* GetTempFileNameW)(
		LPCWSTR lpPathName,
		LPCWSTR lpPrefixString,
		UINT uUnique,
		LPWSTR lpTempFileName) = ::GetTempFileNameW;

	/// <summary>
	/// Retrieves the path of the directory designated for temporary files.
	/// </summary>
	DWORD (WINAPI* GetTempPathA)(
		DWORD nBufferLength,
		LPSTR lpBuffer) = ::GetTempPathA;
	DWORD (WINAPI* GetTempPathW)(
		DWORD nBufferLength,
		LPWSTR lpBuffer) = ::GetTempPathW;

	/// <summary>
	/// Retrieves information about the file system and volume associated with the specified root directory.
	/// </summary>
	BOOL (WINAPI* GetVolumeInformationA)(
		LPCSTR lpRootPathName,
		LPSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize) = ::GetVolumeInformationA;
	BOOL (WINAPI* GetVolumeInformationW)(
		LPCWSTR lpRootPathName,
		LPWSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPWSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize) = ::GetVolumeInformationW;

	/// <summary>
	/// Retrieves information about the file system and volume associated with the specified file.
	/// </summary>
	BOOL (WINAPI* GetVolumeInformationByHandleW)(
		HANDLE hFile,
		LPWSTR lpVolumeNameBuffer,
		DWORD nVolumeNameSize,
		LPDWORD lpVolumeSerialNumber,
		LPDWORD lpMaximumComponentLength,
		LPDWORD lpFileSystemFlags,
		LPWSTR lpFileSystemNameBuffer,
		DWORD nFileSystemNameSize) = ::GetVolumeInformationByHandleW;

	/// <summary>
	/// Retrieves a volume GUID path for the volume that is associated with the specified volume mount point
	/// (drive letter, volume GUID path, or mounted folder).
	/// </summary>
	BOOL (WINAPI* GetVolumeNameForVolumeMountPointW)(
		LPCWSTR lpszVolumeMountPoint,
		LPWSTR lpszVolumeName,
		DWORD cchBufferLength) = ::GetVolumeNameForVolumeMountPointW;

	/// <summary>
	/// Retrieves a list of drive letters and mounted folder paths for the specified volume.
	/// </summary>
	BOOL (WINAPI* GetVolumePathNamesForVolumeNameW)(
		LPCWSTR lpszVolumeName,
		LPWCH lpszVolumePathNames,
		DWORD cchBufferLength,
		PDWORD lpcchReturnLength) = ::GetVolumePathNamesForVolumeNameW;

	/// <summary>
	/// Retrieves the volume mount point where the specified path is mounted.
	/// </summary>
	BOOL (WINAPI* GetVolumePathNameW)(
		LPCWSTR lpszFileName,
		LPWSTR lpszVolumePathName,
		DWORD cchBufferLength) = ::GetVolumePathNameW;

	/// <summary>
	/// Converts a local file time to a file time based on the Coordinated Universal Time (UTC).
	/// </summary>
	BOOL (WINAPI* LocalFileTimeToFileTime)(
		const FILETIME *lpLocalFileTime,
		LPFILETIME lpFileTime) = ::LocalFileTimeToFileTime;

	/// <summary>
	/// Locks the specified file for exclusive access by the calling process.
	/// </summary>
	BOOL (WINAPI* LockFile)(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToLockLow,
		DWORD nNumberOfBytesToLockHigh) = ::LockFile;

	/// <summary>
	/// Locks the specified file for exclusive access by the calling process. This function can operate
	/// either synchronously or asynchronously and can request either an exclusive or a shared lock.
	/// </summary>
	BOOL (WINAPI* LockFileEx)(
		HANDLE hFile,
		DWORD dwFlags,
		DWORD dwReserved,
		DWORD nNumberOfBytesToLockLow,
		DWORD nNumberOfBytesToLockHigh,
		LPOVERLAPPED lpOverlapped) = ::LockFileEx;

	/// <summary>
	/// Retrieves information about MS-DOS device names.
	/// </summary>
	DWORD (WINAPI* QueryDosDeviceW)(
		LPCWSTR lpDeviceName,
		LPWSTR lpTargetPath,
		DWORD ucchMax) = ::QueryDosDeviceW;
 	
	/// <summary>
	/// Reads data from the specified file or input/output (I/O) device. Reads occur at the position
	/// specified by the file pointer if supported by the device.
	/// </summary>
	BOOL (WINAPI* ReadFile)(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPDWORD lpNumberOfBytesRead,
		LPOVERLAPPED lpOverlapped) = ::ReadFile;

	/// <summary>
	/// Reads data from the specified file or input/output (I/O) device. It reports its completion
	/// status asynchronously, calling the specified completion routine when reading is completed
	/// or canceled and the calling thread is in an alertable wait state.
	/// </summary>
	BOOL (WINAPI* ReadFileEx)(
		HANDLE hFile,
		LPVOID lpBuffer,
		DWORD nNumberOfBytesToRead,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) = ::ReadFileEx;

	/// <summary>
	/// Reads data from a file and stores it in an array of buffers.
	/// </summary>
	BOOL (WINAPI* ReadFileScatter)(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToRead,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped) = ::ReadFileScatter;

	/// <summary>
	/// Deletes an existing empty directory.
	/// </summary>
	BOOL (WINAPI* RemoveDirectoryA)(
		LPCSTR lpPathName) = ::RemoveDirectoryA;
	BOOL (WINAPI* RemoveDirectoryW)(
		LPCWSTR lpPathName) = ::RemoveDirectoryW;

	/// <summary>
	/// Sets the physical file size for the specified file to the current position of the file pointer.
	/// </summary>
	BOOL (WINAPI* SetEndOfFile)(
		HANDLE hFile) = ::SetEndOfFile;

	/// <summary>
	/// Causes the file I/O functions to use the ANSI character set code page for the current process.
	/// </summary>
	void (WINAPI* SetFileApisToANSI)() = ::SetFileApisToANSI;

	/// <summary>
	/// Causes the file I/O functions for the process to use the OEM character set code page.
	/// </summary>
	void (WINAPI* SetFileApisToOEM)() = ::SetFileApisToOEM;

	/// <summary>
	/// Sets the attributes for a file or directory.
	/// </summary>
	BOOL (WINAPI* SetFileAttributesA)(
		LPCSTR lpFileName,
		DWORD  dwFileAttributes) = ::SetFileAttributesA;
	BOOL (WINAPI* SetFileAttributesW)(
		LPCWSTR lpFileName,
		DWORD  dwFileAttributes) = ::SetFileAttributesW;

	/// <summary>
	/// Sets the file information for the specified file.
	/// </summary>
	BOOL (WINAPI* SetFileInformationByHandle)(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize) = ::SetFileInformationByHandle;

	/// <summary>
	/// Associates a virtual address range with the specified file handle.
	/// </summary>
	BOOL (WINAPI* SetFileIoOverlappedRange)(
		HANDLE FileHandle,
		PUCHAR OverlappedRangeStart,
		ULONG Length) = ::SetFileIoOverlappedRange;

	/// <summary>
	/// Moves the file pointer of the specified file.
	/// </summary>
	DWORD (WINAPI* SetFilePointer)(
		HANDLE hFile,
		LONG lDistanceToMove,
		PLONG lpDistanceToMoveHigh,
		DWORD dwMoveMethod) = ::SetFilePointer;

	/// <summary>
	/// Moves the file pointer of the specified file.
	/// </summary>
	BOOL (WINAPI* SetFilePointerEx)(
		HANDLE hFile,
		LARGE_INTEGER liDistanceToMove,
		PLARGE_INTEGER lpNewFilePointer,
		DWORD dwMoveMethod) = ::SetFilePointerEx;

	/// <summary>
	/// Sets the date and time that the specified file or directory was created, last accessed, or last modified.
	/// </summary>
	BOOL (WINAPI* SetFileTime)(
		HANDLE hFile,
		const FILETIME *lpCreationTime,
		const FILETIME *lpLastAccessTime,
		const FILETIME *lpLastWriteTime) = ::SetFileTime;

	/// <summary>
	/// Sets the valid data length of the specified file. This function is useful in very limited scenarios.
	/// For more information, see the Remarks section.
	/// </summary>
	BOOL (WINAPI* SetFileValidData)(
		HANDLE   hFile,
		LONGLONG ValidDataLength) = ::SetFileValidData;

	/// <summary>
	/// Unlocks a region in an open file.
	/// </summary>
	BOOL (WINAPI* UnlockFile)(
		HANDLE hFile,
		DWORD dwFileOffsetLow,
		DWORD dwFileOffsetHigh,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh) = ::UnlockFile;

	/// <summary>
	/// Unlocks a region in the specified file. This function can operate either synchronously or asynchronously.
	/// </summary>
	BOOL (WINAPI* UnlockFileEx)(
		HANDLE hFile,
		DWORD dwReserved,
		DWORD nNumberOfBytesToUnlockLow,
		DWORD nNumberOfBytesToUnlockHigh,
		LPOVERLAPPED lpOverlapped) = ::UnlockFileEx;

	/// <summary>
	/// Writes data to the specified file or input/output (I/O) device.
	/// </summary>
	BOOL (WINAPI* WriteFile)(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpNumberOfBytesWritten,
		LPOVERLAPPED lpOverlapped) = ::WriteFile;

	/// <summary>
	/// Writes data to the specified file or input/output (I/O) device. It reports its completion
	/// status asynchronously, calling the specified completion routine when writing is completed
	/// or canceled and the calling thread is in an alertable wait state.
	/// </summary>
	BOOL (WINAPI* WriteFileEx)(
		HANDLE hFile,
		LPCVOID lpBuffer,
		DWORD nNumberOfBytesToWrite,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) = ::WriteFileEx;

	/// <summary>
	/// Retrieves data from an array of buffers and writes the data to a file.
	/// </summary>
	BOOL (WINAPI* WriteFileGather)(
		HANDLE hFile,
		FILE_SEGMENT_ELEMENT aSegmentArray[],
		DWORD nNumberOfBytesToWrite,
		LPDWORD lpReserved,
		LPOVERLAPPED lpOverlapped) = ::WriteFileGather;
}