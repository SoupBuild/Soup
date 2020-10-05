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

		void LogError(std::string_view message)
		{
			m_callback->OnError(message);
		}

		void LogMessage(DetourMessage& message)
		{
			int32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case DetourMessageType::Info_Initialize:
				{
					m_callback->OnInitialize();
					break;
				}
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
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result);
					break;
				}
				case DetourMessageType::CreateFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result);
					break;
				}
				case DetourMessageType::CreateFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result);
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
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameW(fileName, buffer, result);
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
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadLibraryA(libFileName);
					break;
				}
				case DetourMessageType::LoadLibraryW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_callback->OnLoadLibraryW(libFileName);
					break;
				}
				case DetourMessageType::LoadLibraryExA:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadLibraryExA(libFileName);
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
					auto path = ReadStringValue(message, offset);
					auto fileName = ReadStringValue(message, offset);
					auto extension = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSearchPathA(path, fileName, extension, result);
					break;
				}
				case DetourMessageType::SearchPathW:
				{
					auto path = ReadWStringValue(message, offset);
					auto fileName = ReadWStringValue(message, offset);
					auto extension = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSearchPathW(path, fileName, extension, result);
					break;
				}

				// ProcessThreadsApi
				case DetourMessageType::CreateProcessA:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessA(wasDetoured, applicationName, result);
					break;
				}
				case DetourMessageType::CreateProcessW:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessW(wasDetoured, applicationName, result);
					break;
				}
				case DetourMessageType::CreateProcessAsUserA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessAsUserA(applicationName, result);
					break;
				}
				case DetourMessageType::CreateProcessAsUserW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessAsUserW(applicationName, result);
					break;
				}
				case DetourMessageType::ExitProcess:
				{
					auto exitCode = ReadUInt32Value(message, offset);
					m_callback->OnExitProcess(exitCode);
					break;
				}

				// UndocumentedApi
				case DetourMessageType::PrivCopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnPrivCopyFileExA(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::PrivCopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnPrivCopyFileExW(existingFileName, newFileName, result);
					break;
				}
				
				// WinBase
				case DetourMessageType::CopyFileA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto failIfExists = ReadBoolValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileA(existingFileName, newFileName, failIfExists, result);
					break;
				}
				case DetourMessageType::CopyFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto failIfExists = ReadBoolValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileW(existingFileName, newFileName, failIfExists, result);
					break;
				}
				case DetourMessageType::CopyFile2:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCopyFile2(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::CopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileExA(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::CopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileExW(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::CopyFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileTransactedA(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::CopyFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCopyFileTransactedW(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::CreateDirectoryExA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryExA(templateDirectory, newDirectory, result);
					break;
				}
				case DetourMessageType::CreateDirectoryExW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryExW(templateDirectory, newDirectory, result);
					break;
				}
				case DetourMessageType::CreateDirectoryTransactedA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result);
					break;
				}
				case DetourMessageType::CreateDirectoryTransactedW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result);
					break;
				}
				case DetourMessageType::CreateFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result);
					break;
				}
				case DetourMessageType::CreateFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					m_callback->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result);
					break;
				}
				case DetourMessageType::CreateHardLinkA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkA(fileName, existingFileName, result);
					break;
				}
				case DetourMessageType::CreateHardLinkW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkW(fileName, existingFileName, result);
					break;
				}
				case DetourMessageType::CreateHardLinkTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
					break;
				}
				case DetourMessageType::CreateHardLinkTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
					break;
				}
				case DetourMessageType::CreateProcessWithLogonW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessWithLogonW(applicationName, result);
					break;
				}
				case DetourMessageType::CreateProcessWithTokenW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessWithTokenW(applicationName, result);
					break;
				}
				case DetourMessageType::CreateSymbolicLinkA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourMessageType::CreateSymbolicLinkW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourMessageType::CreateSymbolicLinkTransactedA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourMessageType::CreateSymbolicLinkTransactedW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourMessageType::DecryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDecryptFileA(fileName, result);
					break;
				}
				case DetourMessageType::DecryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDecryptFileW(fileName, result);
					break;
				}
				case DetourMessageType::DeleteFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileTransactedA(fileName, result);
					break;
				}
				case DetourMessageType::DeleteFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileTransactedW(fileName, result);
					break;
				}
				case DetourMessageType::EncryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnEncryptFileA(fileName, result);
					break;
				}
				case DetourMessageType::EncryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnEncryptFileW(fileName, result);
					break;
				}
				case DetourMessageType::FileEncryptionStatusA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileEncryptionStatusA(fileName, result);
					break;
				}
				case DetourMessageType::FileEncryptionStatusW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileEncryptionStatusW(fileName, result);
					break;
				}
				case DetourMessageType::FindFirstFileNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindFirstFileNameTransactedW(fileName, result);
					break;
				}
				case DetourMessageType::FindFirstFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileTransactedA(fileName);
					break;
				}
				case DetourMessageType::FindFirstFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileTransactedW(fileName);
					break;
				}
				case DetourMessageType::FindFirstStreamTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstStreamTransactedW(fileName);
					break;
				}
				case DetourMessageType::GetBinaryTypeA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetBinaryTypeA(applicationName, result);
					break;
				}
				case DetourMessageType::GetBinaryTypeW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetBinaryTypeW(applicationName, result);
					break;
				}
				case DetourMessageType::GetCompressedFileSizeTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeTransactedA(fileName, result);
					break;
				}
				case DetourMessageType::GetCompressedFileSizeTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeTransactedW(fileName, result);
					break;
				}
				case DetourMessageType::GetDllDirectoryA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDllDirectoryA(result);
					break;
				}
				case DetourMessageType::GetDllDirectoryW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDllDirectoryW(result);
					break;
				}
				case DetourMessageType::GetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesTransactedA(fileName, result);
					break;
				}
				case DetourMessageType::GetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesTransactedW(fileName, result);
					break;
				}
				case DetourMessageType::GetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileBandwidthReservation(result);
					break;
				}
				case DetourMessageType::GetFileInformationByHandleEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileInformationByHandleEx(result);
					break;
				}
				case DetourMessageType::GetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileSecurityA(fileName, result);
					break;
				}
				case DetourMessageType::GetFullPathNameTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameTransactedA(fileName, result);
					break;
				}
				case DetourMessageType::GetFullPathNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameTransactedW(fileName, result);
					break;
				}
				case DetourMessageType::GetLongPathNameTransactedA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameTransactedA(shortPath, longPath, result);
					break;
				}
				case DetourMessageType::GetLongPathNameTransactedW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameTransactedW(shortPath, longPath, result);
					break;
				}
				case DetourMessageType::GetQueuedCompletionStatus:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetQueuedCompletionStatus(result);
					break;
				}
				case DetourMessageType::GetQueuedCompletionStatusEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetQueuedCompletionStatusEx(result);
					break;
				}
				case DetourMessageType::GetShortPathNameA:
				{
					auto longPath = ReadStringValue(message, offset);
					auto shortPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetShortPathNameA(longPath, shortPath, result);
					break;
				}
				case DetourMessageType::LoadModule:
				{
					auto moduleName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnLoadModule(moduleName, result);
					break;
				}
				case DetourMessageType::LoadPackagedLibrary:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadPackagedLibrary(libFileName);
					break;
				}
				case DetourMessageType::MoveFileA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileA(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::MoveFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileW(existingFileName, newFileName, result);
					break;
				}
				case DetourMessageType::MoveFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileExA(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::MoveFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileExW(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::MoveFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileTransactedA(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::MoveFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileTransactedW(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::MoveFileWithProgressA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::MoveFileWithProgressW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result);
					break;
				}
				case DetourMessageType::OpenEncryptedFileRawA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnOpenEncryptedFileRawA(fileName, flags, result);
					break;
				}
				case DetourMessageType::OpenEncryptedFileRawW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnOpenEncryptedFileRawW(fileName, flags, result);
					break;
				}
				case DetourMessageType::OpenFile:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnOpenFile(fileName);
					break;
				}
				case DetourMessageType::OpenFileById:
				{
					m_callback->OnOpenFileById();
					break;
				}
				case DetourMessageType::ReadEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnReadEncryptedFileRaw(result);
					break;
				}
				case DetourMessageType::RemoveDirectoryTransactedA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryTransactedA(pathName, result);
					break;
				}
				case DetourMessageType::RemoveDirectoryTransactedW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryTransactedW(pathName, result);
					break;
				}
				case DetourMessageType::ReOpenFile:
				{
					m_callback->OnReOpenFile();
					break;
				}
				case DetourMessageType::ReplaceFileA:
				{
					auto replacedFileName = ReadStringValue(message, offset);
					auto replacementFileName = ReadStringValue(message, offset);
					auto backupFileName = ReadStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourMessageType::ReplaceFileW:
				{
					auto replacedFileName = ReadWStringValue(message, offset);
					auto replacementFileName = ReadWStringValue(message, offset);
					auto backupFileName = ReadWStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourMessageType::SetCurrentDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetCurrentDirectoryA(pathName, result);
					break;
				}
				case DetourMessageType::SetCurrentDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetCurrentDirectoryW(pathName, result);
					break;
				}
				case DetourMessageType::SetDllDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetDllDirectoryA(pathName, result);
					break;
				}
				case DetourMessageType::SetDllDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetDllDirectoryW(pathName, result);
					break;
				}
				case DetourMessageType::SetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesTransactedA(fileName, fileAttributes, result);
					break;
				}
				case DetourMessageType::SetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesTransactedW(fileName, fileAttributes, result);
					break;
				}
				case DetourMessageType::SetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileBandwidthReservation(result);
					break;
				}
				case DetourMessageType::SetFileCompletionNotificationModes:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileCompletionNotificationModes(result);
					break;
				}
				case DetourMessageType::SetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileSecurityA(fileName, result);
					break;
				}
				case DetourMessageType::SetFileShortNameA:
				{
					auto shortName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileShortNameA(shortName, result);
					break;
				}
				case DetourMessageType::SetFileShortNameW:
				{
					auto shortName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileShortNameW(shortName, result);
					break;
				}
				case DetourMessageType::SetSearchPathMode:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetSearchPathMode(flags, result);
					break;
				}
				case DetourMessageType::WriteEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnWriteEncryptedFileRaw(result);
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
				throw std::runtime_error("ReadUInt32Value past end of content");
			return result;
		}

		uint64_t ReadUInt64Value(DetourMessage& message, int32_t& offset)
		{
			auto result = *reinterpret_cast<uint64_t*>(message.Content + offset);
			offset += sizeof(uint64_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt64Value past end of content");
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
