// <copyright file="WindowsDetourEventListener.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IWindowsSystemMonitor.h"

namespace Monitor::Windows
{
	/// <summary>
	/// The event listener knows how to parse an incoming message and pass it along to the
	/// registered monitor.
	/// </summary>
	class WindowsDetourEventListener
	{
	private:
		// Input
		std::shared_ptr<IWindowsSystemMonitor> m_monitor;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='EventListener'/> class.
		/// </summary>
		DetourEventListener(
			std::shared_ptr<IWindowsSystemMonitor> monitor) :
			m_monitor(std::move(monitor))
		{
		}

		void LogError(std::string_view message)
		{
			m_monitor->OnError(message);
		}

		void SafeLogMessage(Message& message)
		{
			try
			{
				LogMessage(message);
			}
			catch (std::exception& ex)
			{
				Log::Error("Event Listener encountered invalid message: {}", ex.what());
			}
		}

	private:
		void LogMessage(Message& message)
		{
			uint32_t offset = 0;
			switch (message.Type)
			{
				// Info
				case MessageType::Initialize:
				{
					m_monitor->OnInitialize();
					break;
				}
				case MessageType::Shutdown:
				{
					auto hadError = ReadBoolValue(message, offset);
					m_monitor->OnShutdown(hadError);
					break;
				}
				case MessageType::Error:
				{
					auto errorMessage = ReadStringValue(message, offset);
					m_monitor->OnError(errorMessage);
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

		void HandleDetourMessage(Message& message, uint32_t& offset)
		{
			auto eventType = static_cast<DetourEventType>(ReadUInt32Value(message, offset));
			switch (eventType)
			{
				// FileApi
				case DetourEventType::AreFileApisANSI:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnAreFileApisANSI(result);
					break;
				}
				case DetourEventType::CompareFileTime:
				{
					auto result = ReadInt32Value(message, offset);
					m_monitor->OnCompareFileTime(result);
					break;
				}
				case DetourEventType::CreateDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryW(pathName, result, wasBlocked);
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
					m_monitor->OnCreateFile2(fileName, desiredAccess, sharedMode, creationDisposition, result, wasBlocked);
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
					m_monitor->OnCreateFileA(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
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
					m_monitor->OnCreateFileW(fileName, desiredAccess, sharedMode, creationDisposition, flagsAndAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::DefineDosDeviceW:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto deviceName = ReadWStringValue(message, offset);
					auto targetPath = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnDefineDosDeviceW(flags, deviceName, targetPath, result);
					break;
				}
				case DetourEventType::DeleteFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnDeleteFileA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnDeleteFileW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteVolumeMountPointW:
				{
					auto volumeMountPoint = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnDeleteVolumeMountPointW(volumeMountPoint, result);
					break;
				}
				case DetourEventType::FileTimeToLocalFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFileTimeToLocalFileTime(result);
					break;
				}
				case DetourEventType::FindClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindClose(result);
					break;
				}
				case DetourEventType::FindCloseChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindCloseChangeNotification(result);
					break;
				}
				case DetourEventType::FindFirstChangeNotificationA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_monitor->OnFindFirstChangeNotificationA(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourEventType::FindFirstChangeNotificationW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto watchSubtree = ReadBoolValue(message, offset);
					auto notifyFilter = ReadUInt32Value(message, offset);
					m_monitor->OnFindFirstChangeNotificationW(pathName, watchSubtree, notifyFilter);
					break;
				}
				case DetourEventType::FindFirstFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_monitor->OnFindFirstFileA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstFileW(fileName);
					break;
				}
				case DetourEventType::FindFirstFileExA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_monitor->OnFindFirstFileExA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstFileExW(fileName);
					break;
				}
				case DetourEventType::FindFirstFileNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					m_monitor->OnFindFirstFileNameW(fileName, flags);
					break;
				}
				case DetourEventType::FindFirstStreamW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstStreamW(fileName);
					break;
				}
				case DetourEventType::FindFirstVolumeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstVolumeW(fileName);
					break;
				}
				case DetourEventType::FindNextChangeNotification:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextChangeNotification(result);
					break;
				}
				case DetourEventType::FindNextFileA:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextFileA(result);
					break;
				}
				case DetourEventType::FindNextFileW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextFileW(result);
					break;
				}
				case DetourEventType::FindNextFileNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextFileNameW(result);
					break;
				}
				case DetourEventType::FindNextStreamW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextStreamW(result);
					break;
				}
				case DetourEventType::FindNextVolumeW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindNextVolumeW(result);
					break;
				}
				case DetourEventType::FindVolumeClose:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindVolumeClose(result);
					break;
				}
				case DetourEventType::FlushFileBuffers:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFlushFileBuffers(result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetCompressedFileSizeA(fileName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetCompressedFileSizeW(fileName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetDiskFreeSpaceA(rootPathName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetDiskFreeSpaceW(rootPathName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceExA:
				{
					auto directoryName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetDiskFreeSpaceExA(directoryName, result);
					break;
				}
				case DetourEventType::GetDiskFreeSpaceExW:
				{
					auto directoryName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetDiskFreeSpaceExW(directoryName, result);
					break;
				}
				case DetourEventType::GetDriveTypeA:
				{
					auto rootPathName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetDriveTypeA(rootPathName, result);
					break;
				}
				case DetourEventType::GetDriveTypeW:
				{
					auto rootPathName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetDriveTypeW(rootPathName, result);
					break;
				}
				case DetourEventType::GetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesExA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesExA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesExW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesExW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetFileInformationByHandle(result);
					break;
				}
				case DetourEventType::GetFileSize:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFileSize(result);
					break;
				}
				case DetourEventType::GetFileSizeEx:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFileSizeEx(result);
					break;
				}
				case DetourEventType::GetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetFileTime(result);
					break;
				}
				case DetourEventType::GetFileType:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFileType(result);
					break;
				}
				case DetourEventType::GetFinalPathNameByHandleA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFinalPathNameByHandleA(result);
					break;
				}
				case DetourEventType::GetFinalPathNameByHandleW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFinalPathNameByHandleW(result);
					break;
				}
				case DetourEventType::GetFullPathNameA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFullPathNameA(fileName, result);
					break;
				}
				case DetourEventType::GetFullPathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFullPathNameW(fileName, buffer, result);
					break;
				}
				case DetourEventType::GetLogicalDrives:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLogicalDrives(result);
					break;
				}
				case DetourEventType::GetLogicalDriveStringsW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLogicalDriveStringsW(buffer, result);
					break;
				}
				case DetourEventType::GetLongPathNameA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLongPathNameA(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetLongPathNameW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLongPathNameW(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetShortPathNameW:
				{
					auto longPath = ReadWStringValue(message, offset);
					auto shortPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetShortPathNameW(longPath, shortPath, result);
					break;
				}
				case DetourEventType::GetTempFileNameA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto prefixString = ReadStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetTempFileNameA(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourEventType::GetTempFileNameW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto prefixString = ReadWStringValue(message, offset);
					auto unique = ReadUInt32Value(message, offset);
					auto tempFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetTempFileNameW(pathName, prefixString, unique, tempFileName, result);
					break;
				}
				case DetourEventType::GetTempPathA:
				{
					auto buffer = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetTempPathA(buffer, result);
					break;
				}
				case DetourEventType::GetTempPathW:
				{
					auto buffer = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetTempPathW(buffer, result);
					break;
				}
				case DetourEventType::GetVolumeInformationA:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumeInformationA(result);
					break;
				}
				case DetourEventType::GetVolumeInformationW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumeInformationW(result);
					break;
				}
				case DetourEventType::GetVolumeInformationByHandleW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumeInformationByHandleW(result);
					break;
				}
				case DetourEventType::GetVolumeNameForVolumeMountPointW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumeNameForVolumeMountPointW(result);
					break;
				}
				case DetourEventType::GetVolumePathNamesForVolumeNameW:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumePathNamesForVolumeNameW(result);
					break;
				}
				case DetourEventType::GetVolumePathNameW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetVolumePathNameW(fileName, result);
					break;
				}
				case DetourEventType::LocalFileTimeToFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnLocalFileTimeToFileTime(result);
					break;
				}
				case DetourEventType::LockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnLockFile(result);
					break;
				}
				case DetourEventType::LockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnLockFileEx(result);
					break;
				}
				case DetourEventType::QueryDosDeviceW:
				{
					auto deviceName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnQueryDosDeviceW(deviceName, result);
					break;
				}
				case DetourEventType::ReadFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnReadFile(result);
					break;
				}
				case DetourEventType::ReadFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnReadFileEx(result);
					break;
				}
				case DetourEventType::ReadFileScatter:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnReadFileScatter(result);
					break;
				}
				case DetourEventType::RemoveDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnRemoveDirectoryA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::RemoveDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnRemoveDirectoryW(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetEndOfFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetEndOfFile(result);
					break;
				}
				case DetourEventType::SetFileApisToANSI:
				{
					m_monitor->OnSetFileApisToANSI();
					break;
				}
				case DetourEventType::SetFileApisToOEM:
				{
					m_monitor->OnSetFileApisToOEM();
					break;
				}
				case DetourEventType::SetFileAttributesA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnSetFileAttributesA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileAttributesW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnSetFileAttributesW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileInformationByHandle:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileInformationByHandle(result);
					break;
				}
				case DetourEventType::SetFileIoOverlappedRange:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileIoOverlappedRange(result);
					break;
				}
				case DetourEventType::SetFilePointer:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnSetFilePointer(result);
					break;
				}
				case DetourEventType::SetFilePointerEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFilePointerEx(result);
					break;
				}
				case DetourEventType::SetFileTime:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileTime(result);
					break;
				}
				case DetourEventType::SetFileValidData:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileValidData(result);
					break;
				}
				case DetourEventType::UnlockFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnUnlockFile(result);
					break;
				}
				case DetourEventType::UnlockFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnUnlockFileEx(result);
					break;
				}
				case DetourEventType::WriteFile:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnWriteFile(result);
					break;
				}
				case DetourEventType::WriteFileEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnWriteFileEx(result);
					break;
				}
				case DetourEventType::WriteFileGather:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnWriteFileGather(result);
					break;
				}
				
				// LibLoaderApi
				case DetourEventType::LoadLibraryA:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_monitor->OnLoadLibraryA(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_monitor->OnLoadLibraryW(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryExA:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_monitor->OnLoadLibraryExA(libFileName);
					break;
				}
				case DetourEventType::LoadLibraryExW:
				{
					auto libFileName = ReadWStringValue(message, offset);
					m_monitor->OnLoadLibraryExW(libFileName);
					break;
				}

				// ProcessEnv
				case DetourEventType::SearchPathA:
				{
					auto path = ReadStringValue(message, offset);
					auto fileName = ReadStringValue(message, offset);
					auto extension = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnSearchPathA(path, fileName, extension, result);
					break;
				}
				case DetourEventType::SearchPathW:
				{
					auto path = ReadWStringValue(message, offset);
					auto fileName = ReadWStringValue(message, offset);
					auto extension = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnSearchPathW(path, fileName, extension, result);
					break;
				}

				// ProcessThreadsApi
				case DetourEventType::CreateProcessA:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessA(wasDetoured, applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessW:
				{
					auto wasDetoured = ReadBoolValue(message, offset);
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessW(wasDetoured, applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessAsUserA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessAsUserA(applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessAsUserW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessAsUserW(applicationName, result);
					break;
				}
				case DetourEventType::ExitProcess:
				{
					auto exitCode = ReadUInt32Value(message, offset);
					m_monitor->OnExitProcess(exitCode);
					break;
				}

				// UndocumentedApi
				case DetourEventType::PrivCopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnPrivCopyFileExA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::PrivCopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnPrivCopyFileExW(existingFileName, newFileName, result, wasBlocked);
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
					m_monitor->OnCopyFileA(existingFileName, newFileName, failIfExists, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto failIfExists = ReadBoolValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFileW(existingFileName, newFileName, failIfExists, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFile2:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFile2(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFileExA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFileExW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFileTransactedA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CopyFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCopyFileTransactedW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryExA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryExA(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryExW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryExW(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryTransactedA:
				{
					auto templateDirectory = ReadStringValue(message, offset);
					auto newDirectory = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryTransactedA(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateDirectoryTransactedW:
				{
					auto templateDirectory = ReadWStringValue(message, offset);
					auto newDirectory = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateDirectoryTransactedW(templateDirectory, newDirectory, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateFileTransactedA(fileName, desiredAccess, shareMode, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto desiredAccess = ReadUInt32Value(message, offset);
					auto shareMode = ReadUInt32Value(message, offset);
					auto result = ReadUInt64Value(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnCreateFileTransactedW(fileName, desiredAccess, shareMode, result, wasBlocked);
					break;
				}
				case DetourEventType::CreateHardLinkA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateHardLinkA(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateHardLinkW(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto existingFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateHardLinkTransactedA(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateHardLinkTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto existingFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateHardLinkTransactedW(fileName, existingFileName, result);
					break;
				}
				case DetourEventType::CreateProcessWithLogonW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessWithLogonW(applicationName, result);
					break;
				}
				case DetourEventType::CreateProcessWithTokenW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateProcessWithTokenW(applicationName, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateSymbolicLinkA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateSymbolicLinkW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkTransactedA:
				{
					auto symlinkFileName = ReadStringValue(message, offset);
					auto targetFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateSymbolicLinkTransactedA(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::CreateSymbolicLinkTransactedW:
				{
					auto symlinkFileName = ReadWStringValue(message, offset);
					auto targetFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnCreateSymbolicLinkTransactedW(symlinkFileName, targetFileName, flags, result);
					break;
				}
				case DetourEventType::DecryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnDecryptFileA(fileName, result);
					break;
				}
				case DetourEventType::DecryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnDecryptFileW(fileName, result);
					break;
				}
				case DetourEventType::DeleteFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnDeleteFileTransactedA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::DeleteFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnDeleteFileTransactedW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::EncryptFileA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnEncryptFileA(fileName, result);
					break;
				}
				case DetourEventType::EncryptFileW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnEncryptFileW(fileName, result);
					break;
				}
				case DetourEventType::FileEncryptionStatusA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFileEncryptionStatusA(fileName, result);
					break;
				}
				case DetourEventType::FileEncryptionStatusW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFileEncryptionStatusW(fileName, result);
					break;
				}
				case DetourEventType::FindFirstFileNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnFindFirstFileNameTransactedW(fileName, result);
					break;
				}
				case DetourEventType::FindFirstFileTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					m_monitor->OnFindFirstFileTransactedA(fileName);
					break;
				}
				case DetourEventType::FindFirstFileTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstFileTransactedW(fileName);
					break;
				}
				case DetourEventType::FindFirstStreamTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					m_monitor->OnFindFirstStreamTransactedW(fileName);
					break;
				}
				case DetourEventType::GetBinaryTypeA:
				{
					auto applicationName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetBinaryTypeA(applicationName, result);
					break;
				}
				case DetourEventType::GetBinaryTypeW:
				{
					auto applicationName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetBinaryTypeW(applicationName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetCompressedFileSizeTransactedA(fileName, result);
					break;
				}
				case DetourEventType::GetCompressedFileSizeTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetCompressedFileSizeTransactedW(fileName, result);
					break;
				}
				case DetourEventType::GetDllDirectoryA:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetDllDirectoryA(result);
					break;
				}
				case DetourEventType::GetDllDirectoryW:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetDllDirectoryW(result);
					break;
				}
				case DetourEventType::GetEnvironmentVariableA:
				{
					auto name = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetEnvironmentVariableA(name, result);
					break;
				}
				case DetourEventType::GetEnvironmentVariableW:
				{
					auto name = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetEnvironmentVariableW(name, result);
					break;
				}
				case DetourEventType::GetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesTransactedA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileAttributesTransactedW(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetFileBandwidthReservation(result);
					break;
				}
				case DetourEventType::GetFileInformationByHandleEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetFileInformationByHandleEx(result);
					break;
				}
				case DetourEventType::GetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnGetFileSecurityA(fileName, result, wasBlocked);
					break;
				}
				case DetourEventType::GetFullPathNameTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFullPathNameTransactedA(fileName, result);
					break;
				}
				case DetourEventType::GetFullPathNameTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetFullPathNameTransactedW(fileName, result);
					break;
				}
				case DetourEventType::GetLongPathNameTransactedA:
				{
					auto shortPath = ReadStringValue(message, offset);
					auto longPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLongPathNameTransactedA(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetLongPathNameTransactedW:
				{
					auto shortPath = ReadWStringValue(message, offset);
					auto longPath = ReadWStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetLongPathNameTransactedW(shortPath, longPath, result);
					break;
				}
				case DetourEventType::GetQueuedCompletionStatus:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetQueuedCompletionStatus(result);
					break;
				}
				case DetourEventType::GetQueuedCompletionStatusEx:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnGetQueuedCompletionStatusEx(result);
					break;
				}
				case DetourEventType::GetShortPathNameA:
				{
					auto longPath = ReadStringValue(message, offset);
					auto shortPath = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnGetShortPathNameA(longPath, shortPath, result);
					break;
				}
				case DetourEventType::LoadModule:
				{
					auto moduleName = ReadStringValue(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnLoadModule(moduleName, result);
					break;
				}
				case DetourEventType::LoadPackagedLibrary:
				{
					auto libFileName = ReadStringValue(message, offset);
					m_monitor->OnLoadPackagedLibrary(libFileName);
					break;
				}
				case DetourEventType::MoveFileA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileA(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileW(existingFileName, newFileName, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileExA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileExA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileExW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileExW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileTransactedA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileTransactedA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileTransactedW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileTransactedW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileWithProgressA:
				{
					auto existingFileName = ReadStringValue(message, offset);
					auto newFileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileWithProgressA(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::MoveFileWithProgressW:
				{
					auto existingFileName = ReadWStringValue(message, offset);
					auto newFileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnMoveFileWithProgressW(existingFileName, newFileName, flags, result, wasBlocked);
					break;
				}
				case DetourEventType::OpenEncryptedFileRawA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnOpenEncryptedFileRawA(fileName, flags, result);
					break;
				}
				case DetourEventType::OpenEncryptedFileRawW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnOpenEncryptedFileRawW(fileName, flags, result);
					break;
				}
				case DetourEventType::OpenFile:
				{
					auto fileName = ReadStringValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnOpenFile(fileName, wasBlocked);
					break;
				}
				case DetourEventType::OpenFileById:
				{
					m_monitor->OnOpenFileById();
					break;
				}
				case DetourEventType::ReadEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnReadEncryptedFileRaw(result);
					break;
				}
				case DetourEventType::RemoveDirectoryTransactedA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnRemoveDirectoryTransactedA(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::RemoveDirectoryTransactedW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnRemoveDirectoryTransactedW(pathName, result, wasBlocked);
					break;
				}
				case DetourEventType::ReOpenFile:
				{
					m_monitor->OnReOpenFile();
					break;
				}
				case DetourEventType::ReplaceFileA:
				{
					auto replacedFileName = ReadStringValue(message, offset);
					auto replacementFileName = ReadStringValue(message, offset);
					auto backupFileName = ReadStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnReplaceFileA(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourEventType::ReplaceFileW:
				{
					auto replacedFileName = ReadWStringValue(message, offset);
					auto replacementFileName = ReadWStringValue(message, offset);
					auto backupFileName = ReadWStringValue(message, offset);
					auto replaceFlags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnReplaceFileW(replacedFileName, replacementFileName, backupFileName, replaceFlags, result);
					break;
				}
				case DetourEventType::SetCurrentDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetCurrentDirectoryA(pathName, result);
					break;
				}
				case DetourEventType::SetCurrentDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetCurrentDirectoryW(pathName, result);
					break;
				}
				case DetourEventType::SetDllDirectoryA:
				{
					auto pathName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetDllDirectoryA(pathName, result);
					break;
				}
				case DetourEventType::SetDllDirectoryW:
				{
					auto pathName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetDllDirectoryW(pathName, result);
					break;
				}
				case DetourEventType::SetFileAttributesTransactedA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnSetFileAttributesTransactedA(fileName, fileAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileAttributesTransactedW:
				{
					auto fileName = ReadWStringValue(message, offset);
					auto fileAttributes = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					auto wasBlocked = ReadBoolValue(message, offset);
					m_monitor->OnSetFileAttributesTransactedW(fileName, fileAttributes, result, wasBlocked);
					break;
				}
				case DetourEventType::SetFileBandwidthReservation:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileBandwidthReservation(result);
					break;
				}
				case DetourEventType::SetFileCompletionNotificationModes:
				{
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileCompletionNotificationModes(result);
					break;
				}
				case DetourEventType::SetFileSecurityA:
				{
					auto fileName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileSecurityA(fileName, result);
					break;
				}
				case DetourEventType::SetFileShortNameA:
				{
					auto shortName = ReadStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileShortNameA(shortName, result);
					break;
				}
				case DetourEventType::SetFileShortNameW:
				{
					auto shortName = ReadWStringValue(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetFileShortNameW(shortName, result);
					break;
				}
				case DetourEventType::SetSearchPathMode:
				{
					auto flags = ReadUInt32Value(message, offset);
					auto result = ReadBoolValue(message, offset);
					m_monitor->OnSetSearchPathMode(flags, result);
					break;
				}
				case DetourEventType::WriteEncryptedFileRaw:
				{
					auto result = ReadUInt32Value(message, offset);
					m_monitor->OnWriteEncryptedFileRaw(result);
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
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadBoolValue missing required field");
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadBoolValue past end of content");
			return result > 0;
		}

		int32_t ReadInt32Value(Message& message, uint32_t& offset)
		{
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadInt32Value missing required field");
			auto result = *reinterpret_cast<int32_t*>(message.Content + offset);
			offset += sizeof(int32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadInt32Value past end of content");
			return result;
		}

		uint32_t ReadUInt32Value(Message& message, uint32_t& offset)
		{
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadUInt32Value missing required field");
			auto result = *reinterpret_cast<uint32_t*>(message.Content + offset);
			offset += sizeof(uint32_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt32Value past end of content");
			return result;
		}

		uint64_t ReadUInt64Value(Message& message, uint32_t& offset)
		{
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadUInt64Value missing required field");
			auto result = *reinterpret_cast<uint64_t*>(message.Content + offset);
			offset += sizeof(uint64_t);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadUInt64Value past end of content");
			return result;
		}

		std::string_view ReadStringValue(Message& message, uint32_t& offset)
		{
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadStringValue missing required field");
			auto result = std::string_view(reinterpret_cast<char*>(message.Content + offset));
			offset += static_cast<uint32_t>(result.size()) + 1;
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadStringValue past end of content");
			return result;
		}

		std::wstring_view ReadWStringValue(Message& message, uint32_t& offset)
		{
			if (offset >= message.ContentSize)
				throw std::runtime_error("ReadWStringValue missing required field");
			auto result = std::wstring_view(reinterpret_cast<wchar_t*>(message.Content + offset));
			offset += 2 * (static_cast<uint32_t>(result.size()) + 1);
			if (offset > message.ContentSize)
				throw std::runtime_error("ReadWStringValue read past end of content");
			return result;
		}
	};
}
