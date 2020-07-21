// <copyright file="EventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Monitor
{
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered callback.
	/// </summary>
	export class EventListener
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsDetourProcess'/> class.
		/// </summary>
		EventListener(
			std::shared_ptr<IDetourCallback> callback) :
			m_callback(std::move(callback))
		{
		}

		void LogMessage(DetourMessage& message)
		{
			int32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case DetourMessageType::Info_Shutdown:
				{
					m_callback->OnShutdown();
					break;
				}
				case DetourMessageType::Info_Error:
				{
					auto errorMessage = ReadStringValue(message, offset);
					m_callback->OnError(errorMessage);
					break;
				}

				// FileApi
				case DetourMessageType::AreFileApisANSI:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnAreFileApisANSI(result);
					break;
				}
				case DetourMessageType::CompareFileTime:
				{
					auto result = ReadInt32Value(message, offset);
					m_callback->OnCompareFileTime(result);
					break;
				}
				case DetourMessageType::CreateDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryA(pathName, result);
					break;
				}
				case DetourMessageType::CreateDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryW(pathName, result);
					break;
				}
				case DetourMessageType::CreateFile2:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					m_callback->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition);
					break;
				}
				case DetourMessageType::CreateFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					m_callback->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes);
					break;
				}
				case DetourMessageType::CreateFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					m_callback->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes);
					break;
				}
				case DetourMessageType::DefineDosDeviceW:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto deviceName = ReadWStringValue(message, offset);
					auto targetPath = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
					break;
				}
				case DetourMessageType::DeleteFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileA(fileName, result);
					break;
				}
				case DetourMessageType::DeleteFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileW(fileName, result);
					break;
				}
				case DetourMessageType::DeleteVolumeMountPointW:
				{
					auto volumeMountPoint = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteVolumeMountPointW(volumeMountPoint, result);
					break;
				}
				case DetourMessageType::FileTimeToLocalFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileTimeToLocalFileTime(result);
					break;
				}
				case DetourMessageType::FindClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindClose(result);
					break;
				}
				case DetourMessageType::FindCloseChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindCloseChangeNotification(result);
					break;
				}
				case DetourMessageType::FindFirstChangeNotificationA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourMessageType::FindFirstChangeNotificationW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourMessageType::FindFirstFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileA(fileName);
					break;
				}
				case DetourMessageType::FindFirstFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileW(fileName);
					break;
				}
				case DetourMessageType::FindFirstFileExA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileExA(fileName);
					break;
				}
				case DetourMessageType::FindFirstFileExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileExW(fileName);
					break;
				}
				case DetourMessageType::FindFirstFileNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstFileNameW(fileName, flags);
					break;
				}
				case DetourMessageType::FindFirstStreamW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstStreamW(fileName);
					break;
				}
				case DetourMessageType::FindFirstVolumeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstVolumeW(fileName);
					break;
				}
				case DetourMessageType::FindNextChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextChangeNotification(result);
					break;
				}
				case DetourMessageType::FindNextFileA:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileA(result);
					break;
				}
				case DetourMessageType::FindNextFileW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileW(result);
					break;
				}
				case DetourMessageType::FindNextFileNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileNameW(result);
					break;
				}
				case DetourMessageType::FindNextStreamW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextStreamW(result);
					break;
				}
				case DetourMessageType::FindNextVolumeW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextVolumeW(result);
					break;
				}
				case DetourMessageType::FindVolumeClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindVolumeClose(result);
					break;
				}
				case DetourMessageType::FlushFileBuffers:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFlushFileBuffers(result);
					break;
				}
				case DetourMessageType::GetCompressedFileSizeA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeA(fileName, result);
					break;
				}
				case DetourMessageType::GetCompressedFileSizeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeW(fileName, result);
					break;
				}
				case DetourMessageType::GetDiskFreeSpaceA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceA(rootPathName, result);
					break;
				}
				case DetourMessageType::GetDiskFreeSpaceW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceW(rootPathName, result);
					break;
				}
				case DetourMessageType::GetDiskFreeSpaceExA:
				{
					auto directoryName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceExA(directoryName, result);
					break;
				}
				case DetourMessageType::GetDiskFreeSpaceExW:
				{
					auto directoryName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceExW(directoryName, result);
					break;
				}
				case DetourMessageType::GetDriveTypeA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDriveTypeA(rootPathName, result);
					break;
				}
				case DetourMessageType::GetDriveTypeW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDriveTypeW(rootPathName, result);
					break;
				}
				case DetourMessageType::GetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileAttributesA(fileName, result);
					break;
				}
				case DetourMessageType::GetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileAttributesW(fileName, result);
					break;
				}
				case DetourMessageType::GetFileAttributesExA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesExA(fileName, result);
					break;
				}
				case DetourMessageType::GetFileAttributesExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesExW(fileName, result);
					break;
				}
				case DetourMessageType::GetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileInformationByHandle(result);
					break;
				}
				case DetourMessageType::GetFileSize:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileSize(result);
					break;
				}
				case DetourMessageType::GetFileSizeEx:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileSizeEx(result);
					break;
				}
				case DetourMessageType::GetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileTime(result);
					break;
				}
				case DetourMessageType::GetFileType:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileType(result);
					break;
				}
				case DetourMessageType::GetFinalPathNameByHandleA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFinalPathNameByHandleA(result);
					break;
				}
				case DetourMessageType::GetFinalPathNameByHandleW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFinalPathNameByHandleW(result);
					break;
				}
				case DetourMessageType::GetFullPathNameA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameA(fileName, result);
					break;
				}
				case DetourMessageType::GetFullPathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameW(fileName, result);
					break;
				}
				case DetourMessageType::GetLogicalDrives:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLogicalDrives(result);
					break;
				}
				case DetourMessageType::GetLogicalDriveStringsW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLogicalDriveStringsW(buffer, result);
					break;
				}
				case DetourMessageType::GetLongPathNameA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameA(shortPath, longPath, result);
					break;
				}
				case DetourMessageType::GetLongPathNameW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameW(shortPath, longPath, result);
					break;
				}
				case DetourMessageType::GetShortPathNameW:
				{
					auto longPath = ReadWStringValue(message, offset);
					auto shortPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetShortPathNameW(longPath, shortPath, result);
					break;
				}
				case DetourMessageType::GetTempFileNameA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto prefixString = ReadStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourMessageType::GetTempFileNameW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto prefixString = ReadWStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourMessageType::GetTempPathA:
				{
					auto buffer = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempPathA(buffer, result);
					break;
				}
				case DetourMessageType::GetTempPathW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempPathW(buffer, result);
					break;
				}
				case DetourMessageType::GetVolumeInformationA:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationA(result);
					break;
				}
				case DetourMessageType::GetVolumeInformationW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationW(result);
					break;
				}
				case DetourMessageType::GetVolumeInformationByHandleW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationByHandleW(result);
					break;
				}
				case DetourMessageType::GetVolumeNameForVolumeMountPointW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeNameForVolumeMountPointW(result);
					break;
				}
				case DetourMessageType::GetVolumePathNamesForVolumeNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumePathNamesForVolumeNameW(result);
					break;
				}
				case DetourMessageType::GetVolumePathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumePathNameW(fileName, result);
					break;
				}
				case DetourMessageType::LocalFileTimeToFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLocalFileTimeToFileTime(result);
					break;
				}
				case DetourMessageType::LockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLockFile(result);
					break;
				}
				case DetourMessageType::LockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLockFileEx(result);
					break;
				}
				case DetourMessageType::QueryDosDeviceW:
				{
					auto deviceName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnQueryDosDeviceW(deviceName, result);
					break;
				}
				case DetourMessageType::ReadFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFile(result);
					break;
				}
				case DetourMessageType::ReadFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFileEx(result);
					break;
				}
				case DetourMessageType::ReadFileScatter:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFileScatter(result);
					break;
				}
				case DetourMessageType::RemoveDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryA(pathName, result);
					break;
				}
				case DetourMessageType::RemoveDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryW(pathName, result);
					break;
				}
				case DetourMessageType::SetEndOfFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetEndOfFile(result);
					break;
				}
				case DetourMessageType::SetFileApisToANSI:
				{
					m_callback->OnSetFileApisToANSI();
					break;
				}
				case DetourMessageType::SetFileApisToOEM:
				{
					m_callback->OnSetFileApisToOEM();
					break;
				}
				case DetourMessageType::SetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesA(fileName, result);
					break;
				}
				case DetourMessageType::SetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesW(fileName, result);
					break;
				}
				case DetourMessageType::SetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileInformationByHandle(result);
					break;
				}
				case DetourMessageType::SetFileIoOverlappedRange:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileIoOverlappedRange(result);
					break;
				}
				case DetourMessageType::SetFilePointer:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSetFilePointer(result);
					break;
				}
				case DetourMessageType::SetFilePointerEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFilePointerEx(result);
					break;
				}
				case DetourMessageType::SetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileTime(result);
					break;
				}
				case DetourMessageType::SetFileValidData:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileValidData(result);
					break;
				}
				case DetourMessageType::UnlockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnUnlockFile(result);
					break;
				}
				case DetourMessageType::UnlockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnUnlockFileEx(result);
					break;
				}
				case DetourMessageType::WriteFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFile(result);
					break;
				}
				case DetourMessageType::WriteFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFileEx(result);
					break;
				}
				case DetourMessageType::WriteFileGather:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFileGather(result);
					break;
				}
				
				// LibLoaderApi
				case DetourMessageType::LoadLibraryA:
				{
					m_callback->OnLoadLibraryA();
					break;
				}
				case DetourMessageType::LoadLibraryW:
				{
					m_callback->OnLoadLibraryW();
					break;
				}
				case DetourMessageType::LoadLibraryExA:
				{
					m_callback->OnLoadLibraryExA();
					break;
				}
				case DetourMessageType::LoadLibraryExW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_callback->OnLoadLibraryExW(libFileName);
					break;
				}

				// ProcessEnv
				case DetourMessageType::SearchPathA:
				{
					m_callback->OnSearchPathA();
					break;
				}
				case DetourMessageType::SearchPathW:
				{
					m_callback->OnSearchPathW();
					break;
				}

				// ProcessThreadsApi
				case DetourMessageType::CreateProcessA:
				{
					m_callback->OnCreateProcessA();
					break;
				}
				case DetourMessageType::CreateProcessW:
				{
					m_callback->OnCreateProcessW();
					break;
				}
				case DetourMessageType::CreateProcessAsUserA:
				{
					m_callback->OnCreateProcessAsUserA();
					break;
				}
				case DetourMessageType::CreateProcessAsUserW:
				{
					m_callback->OnCreateProcessAsUserW();
					break;
				}
				case DetourMessageType::ExitProcess:
				{
					m_callback->OnExitProcess();
					break;
				}

				// UndocumentedApi
				case DetourMessageType::PrivCopyFileExA:
				{
					m_callback->OnPrivCopyFileExA();
					break;
				}
				case DetourMessageType::PrivCopyFileExW:
				{
					m_callback->OnPrivCopyFileExW();
					break;
				}
				
				// WinBase
				case DetourMessageType::CopyFileA:
				{
					m_callback->OnCopyFileA();
					break;
				}
				case DetourMessageType::CopyFileW:
				{
					m_callback->OnCopyFileW();
					break;
				}
				case DetourMessageType::CopyFile2:
				{
					m_callback->OnCopyFile2();
					break;
				}
				case DetourMessageType::CopyFileExA:
				{
					m_callback->OnCopyFileExA();
					break;
				}
				case DetourMessageType::CopyFileExW:
				{
					m_callback->OnCopyFileExW();
					break;
				}
				case DetourMessageType::CopyFileTransactedA:
				{
					m_callback->OnCopyFileTransactedA();
					break;
				}
				case DetourMessageType::CopyFileTransactedW:
				{
					m_callback->OnCopyFileTransactedW();
					break;
				}
				case DetourMessageType::CreateDirectoryExA:
				{
					m_callback->OnCreateDirectoryExA();
					break;
				}
				case DetourMessageType::CreateDirectoryExW:
				{
					m_callback->OnCreateDirectoryExW();
					break;
				}
				case DetourMessageType::CreateDirectoryTransactedA:
				{
					m_callback->OnCreateDirectoryTransactedA();
					break;
				}
				case DetourMessageType::CreateDirectoryTransactedW:
				{
					m_callback->OnCreateDirectoryTransactedW();
					break;
				}
				case DetourMessageType::CreateFileTransactedA:
				{
					m_callback->OnCreateFileTransactedA();
					break;
				}
				case DetourMessageType::CreateFileTransactedW:
				{
					m_callback->OnCreateFileTransactedW();
					break;
				}
				case DetourMessageType::CreateHardLinkA:
				{
					m_callback->OnCreateHardLinkA();
					break;
				}
				case DetourMessageType::CreateHardLinkW:
				{
					m_callback->OnCreateHardLinkW();
					break;
				}
				case DetourMessageType::CreateHardLinkTransactedA:
				{
					m_callback->OnCreateHardLinkTransactedA();
					break;
				}
				case DetourMessageType::CreateHardLinkTransactedW:
				{
					m_callback->OnCreateHardLinkTransactedW();
					break;
				}
				case DetourMessageType::CreateProcessWithLogonW:
				{
					m_callback->OnCreateProcessWithLogonW();
					break;
				}
				case DetourMessageType::CreateProcessWithTokenW:
				{
					m_callback->OnCreateProcessWithTokenW();
					break;
				}
				case DetourMessageType::CreateSymbolicLinkA:
				{
					m_callback->OnCreateSymbolicLinkA();
					break;
				}
				case DetourMessageType::CreateSymbolicLinkW:
				{
					m_callback->OnCreateSymbolicLinkW();
					break;
				}
				case DetourMessageType::CreateSymbolicLinkTransactedA:
				{
					m_callback->OnCreateSymbolicLinkTransactedA();
					break;
				}
				case DetourMessageType::CreateSymbolicLinkTransactedW:
				{
					m_callback->OnCreateSymbolicLinkTransactedW();
					break;
				}
				case DetourMessageType::DecryptFileA:
				{
					m_callback->OnDecryptFileA();
					break;
				}
				case DetourMessageType::DecryptFileW:
				{
					m_callback->OnDecryptFileW();
					break;
				}
				case DetourMessageType::DeleteFileTransactedA:
				{
					m_callback->OnDeleteFileTransactedA();
					break;
				}
				case DetourMessageType::DeleteFileTransactedW:
				{
					m_callback->OnDeleteFileTransactedW();
					break;
				}
				case DetourMessageType::EncryptFileA:
				{
					m_callback->OnEncryptFileA();
					break;
				}
				case DetourMessageType::EncryptFileW:
				{
					m_callback->OnEncryptFileW();
					break;
				}
				case DetourMessageType::FileEncryptionStatusA:
				{
					m_callback->OnFileEncryptionStatusA();
					break;
				}
				case DetourMessageType::FileEncryptionStatusW:
				{
					m_callback->OnFileEncryptionStatusW();
					break;
				}
				case DetourMessageType::FindFirstFileNameTransactedW:
				{
					m_callback->OnFindFirstFileNameTransactedW();
					break;
				}
				case DetourMessageType::FindFirstFileTransactedA:
				{
					m_callback->OnFindFirstFileTransactedA();
					break;
				}
				case DetourMessageType::FindFirstFileTransactedW:
				{
					m_callback->OnFindFirstFileTransactedW();
					break;
				}
				case DetourMessageType::FindFirstStreamTransactedW:
				{
					m_callback->OnFindFirstStreamTransactedW();
					break;
				}
				case DetourMessageType::GetBinaryTypeA:
				{
					m_callback->OnGetBinaryTypeA();
					break;
				}
				case DetourMessageType::GetBinaryTypeW:
				{
					m_callback->OnGetBinaryTypeW();
					break;
				}
				case DetourMessageType::GetCompressedFileSizeTransactedA:
				{
					m_callback->OnGetCompressedFileSizeTransactedA();
					break;
				}
				case DetourMessageType::GetCompressedFileSizeTransactedW:
				{
					m_callback->OnGetCompressedFileSizeTransactedW();
					break;
				}
				case DetourMessageType::GetDllDirectoryA:
				{
					m_callback->OnGetDllDirectoryA();
					break;
				}
				case DetourMessageType::GetDllDirectoryW:
				{
					m_callback->OnGetDllDirectoryW();
					break;
				}
				case DetourMessageType::GetFileAttributesTransactedA:
				{
					m_callback->OnGetFileAttributesTransactedA();
					break;
				}
				case DetourMessageType::GetFileAttributesTransactedW:
				{
					m_callback->OnGetFileAttributesTransactedW();
					break;
				}
				case DetourMessageType::GetFileBandwidthReservation:
				{
					m_callback->OnGetFileBandwidthReservation();
					break;
				}
				case DetourMessageType::GetFileInformationByHandleEx:
				{
					m_callback->OnGetFileInformationByHandleEx();
					break;
				}
				case DetourMessageType::GetFileSecurityA:
				{
					m_callback->OnGetFileSecurityA();
					break;
				}
				case DetourMessageType::GetFullPathNameTransactedA:
				{
					m_callback->OnGetFullPathNameTransactedA();
					break;
				}
				case DetourMessageType::GetFullPathNameTransactedW:
				{
					m_callback->OnGetFullPathNameTransactedW();
					break;
				}
				case DetourMessageType::GetLongPathNameTransactedA:
				{
					m_callback->OnGetLongPathNameTransactedA();
					break;
				}
				case DetourMessageType::GetLongPathNameTransactedW:
				{
					m_callback->OnGetLongPathNameTransactedW();
					break;
				}
				case DetourMessageType::GetQueuedCompletionStatus:
				{
					m_callback->OnGetQueuedCompletionStatus();
					break;
				}
				case DetourMessageType::GetQueuedCompletionStatusEx:
				{
					m_callback->OnGetQueuedCompletionStatusEx();
					break;
				}
				case DetourMessageType::GetShortPathNameA:
				{
					m_callback->OnGetShortPathNameA();
					break;
				}
				case DetourMessageType::LoadModule:
				{
					m_callback->OnLoadModule();
					break;
				}
				case DetourMessageType::LoadPackagedLibrary:
				{
					m_callback->OnLoadPackagedLibrary();
					break;
				}
				case DetourMessageType::MoveFileA:
				{
					m_callback->OnMoveFileA();
					break;
				}
				case DetourMessageType::MoveFileW:
				{
					m_callback->OnMoveFileW();
					break;
				}
				case DetourMessageType::MoveFileExA:
				{
					m_callback->OnMoveFileExA();
					break;
				}
				case DetourMessageType::MoveFileExW:
				{
					m_callback->OnMoveFileExW();
					break;
				}
				case DetourMessageType::MoveFileTransactedA:
				{
					m_callback->OnMoveFileTransactedA();
					break;
				}
				case DetourMessageType::MoveFileTransactedW:
				{
					m_callback->OnMoveFileTransactedW();
					break;
				}
				case DetourMessageType::MoveFileWithProgressA:
				{
					m_callback->OnMoveFileWithProgressA();
					break;
				}
				case DetourMessageType::MoveFileWithProgressW:
				{
					m_callback->OnMoveFileWithProgressW();
					break;
				}
				case DetourMessageType::OpenEncryptedFileRawA:
				{
					m_callback->OnOpenEncryptedFileRawA();
					break;
				}
				case DetourMessageType::OpenEncryptedFileRawW:
				{
					m_callback->OnOpenEncryptedFileRawW();
					break;
				}
				case DetourMessageType::OpenFile:
				{
					m_callback->OnOpenFile();
					break;
				}
				case DetourMessageType::OpenFileById:
				{
					m_callback->OnOpenFileById();
					break;
				}
				case DetourMessageType::ReadEncryptedFileRaw:
				{
					m_callback->OnReadEncryptedFileRaw();
					break;
				}
				case DetourMessageType::RemoveDirectoryTransactedA:
				{
					m_callback->OnRemoveDirectoryTransactedA();
					break;
				}
				case DetourMessageType::RemoveDirectoryTransactedW:
				{
					m_callback->OnRemoveDirectoryTransactedW();
					break;
				}
				case DetourMessageType::ReOpenFile:
				{
					m_callback->OnReOpenFile();
					break;
				}
				case DetourMessageType::ReplaceFileA:
				{
					m_callback->OnReplaceFileA();
					break;
				}
				case DetourMessageType::ReplaceFileW:
				{
					m_callback->OnReplaceFileW();
					break;
				}
				case DetourMessageType::SetCurrentDirectoryA:
				{
					m_callback->OnSetCurrentDirectoryA();
					break;
				}
				case DetourMessageType::SetCurrentDirectoryW:
				{
					m_callback->OnSetCurrentDirectoryW();
					break;
				}
				case DetourMessageType::SetDllDirectoryA:
				{
					m_callback->OnSetDllDirectoryA();
					break;
				}
				case DetourMessageType::SetDllDirectoryW:
				{
					m_callback->OnSetDllDirectoryW();
					break;
				}
				case DetourMessageType::SetFileAttributesTransactedA:
				{
					m_callback->OnSetFileAttributesTransactedA();
					break;
				}
				case DetourMessageType::SetFileAttributesTransactedW:
				{
					m_callback->OnSetFileAttributesTransactedW();
					break;
				}
				case DetourMessageType::SetFileBandwidthReservation:
				{
					m_callback->OnSetFileBandwidthReservation();
					break;
				}
				case DetourMessageType::SetFileCompletionNotificationModes:
				{
					m_callback->OnSetFileCompletionNotificationModes();
					break;
				}
				case DetourMessageType::SetFileSecurityA:
				{
					m_callback->OnSetFileSecurityA();
					break;
				}
				case DetourMessageType::SetFileShortNameA:
				{
					m_callback->OnSetFileShortNameA();
					break;
				}
				case DetourMessageType::SetFileShortNameW:
				{
					m_callback->OnSetFileShortNameW();
					break;
				}
				case DetourMessageType::SetSearchPathMode:
				{
					m_callback->OnSetSearchPathMode();
					break;
				}
				case DetourMessageType::WriteEncryptedFileRaw:
				{
					m_callback->OnWriteEncryptedFileRaw();
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown message type");
				}
			}

			// Verify that we read the entire message
			if (offset != message.ContentSize)
			{
				throw std::runtime_error("Did not read the entire message");
			}
		}

	private:
		bool ReadBoolValue(DetourMessage& message, int32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadBoolValue past end of content");
			return result > 0;
		}

		int32_t ReadInt32Value(DetourMessage& message, int32_t& offset)
		{
			auto result = *reinterpret_cast<int32_t*>(message.Content + offset);
			offset += sizeof(int32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadInt32Value past end of content");
			return result;
		}

		uint32_t ReadUInt32Value(DetourMessage& message, int32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadInt32Value past end of content");
			return result;
		}

		std::string_view ReadStringValue(DetourMessage& message, int32_t& offset)
		{
			auto result = std::string_view(reinterpret_cast<char*>(message.Content + offset));
			offset += result.size() + 1;
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadStringValue past end of content");
			return result;
		}

		std::wstring_view ReadWStringValue(DetourMessage& message, int32_t& offset)
		{
			auto result = std::wstring_view(reinterpret_cast<wchar_t*>(message.Content + offset));
			offset += 2 * (result.size() + 1);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadWStringValue past end of content");
			return result;
		}

	private:
		// Input
		std::shared_ptr<IDetourCallback> m_callback;
	};
}
