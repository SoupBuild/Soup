#pragma once

namespace Functions::WinBase::Cache
{
	/// <summary>
	/// Copies an existing file to a new file.
	/// </summary>
	BOOL (WINAPI* CopyFileA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		BOOL bFailIfExists) = ::CopyFileA;
	BOOL (WINAPI* CopyFileW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		BOOL bFailIfExists) = ::CopyFileW;

	/// <summary>
	/// Copies an existing file to a new file, notifying the application of its progress through a callback function.
	/// </summary>
	HRESULT (WINAPI* CopyFile2)(
		PCWSTR pwszExistingFileName,
		PCWSTR pwszNewFileName,
		COPYFILE2_EXTENDED_PARAMETERS *pExtendedParameters) = ::CopyFile2;

	/// <summary>
	/// Copies an existing file to a new file, notifying the application of its progress
	/// through a callback function.
	/// </summary>
	BOOL (WINAPI* CopyFileExA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags) = ::CopyFileExA;
	BOOL (WINAPI* CopyFileExW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags) = ::CopyFileExW;

	/// <summary>
	/// Copies an existing file to a new file as a transacted operation, notifying the
	/// application of its progress through a callback function.
	/// </summary>
	BOOL (WINAPI* CopyFileTransactedA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags,
		HANDLE hTransaction) = ::CopyFileTransactedA;
	BOOL (WINAPI* CopyFileTransactedW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		LPBOOL pbCancel,
		DWORD dwCopyFlags,
		HANDLE hTransaction) = ::CopyFileTransactedW;

	/// <summary>
	/// Creates a new directory.
	/// </summary>
	BOOL (WINAPI* CreateDirectoryExA)(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateDirectoryExA;
	BOOL (WINAPI* CreateDirectoryExW)(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateDirectoryExW;

	/// <summary>
	/// Creates a new directory as a transacted operation, with the
	/// attributes of a specified template directory.
	/// </summary>
	BOOL (WINAPI* CreateDirectoryTransactedA)(
		LPCSTR lpTemplateDirectory,
		LPCSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction) = ::CreateDirectoryTransactedA;
	BOOL (WINAPI* CreateDirectoryTransactedW)(
		LPCWSTR lpTemplateDirectory,
		LPCWSTR lpNewDirectory,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction) = ::CreateDirectoryTransactedW;

	/// <summary>
	/// Creates or opens a file, file stream, or directory as a transacted operation.
	/// </summary>
	HANDLE (WINAPI* CreateFileTransactedA)(
		LPCSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile,
		HANDLE hTransaction,
		PUSHORT pusMiniVersion,
		PVOID lpExtendedParameter) = ::CreateFileTransactedA;
	HANDLE (WINAPI* CreateFileTransactedW)(
		LPCWSTR lpFileName,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwCreationDisposition,
		DWORD dwFlagsAndAttributes,
		HANDLE hTemplateFile,
		HANDLE hTransaction,
		PUSHORT pusMiniVersion,
		PVOID lpExtendedParameter) = ::CreateFileTransactedW;

	/// <summary>
	/// Establishes a hard link between an existing file and a new file.
	/// </summary>
	BOOL (WINAPI* CreateHardLinkA)(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateHardLinkA;
	BOOL (WINAPI* CreateHardLinkW)(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes) = ::CreateHardLinkW;

	/// <summary>
	/// Establishes a hard link between an existing file and a new file as a transacted operation.
	/// </summary>
	BOOL (WINAPI* CreateHardLinkTransactedA)(
		LPCSTR lpFileName,
		LPCSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction) = ::CreateHardLinkTransactedA;
	BOOL (WINAPI* CreateHardLinkTransactedW)(
		LPCWSTR lpFileName,
		LPCWSTR lpExistingFileName,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		HANDLE hTransaction) = ::CreateHardLinkTransactedW;

	/// <summary>
	/// Creates a new process and its primary thread. Then the new process runs the specified
	/// executable file in the security context of the specified credentials (user, domain, and password).
	/// It can optionally load the user profile for a specified user.
	/// </summary>
	BOOL (WINAPI* CreateProcessWithLogonW)(
		LPCWSTR lpUsername,
		LPCWSTR lpDomain,
		LPCWSTR lpPassword,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessWithLogonW;

	/// <summary>
	/// Creates a new process and its primary thread. The new process runs in the security context of
	/// the specified token. It can optionally load the user profile for the specified user.
	/// </summary>
	BOOL (WINAPI* CreateProcessWithTokenW)(
		HANDLE hToken,
		DWORD dwLogonFlags,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessWithTokenW;

	/// <summary>
	/// Creates a symbolic link.
	/// </summary>
	BOOLEAN (WINAPI* CreateSymbolicLinkA)(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags) = ::CreateSymbolicLinkA;
	BOOLEAN (WINAPI* CreateSymbolicLinkW)(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags) = ::CreateSymbolicLinkW;

	/// <summary>
	/// Creates a symbolic link as a transacted operation.
	/// </summary>
	BOOLEAN (WINAPI* CreateSymbolicLinkTransactedA)(
		LPCSTR lpSymlinkFileName,
		LPCSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction) = ::CreateSymbolicLinkTransactedA;
	BOOLEAN (WINAPI* CreateSymbolicLinkTransactedW)(
		LPCWSTR lpSymlinkFileName,
		LPCWSTR lpTargetFileName,
		DWORD dwFlags,
		HANDLE hTransaction) = ::CreateSymbolicLinkTransactedW;

	/// <summary>
	/// Decrypts an encrypted file or directory.
	/// </summary>
	BOOL (WINAPI* DecryptFileA)(
		LPCSTR lpFileName,
		DWORD dwReserved) = ::DecryptFileA;
	BOOL (WINAPI* DecryptFileW)(
		LPCWSTR lpFileName,
		DWORD dwReserved) = ::DecryptFileW;

	/// <summary>
	/// Deletes an existing file.
	/// </summary>
	BOOL (WINAPI* DeleteFile)(
		LPCSTR lpFileName) = ::DeleteFile;

	/// <summary>
	/// Deletes an existing file as a transacted operation.
	/// </summary>
	BOOL (WINAPI* DeleteFileTransactedA)(
		LPCSTR lpFileName,
		HANDLE hTransaction) = ::DeleteFileTransactedA;
	BOOL (WINAPI* DeleteFileTransactedW)(
		LPCWSTR lpFileName,
		HANDLE hTransaction) = ::DeleteFileTransactedW;

	/// <summary>
	/// Encrypts a file or directory.
	/// </summary>
	BOOL (WINAPI* EncryptFileA)(
		LPCSTR lpFileName) = ::EncryptFileA;
	BOOL (WINAPI* EncryptFileW)(
		LPCWSTR lpFileName) = ::EncryptFileW;

	/// <summary>
	/// Retrieves the encryption status of the specified file.
	/// </summary>
	BOOL (WINAPI* FileEncryptionStatusA)(
		LPCSTR lpFileName,
		LPDWORD lpStatus) = ::FileEncryptionStatusA;
	BOOL (WINAPI* FileEncryptionStatusW)(
		LPCWSTR lpFileName,
		LPDWORD lpStatus) = ::FileEncryptionStatusW;

	/// <summary>
	/// Creates an enumeration of all the hard links to the specified file as a transacted operation.
	/// The function returns a handle to the enumeration that can be used on subsequent calls to the FindNextFileNameW function.
	/// </summary>
	HANDLE (WINAPI* FindFirstFileNameTransactedW)(
		LPCWSTR lpFileName,
		DWORD dwFlags,
		LPDWORD StringLength,
		PWSTR LinkName,
		HANDLE hTransaction) = ::FindFirstFileNameTransactedW;

	/// <summary>
	/// Searches a directory for a file or subdirectory with a name that matches a specific name as a transacted operation.
	/// </summary>
	HANDLE (WINAPI* FindFirstFileTransactedA)(
		LPCSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags,
		HANDLE hTransaction) = ::FindFirstFileTransactedA;
	HANDLE (WINAPI* FindFirstFileTransactedW)(
		LPCWSTR lpFileName,
		FINDEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFindFileData,
		FINDEX_SEARCH_OPS fSearchOp,
		LPVOID lpSearchFilter,
		DWORD dwAdditionalFlags,
		HANDLE hTransaction) = ::FindFirstFileTransactedW;

	/// <summary>
	/// Enumerates the first stream in the specified file or directory as a transacted operation.
	/// </summary>
	HANDLE (WINAPI* FindFirstStreamTransactedW)(
		LPCWSTR lpFileName,
		STREAM_INFO_LEVELS InfoLevel,
		LPVOID lpFindStreamData,
		DWORD dwFlags,
		HANDLE hTransaction) = ::FindFirstStreamTransactedW;

	/// <summary>
	/// Determines whether a file is an executable (.exe) file, and if so, which subsystem runs the executable file.
	/// </summary>
	BOOL (WINAPI* GetBinaryTypeA)(
		LPCSTR lpApplicationName,
		LPDWORD lpBinaryType) = ::GetBinaryTypeA;
	BOOL (WINAPI* GetBinaryTypeW)(
		LPCWSTR lpApplicationName,
		LPDWORD lpBinaryType) = ::GetBinaryTypeW;

	/// <summary>
	/// Retrieves the actual number of bytes of disk storage used to store a specified file as a transacted operation.
	/// </summary>
	DWORD (WINAPI* GetCompressedFileSizeTransactedA)(
		LPCSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction) = ::GetCompressedFileSizeTransactedA;
	DWORD (WINAPI* GetCompressedFileSizeTransactedW)(
		LPCWSTR lpFileName,
		LPDWORD lpFileSizeHigh,
		HANDLE hTransaction) = ::GetCompressedFileSizeTransactedW;

	/// <summary>
	/// Retrieves the application-specific portion of the search path used to locate DLLs for the application.
	/// </summary>
	DWORD (WINAPI* GetDllDirectoryA)(
		DWORD nBufferLength,
		LPSTR lpBuffer) = ::GetDllDirectoryA;
	DWORD (WINAPI* GetDllDirectoryW)(
		DWORD nBufferLength,
		LPWSTR lpBuffer) = ::GetDllDirectoryW;

	/// <summary>
	/// Retrieves file system attributes for a specified file or directory as a transacted operation.
	/// </summary>
	BOOL (WINAPI* GetFileAttributesTransactedA)(
		LPCSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction) = ::GetFileAttributesTransactedA;
	BOOL (WINAPI* GetFileAttributesTransactedW)(
		LPCWSTR lpFileName,
		GET_FILEEX_INFO_LEVELS fInfoLevelId,
		LPVOID lpFileInformation,
		HANDLE hTransaction) = ::GetFileAttributesTransactedW;

	/// <summary>
	/// Retrieves the bandwidth reservation properties of the volume on which the specified file resides.
	/// </summary>
	BOOL (WINAPI* GetFileBandwidthReservation)(
		HANDLE hFile,
		LPDWORD lpPeriodMilliseconds,
		LPDWORD lpBytesPerPeriod,
		LPBOOL pDiscardable,
		LPDWORD lpTransferSize,
		LPDWORD lpNumOutstandingRequests) = ::GetFileBandwidthReservation;

	/// <summary>
	/// Retrieves file information for the specified file.
	/// </summary>
	BOOL (WINAPI* GetFileInformationByHandleEx)(
		HANDLE hFile,
		FILE_INFO_BY_HANDLE_CLASS FileInformationClass,
		LPVOID lpFileInformation,
		DWORD dwBufferSize) = ::GetFileInformationByHandleEx;

	/// <summary>
	/// Obtains specified information about the security of a file or directory. The information obtained is constrained by the caller's access rights and privileges.
	/// </summary>
	BOOL (WINAPI* GetFileSecurityA)(
		LPCSTR lpFileName,
		SECURITY_INFORMATION RequestedInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor,
		DWORD nLength,
		LPDWORD lpnLengthNeeded) = ::GetFileSecurityA;

	/// <summary>
	/// Retrieves the full path and file name of the specified file as a transacted operation.
	/// </summary>
	DWORD (WINAPI* GetFullPathNameTransactedA)(
		LPCSTR lpFileName,
		DWORD nBufferLength,
		LPSTR lpBuffer,
		LPSTR *lpFilePart,
		HANDLE hTransaction) = ::GetFullPathNameTransactedA;
	DWORD (WINAPI* GetFullPathNameTransactedW)(
		LPCWSTR lpFileName,
		DWORD nBufferLength,
		LPWSTR lpBuffer,
		LPWSTR *lpFilePart,
		HANDLE hTransaction) = ::GetFullPathNameTransactedW;

	/// <summary>
	/// Converts the specified path to its long form as a transacted operation.
	/// </summary>
	DWORD (WINAPI* GetLongPathNameTransactedA)(
		LPCSTR lpszShortPath,
		LPSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction) = ::GetLongPathNameTransactedA;
	DWORD (WINAPI* GetLongPathNameTransactedW)(
		LPCWSTR lpszShortPath,
		LPWSTR lpszLongPath,
		DWORD cchBuffer,
		HANDLE hTransaction) = ::GetLongPathNameTransactedW;

	/// <summary>
	/// Attempts to dequeue an I/O completion packet from the specified I/O completion port.
	/// </summary>
	BOOL (WINAPI* GetQueuedCompletionStatus)(
		HANDLE CompletionPort,
		LPDWORD lpNumberOfBytesTransferred,
		PULONG_PTR lpCompletionKey,
		LPOVERLAPPED *lpOverlapped,
		DWORD dwMilliseconds) = ::GetQueuedCompletionStatus;

	/// <summary>
	/// Retrieves multiple completion port entries simultaneously.
	/// </summary>
	BOOL (WINAPI* GetQueuedCompletionStatusEx)(
		HANDLE CompletionPort,
		LPOVERLAPPED_ENTRY lpCompletionPortEntries,
		ULONG ulCount,
		PULONG ulNumEntriesRemoved,
		DWORD dwMilliseconds,
		BOOL fAlertable) = ::GetQueuedCompletionStatusEx;

	/// <summary>
	/// Retrieves the short path form of the specified path.
	/// </summary>
	DWORD (WINAPI* GetShortPathNameA)(
		LPCSTR lpszLongPath,
		LPSTR lpszShortPath,
		DWORD cchBuffer) = ::GetShortPathNameA;

	/// <summary>
	/// Creates a name for a temporary file. If a unique file name is generated, an empty
	/// file is created and the handle to it is released; otherwise, only a file name is generated.
	/// </summary>
	UINT (WINAPI* GetTempFileName)(
		LPCSTR lpPathName,
		LPCSTR lpPrefixString,
		UINT uUnique,
		LPSTR lpTempFileName) = ::GetTempFileName;

	/// <summary>
	/// Loads and executes an application or creates a new instance of an existing application.
	/// </summary>
	DWORD (WINAPI* LoadModule)(
		LPCSTR lpModuleName,
		LPVOID lpParameterBlock) = ::LoadModule;

	/// <summary>
	/// Loads the specified packaged module and its dependencies into the address space of the calling process.
	/// </summary>
	HMODULE (WINAPI* LoadPackagedLibrary)(
		LPCWSTR lpwLibFileName,
		DWORD Reserved) = ::LoadPackagedLibrary;

	/// <summary>
	/// Moves an existing file or a directory, including its children.
	/// </summary>
	BOOL (WINAPI* MoveFileA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName) = ::MoveFileA;
	BOOL (WINAPI* MoveFileW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName) = ::MoveFileW;

	/// <summary>
	/// Moves an existing file or directory, including its children, with various move options.
	/// </summary>
	BOOL (WINAPI* MoveFileExA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		DWORD dwFlags) = ::MoveFileExA;
	BOOL (WINAPI* MoveFileExW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		DWORD dwFlags) = ::MoveFileExW;

	/// <summary>
	/// Moves an existing file or a directory, including its children, as a transacted operation.
	/// </summary>
	BOOL (WINAPI* MoveFileTransactedA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags,
		HANDLE hTransaction) = ::MoveFileTransactedA;
	BOOL (WINAPI* MoveFileTransactedW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags,
		HANDLE hTransaction) = ::MoveFileTransactedW;

	/// <summary>
	/// Moves a file or directory, including its children. You can provide a callback function that receives progress notifications.
	/// </summary>
	BOOL (WINAPI* MoveFileWithProgressA)(
		LPCSTR lpExistingFileName,
		LPCSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags) = ::MoveFileWithProgressA;
	BOOL (WINAPI* MoveFileWithProgressW)(
		LPCWSTR lpExistingFileName,
		LPCWSTR lpNewFileName,
		LPPROGRESS_ROUTINE lpProgressRoutine,
		LPVOID lpData,
		DWORD dwFlags) = ::MoveFileWithProgressW;

	/// <summary>
	/// Opens an encrypted file in order to backup (export) or restore (import) the file.
	/// </summary>
	DWORD (WINAPI* OpenEncryptedFileRawA)(
		LPCSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext) = ::OpenEncryptedFileRawA;
	DWORD (WINAPI* OpenEncryptedFileRawW)(
		LPCWSTR lpFileName,
		ULONG ulFlags,
		PVOID *pvContext) = ::OpenEncryptedFileRawW;

	/// <summary>
	/// Creates, opens, reopens, or deletes a file.
	/// </summary>
	HFILE (WINAPI* OpenFile)(
		LPCSTR lpFileName,
		LPOFSTRUCT lpReOpenBuff,
		UINT uStyle) = ::OpenFile;

	/// <summary>
	/// Opens the file that matches the specified identifier.
	/// </summary>
	HANDLE (WINAPI* OpenFileById)(
		HANDLE hVolumeHint,
		LPFILE_ID_DESCRIPTOR lpFileId,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		DWORD dwFlagsAndAttributes) = ::OpenFileById;

	/// <summary>
	/// Backs up (export) encrypted files.
	/// </summary>
	DWORD (WINAPI* ReadEncryptedFileRaw)(
		PFE_EXPORT_FUNC pfExportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext) = ::ReadEncryptedFileRaw;

	/// <summary>
	/// Deletes an existing empty directory as a transacted operation.
	/// </summary>
	BOOL (WINAPI* RemoveDirectoryTransactedA)(
		LPCSTR lpPathName,
		HANDLE hTransaction) = ::RemoveDirectoryTransactedA;
	BOOL (WINAPI* RemoveDirectoryTransactedW)(
		LPCWSTR lpPathName,
		HANDLE hTransaction) = ::RemoveDirectoryTransactedW;

	/// <summary>
	/// Reopens the specified file system object with different access rights, sharing mode, and flags.
	/// </summary>
	HANDLE (WINAPI* ReOpenFile)(
		HANDLE hOriginalFile,
		DWORD dwDesiredAccess,
		DWORD dwShareMode,
		DWORD dwFlagsAndAttributes) = ::ReOpenFile;

	/// <summary>
	/// Replaces one file with another file, with the option of creating a backup copy of the original file.
	/// </summary>
	BOOL (WINAPI* ReplaceFileA)(
		LPCSTR lpReplacedFileName,
		LPCSTR lpReplacementFileName,
		LPCSTR lpBackupFileName,
		DWORD dwReplaceFlags,
		LPVOID lpExclude,
		LPVOID lpReserved) = ::ReplaceFileA;
	BOOL (WINAPI* ReplaceFileW)(
		LPCWSTR lpReplacedFileName,
		LPCWSTR lpReplacementFileName,
		LPCWSTR lpBackupFileName,
		DWORD dwReplaceFlags,
		LPVOID lpExclude,
		LPVOID lpReserved) = ::ReplaceFileW;

	/// <summary>
	/// Changes the current directory for the current process.
	/// </summary>
	BOOL (WINAPI* SetCurrentDirectoryA)(
		LPCSTR lpPathName) = ::SetCurrentDirectoryA;
	BOOL (WINAPI* SetCurrentDirectoryW)(
		LPCWSTR lpPathName) = ::SetCurrentDirectoryW;

	/// <summary>
	/// Adds a directory to the search path used to locate DLLs for the application.
	/// </summary>
	BOOL (WINAPI* SetDllDirectoryA)(
		LPCSTR lpPathName) = ::SetDllDirectoryA;
	BOOL (WINAPI* SetDllDirectoryW)(
		LPCWSTR lpPathName) = ::SetDllDirectoryW;

	/// <summary>
	/// Sets the attributes for a file or directory as a transacted operation.
	/// </summary>
	BOOL (WINAPI* SetFileAttributesTransactedA)(
		LPCSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction) = ::SetFileAttributesTransactedA;
	BOOL (WINAPI* SetFileAttributesTransactedW)(
		LPCWSTR lpFileName,
		DWORD dwFileAttributes,
		HANDLE hTransaction) = ::SetFileAttributesTransactedW;

	/// <summary>
	/// Requests that bandwidth for the specified file stream be reserved. The reservation
	/// is specified as a number of bytes in a period of milliseconds for I/O requests on the specified file handle.
	/// </summary>
	BOOL (WINAPI* SetFileBandwidthReservation)(
		HANDLE hFile,
		DWORD nPeriodMilliseconds,
		DWORD nBytesPerPeriod,
		BOOL bDiscardable,
		LPDWORD lpTransferSize,
		LPDWORD lpNumOutstandingRequests) = ::SetFileBandwidthReservation;

	/// <summary>
	/// Sets the notification modes for a file handle, allowing you to specify how completion notifications work for the specified file.
	/// </summary>
	BOOL (WINAPI* SetFileCompletionNotificationModes)(
		HANDLE FileHandle,
		UCHAR Flags) = ::SetFileCompletionNotificationModes;

	/// <summary>
	/// Sets the security of a file or directory object.
	/// </summary>
	BOOL (WINAPI* SetFileSecurityA)(
		LPCSTR lpFileName,
		SECURITY_INFORMATION SecurityInformation,
		PSECURITY_DESCRIPTOR pSecurityDescriptor) = ::SetFileSecurityA;

	/// <summary>
	/// Sets the short name for the specified file.
	/// </summary>
	BOOL (WINAPI* SetFileShortNameA)(
		HANDLE hFile,
		LPCSTR lpShortName) = ::SetFileShortNameA;
	BOOL (WINAPI* SetFileShortNameW)(
		HANDLE hFile,
		LPCWSTR lpShortName) = ::SetFileShortNameW;

	/// <summary>
	/// Sets the per-process mode that the SearchPath function uses when locating files.
	/// </summary>
	BOOL (WINAPI* SetSearchPathMode)(
		DWORD Flags) = ::SetSearchPathMode;

	/// <summary>
	/// Restores (import) encrypted files.
	/// </summary>
	DWORD (WINAPI* WriteEncryptedFileRaw)(
		PFE_IMPORT_FUNC pfImportCallback,
		PVOID pvCallbackContext,
		PVOID pvContext) = ::WriteEncryptedFileRaw;
}