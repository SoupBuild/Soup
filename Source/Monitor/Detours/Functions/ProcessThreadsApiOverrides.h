#pragma once
#include "ProcessThreadsApiCache.h"

namespace Functions::ProcessThreadsApi::Overrides
{
	BOOL WINAPI CreateProcessA(
		LPCSTR lpApplicationName,
		LPSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCSTR lpCurrentDirectory,
		LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			// If the caller did not request the process information then create a temporary
			// one for ourselves
			LPPROCESS_INFORMATION lpInternalProcessInformation = lpProcessInformation;
			PROCESS_INFORMATION privateProcessInformation;
			if (lpInternalProcessInformation == nullptr)
			{
				lpInternalProcessInformation = &privateProcessInformation;
			}

			// Pass along the request with an initially suspended process
			result = DetourCreateProcessWithDllExA(
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags | CREATE_SUSPENDED,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpInternalProcessInformation,
				s_szDllPath,
				Cache::CreateProcessA);

			if (result)
			{
				// Perform the detour setup
				CreateProcessInternals(lpInternalProcessInformation->hProcess);

				// If the caller did not create the process suspended then undo our override
				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(lpInternalProcessInformation->hThread);
				}

				// Cleanup if we used the private information store
				if (lpInternalProcessInformation == &privateProcessInformation)
				{
					CloseHandle(privateProcessInformation.hThread);
					CloseHandle(privateProcessInformation.hProcess);
				}
			}
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessA;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateProcessW(
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			// If the caller did not request the process information then create a temporary
			// one for ourselves
			LPPROCESS_INFORMATION lpInternalProcessInformation = lpProcessInformation;
			PROCESS_INFORMATION privateProcessInformation;
			if (lpInternalProcessInformation == nullptr)
			{
				lpInternalProcessInformation = &privateProcessInformation;
			}

			// Pass along the request with an initially suspended process
			result = DetourCreateProcessWithDllExW(
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags | CREATE_SUSPENDED,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpInternalProcessInformation,
				s_szDllPath,
				Cache::CreateProcessW);

			if (result)
			{
				// Perform the detour setup
				CreateProcessInternals(lpInternalProcessInformation->hProcess);

				// If the caller did not create the process suspended then undo our override
				if (!(dwCreationFlags & CREATE_SUSPENDED))
				{
					ResumeThread(lpInternalProcessInformation->hThread);
				}

				// Cleanup if we used the private information store
				if (lpInternalProcessInformation == &privateProcessInformation)
				{
					CloseHandle(privateProcessInformation.hThread);
					CloseHandle(privateProcessInformation.hProcess);
				}
			}
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessW;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);
		}

		return result;
	}

	BOOL WINAPI CreateProcessAsUserA(
		HANDLE hToken,
		LPCSTR lpApplicationName,
		LPSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCSTR lpCurrentDirectory,
		LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateProcessAsUserA(
				hToken,
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpProcessInformation);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessAsUserA;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);

			// TODO
			EventLogger::WriteError("Cannot detour process from CreateProcessAsUserA");
		}

		return result;
	}

	BOOL WINAPI CreateProcessAsUserW(
		HANDLE hToken,
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation)
	{
		BOOL result = 0;
		__try
		{
			result = Cache::CreateProcessAsUserW(
				hToken,
				lpApplicationName,
				lpCommandLine,
				lpProcessAttributes,
				lpThreadAttributes,
				bInheritHandles,
				dwCreationFlags,
				lpEnvironment,
				lpCurrentDirectory,
				lpStartupInfo,
				lpProcessInformation);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::CreateProcessAsUserW;
			EventLogger::AppendValue(message, lpApplicationName);
			EventLogger::AppendValue(message, result);
			EventLogger::WriteMessage(message);

			// TODO
			EventLogger::WriteError("Cannot detour process from CreateProcessAsUserW");
		}

		return result;
	}

	void WINAPI ExitProcess(UINT uExitCode)
	{
		__try
		{
			Cache::ExitProcess(uExitCode);
		}
		__finally
		{
			auto message = Monitor::DetourMessage();
			message.Type = Monitor::DetourMessageType::ExitProcess;
			EventLogger::AppendValue(message, uExitCode);
			EventLogger::WriteMessage(message);
		}
	}
}