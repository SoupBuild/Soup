// <copyright file="EventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourCallback.h"

namespace Monitor::Windows
{
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered callback.
	/// </summary>
	class EventListener
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='EventListener'/> class.
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

		void LogMessage(Message& message)
		{
			uint32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case MessageType::Initialize:
				{
					m_callback->OnInitialize();
					break;
				}
				case MessageType::Shutdown:
				{
					m_callback->OnShutdown();
					break;
				}
				case MessageType::Error:
				{
					auto errorMessage = ReadStringValue(message, offset);
					m_callback->OnError(errorMessage);
					break;
				}
				case MessageType::Detour:
				{
					HandleDetourMessage(message, offset);
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
		void HandleDetourMessage(Message& message, uint32_t& offset)
		{
			auto eventType = static_cast<DetourEventType>(ReadUInt32Value(message, offset));
			switch (eventType)
			{
				// FileApi
				case DetourEventType::AreFileApisANSI:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnAreFileApisANSI(result);
					break;
				}
				case DetourEventType::CompareFileTime:
				{
					auto result = ReadInt32Value(message, offset);
					m_callback->OnCompareFileTime(result);
					break;
				}
				case DetourEventType::CreateDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryW(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFile2:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto sharedMode = ReadUInt32Value(message, offset);
					auto creationDisposition = ReadUInt32Value(message, offset);
					auto flagsAndAttributes = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::DefineDosDeviceW:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto deviceName = ReadWStringValue(message, offset);
					auto targetPath = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
					break;
				}
				case DetourEventType::DeleteFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteVolumeMountPointW:
				{
					auto volumeMountPoint = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDeleteVolumeMountPointW(volumeMountPoint, result);
					break;
				}
				case DetourEventType::FileTimeToLocalFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileTimeToLocalFileTime(result);
					break;
				}
				case DetourEventType::FindClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindClose(result);
					break;
				}
				case DetourEventType::FindCloseChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindCloseChangeNotification(result);
					break;
				}
				case DetourEventType::FindFirstChangeNotificationA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourEventType::FindFirstChangeNotificationW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourEventType::FindFirstFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileW(fileName);
					break;
				}
				case DetourEventType::FindFirstFileExA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileExA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileExW(fileName);
					break;
				}
				case DetourEventType::FindFirstFileNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					m_callback->OnFindFirstFileNameW(fileName, flags);
					break;
				}
				case DetourEventType::FindFirstStreamW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstStreamW(fileName);
					break;
				}
				case DetourEventType::FindFirstVolumeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstVolumeW(fileName);
					break;
				}
				case DetourEventType::FindNextChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextChangeNotification(result);
					break;
				}
				case DetourEventType::FindNextFileA:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileA(result);
					break;
				}
				case DetourEventType::FindNextFileW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileW(result);
					break;
				}
				case DetourEventType::FindNextFileNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextFileNameW(result);
					break;
				}
				case DetourEventType::FindNextStreamW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextStreamW(result);
					break;
				}
				case DetourEventType::FindNextVolumeW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindNextVolumeW(result);
					break;
				}
				case DetourEventType::FindVolumeClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindVolumeClose(result);
					break;
				}
				case DetourEventType::FlushFileBuffers:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFlushFileBuffers(result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeA(fileName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeW(fileName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceA(rootPathName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceW(rootPathName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceExA:
				{
					auto directoryName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceExA(directoryName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceExW:
				{
					auto directoryName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetDiskFreeSpaceExW(directoryName, result);
					break;
				}
				case DetourEventType::GetDriveTypeA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDriveTypeA(rootPathName, result);
					break;
				}
				case DetourEventType::GetDriveTypeW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDriveTypeW(rootPathName, result);
					break;
				}
				case DetourEventType::GetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesExA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesExA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesExW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileInformationByHandle(result);
					break;
				}
				case DetourEventType::GetFileSize:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileSize(result);
					break;
				}
				case DetourEventType::GetFileSizeEx:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileSizeEx(result);
					break;
				}
				case DetourEventType::GetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileTime(result);
					break;
				}
				case DetourEventType::GetFileType:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFileType(result);
					break;
				}
				case DetourEventType::GetFinalPathNameByHandleA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFinalPathNameByHandleA(result);
					break;
				}
				case DetourEventType::GetFinalPathNameByHandleW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFinalPathNameByHandleW(result);
					break;
				}
				case DetourEventType::GetFullPathNameA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameA(fileName, result);
					break;
				}
				case DetourEventType::GetFullPathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameW(fileName, buffer, result);
					break;
				}
				case DetourEventType::GetLogicalDrives:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLogicalDrives(result);
					break;
				}
				case DetourEventType::GetLogicalDriveStringsW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLogicalDriveStringsW(buffer, result);
					break;
				}
				case DetourEventType::GetLongPathNameA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameA(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetLongPathNameW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameW(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetShortPathNameW:
				{
					auto longPath = ReadWStringValue(message, offset);
					auto shortPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetShortPathNameW(longPath, shortPath, result);
					break;
				}
				case DetourEventType::GetTempFileNameA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto prefixString = ReadStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourEventType::GetTempFileNameW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto prefixString = ReadWStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourEventType::GetTempPathA:
				{
					auto buffer = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempPathA(buffer, result);
					break;
				}
				case DetourEventType::GetTempPathW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetTempPathW(buffer, result);
					break;
				}
				case DetourEventType::GetVolumeInformationA:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationA(result);
					break;
				}
				case DetourEventType::GetVolumeInformationW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationW(result);
					break;
				}
				case DetourEventType::GetVolumeInformationByHandleW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeInformationByHandleW(result);
					break;
				}
				case DetourEventType::GetVolumeNameForVolumeMountPointW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumeNameForVolumeMountPointW(result);
					break;
				}
				case DetourEventType::GetVolumePathNamesForVolumeNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumePathNamesForVolumeNameW(result);
					break;
				}
				case DetourEventType::GetVolumePathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetVolumePathNameW(fileName, result);
					break;
				}
				case DetourEventType::LocalFileTimeToFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLocalFileTimeToFileTime(result);
					break;
				}
				case DetourEventType::LockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLockFile(result);
					break;
				}
				case DetourEventType::LockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnLockFileEx(result);
					break;
				}
				case DetourEventType::QueryDosDeviceW:
				{
					auto deviceName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnQueryDosDeviceW(deviceName, result);
					break;
				}
				case DetourEventType::ReadFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFile(result);
					break;
				}
				case DetourEventType::ReadFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFileEx(result);
					break;
				}
				case DetourEventType::ReadFileScatter:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReadFileScatter(result);
					break;
				}
				case DetourEventType::RemoveDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::RemoveDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryW(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetEndOfFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetEndOfFile(result);
					break;
				}
				case DetourEventType::SetFileApisToANSI:
				{
					m_callback->OnSetFileApisToANSI();
					break;
				}
				case DetourEventType::SetFileApisToOEM:
				{
					m_callback->OnSetFileApisToOEM();
					break;
				}
				case DetourEventType::SetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileInformationByHandle(result);
					break;
				}
				case DetourEventType::SetFileIoOverlappedRange:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileIoOverlappedRange(result);
					break;
				}
				case DetourEventType::SetFilePointer:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSetFilePointer(result);
					break;
				}
				case DetourEventType::SetFilePointerEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFilePointerEx(result);
					break;
				}
				case DetourEventType::SetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileTime(result);
					break;
				}
				case DetourEventType::SetFileValidData:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileValidData(result);
					break;
				}
				case DetourEventType::UnlockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnUnlockFile(result);
					break;
				}
				case DetourEventType::UnlockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnUnlockFileEx(result);
					break;
				}
				case DetourEventType::WriteFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFile(result);
					break;
				}
				case DetourEventType::WriteFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFileEx(result);
					break;
				}
				case DetourEventType::WriteFileGather:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnWriteFileGather(result);
					break;
				}
				
				// LibLoaderApi
				case DetourEventType::LoadLibraryA:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadLibraryA(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_callback->OnLoadLibraryW(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryExA:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadLibraryExA(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryExW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_callback->OnLoadLibraryExW(libFileName);
					break;
				}

				// ProcessEnv
				case DetourEventType::SearchPathA:
				{
					auto path = ReadStringValue(message, offset);
					auto fileName = ReadStringValue(message, offset);
					auto extension = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSearchPathA(path, fileName, extension, result);
					break;
				}
				case DetourEventType::SearchPathW:
				{
					auto path = ReadWStringValue(message, offset);
					auto fileName = ReadWStringValue(message, offset);
					auto extension = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnSearchPathW(path, fileName, extension, result);
					break;
				}

				// ProcessThreadsApi
				case DetourEventType::CreateProcessA:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessA(wasDetoured, applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessW:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessW(wasDetoured, applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessAsUserA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessAsUserA(applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessAsUserW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessAsUserW(applicationName, result);
					break;
				}
				case DetourEventType::ExitProcess:
				{
					auto exitCode = ReadUInt32Value(message, offset);
					m_callback->OnExitProcess(exitCode);
					break;
				}

				// UndocumentedApi
				case DetourEventType::PrivCopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::PrivCopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				
				// WinBase
				case DetourEventType::CopyFileA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto failIfExists = ReadBoolValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto failIfExists = ReadBoolValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFile2:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryExA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryExW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryTransactedA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryTransactedW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateHardLinkA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkA(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkW(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateProcessWithLogonW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessWithLogonW(applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessWithTokenW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateProcessWithTokenW(applicationName, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkTransactedA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkTransactedW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::DecryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDecryptFileA(fileName, result);
					break;
				}
				case DetourEventType::DecryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnDecryptFileW(fileName, result);
					break;
				}
				case DetourEventType::DeleteFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileTransactedA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnDeleteFileTransactedW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::EncryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnEncryptFileA(fileName, result);
					break;
				}
				case DetourEventType::EncryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnEncryptFileW(fileName, result);
					break;
				}
				case DetourEventType::FileEncryptionStatusA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileEncryptionStatusA(fileName, result);
					break;
				}
				case DetourEventType::FileEncryptionStatusW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFileEncryptionStatusW(fileName, result);
					break;
				}
				case DetourEventType::FindFirstFileNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnFindFirstFileNameTransactedW(fileName, result);
					break;
				}
				case DetourEventType::FindFirstFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_callback->OnFindFirstFileTransactedA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstFileTransactedW(fileName);
					break;
				}
				case DetourEventType::FindFirstStreamTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_callback->OnFindFirstStreamTransactedW(fileName);
					break;
				}
				case DetourEventType::GetBinaryTypeA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetBinaryTypeA(applicationName, result);
					break;
				}
				case DetourEventType::GetBinaryTypeW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetBinaryTypeW(applicationName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeTransactedA(fileName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetCompressedFileSizeTransactedW(fileName, result);
					break;
				}
				case DetourEventType::GetDllDirectoryA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDllDirectoryA(result);
					break;
				}
				case DetourEventType::GetDllDirectoryW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetDllDirectoryW(result);
					break;
				}
				case DetourEventType::GetEnvironmentVariableA:
				{
					auto name = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetEnvironmentVariableA(name, result);
					break;
				}
				case DetourEventType::GetEnvironmentVariableW:
				{
					auto name = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetEnvironmentVariableW(name, result);
					break;
				}
				case DetourEventType::GetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileBandwidthReservation(result);
					break;
				}
				case DetourEventType::GetFileInformationByHandleEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetFileInformationByHandleEx(result);
					break;
				}
				case DetourEventType::GetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnGetFileSecurityA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFullPathNameTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameTransactedA(fileName, result);
					break;
				}
				case DetourEventType::GetFullPathNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetFullPathNameTransactedW(fileName, result);
					break;
				}
				case DetourEventType::GetLongPathNameTransactedA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameTransactedA(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetLongPathNameTransactedW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetLongPathNameTransactedW(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetQueuedCompletionStatus:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetQueuedCompletionStatus(result);
					break;
				}
				case DetourEventType::GetQueuedCompletionStatusEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnGetQueuedCompletionStatusEx(result);
					break;
				}
				case DetourEventType::GetShortPathNameA:
				{
					auto longPath = ReadStringValue(message, offset);
					auto shortPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnGetShortPathNameA(longPath, shortPath, result);
					break;
				}
				case DetourEventType::LoadModule:
				{
					auto moduleName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnLoadModule(moduleName, result);
					break;
				}
				case DetourEventType::LoadPackagedLibrary:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_callback->OnLoadPackagedLibrary(libFileName);
					break;
				}
				case DetourEventType::MoveFileA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileWithProgressA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileWithProgressW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::OpenEncryptedFileRawA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnOpenEncryptedFileRawA(fileName, flags, result);
					break;
				}
				case DetourEventType::OpenEncryptedFileRawW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnOpenEncryptedFileRawW(fileName, flags, result);
					break;
				}
				case DetourEventType::OpenFile:
				{
					auto fileName = ReadStringValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnOpenFile(fileName, wasBlocked);
					break;
				}
				case DetourEventType::OpenFileById:
				{
					m_callback->OnOpenFileById();
					break;
				}
				case DetourEventType::ReadEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnReadEncryptedFileRaw(result);
					break;
				}
				case DetourEventType::RemoveDirectoryTransactedA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::RemoveDirectoryTransactedW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::ReOpenFile:
				{
					m_callback->OnReOpenFile();
					break;
				}
				case DetourEventType::ReplaceFileA:
				{
					auto replacedFileName = ReadStringValue(message, offset);
					auto replacementFileName = ReadStringValue(message, offset);
					auto backupFileName = ReadStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourEventType::ReplaceFileW:
				{
					auto replacedFileName = ReadWStringValue(message, offset);
					auto replacementFileName = ReadWStringValue(message, offset);
					auto backupFileName = ReadWStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourEventType::SetCurrentDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetCurrentDirectoryA(pathName, result);
					break;
				}
				case DetourEventType::SetCurrentDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetCurrentDirectoryW(pathName, result);
					break;
				}
				case DetourEventType::SetDllDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetDllDirectoryA(pathName, result);
					break;
				}
				case DetourEventType::SetDllDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetDllDirectoryW(pathName, result);
					break;
				}
				case DetourEventType::SetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesTransactedA(fileName, fileAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_callback->OnSetFileAttributesTransactedW(fileName, fileAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileBandwidthReservation(result);
					break;
				}
				case DetourEventType::SetFileCompletionNotificationModes:
				{
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileCompletionNotificationModes(result);
					break;
				}
				case DetourEventType::SetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileSecurityA(fileName, result);
					break;
				}
				case DetourEventType::SetFileShortNameA:
				{
					auto shortName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileShortNameA(shortName, result);
					break;
				}
				case DetourEventType::SetFileShortNameW:
				{
					auto shortName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetFileShortNameW(shortName, result);
					break;
				}
				case DetourEventType::SetSearchPathMode:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_callback->OnSetSearchPathMode(flags, result);
					break;
				}
				case DetourEventType::WriteEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_callback->OnWriteEncryptedFileRaw(result);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown detour event type");
				}
			}
		}

		bool ReadBoolValue(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadBoolValue past end of content");
			return result > 0;
		}

		int32_t ReadInt32Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<int32_t*>(message.Content + offset);
			offset += sizeof(int32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadInt32Value past end of content");
			return result;
		}

		uint32_t ReadUInt32Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt32Value past end of content");
			return result;
		}

		uint64_t ReadUInt64Value(Message& message, uint32_t& offset)
		{
			auto result = *reinterpret_cast<uint64_t*>(message.Content + offset);
			offset += sizeof(uint64_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt64Value past end of content");
			return result;
		}

		std::string_view ReadStringValue(Message& message, uint32_t& offset)
		{
			auto result = std::string_view(reinterpret_cast<char*>(message.Content + offset));
			offset += static_cast<uint32_t>(result.size()) + 1;
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadStringValue past end of content");
			return result;
		}

		std::wstring_view ReadWStringValue(Message& message, uint32_t& offset)
		{
			auto result = std::wstring_view(reinterpret_cast<wchar_t*>(message.Content + offset));
			offset += 2 * (static_cast<uint32_t>(result.size()) + 1);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadWStringValue past end of content");
			return result;
		}

	private:
		// Input
		std::shared_ptr<IDetourCallback> m_callback;
	};
}
