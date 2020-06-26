module;

#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>

#define _WIN32_WINNT 0x0500
#define WIN32
#define NT

#define DBG_TRACE 0

#include <windows.h>
#include <stdio.h>
#pragma warning(push)
#if _MSC_VER > 1400
#pragma warning(disable:6102 6103) // /analyze warnings
#endif
#include <strsafe.h>
#pragma warning(pop)

#define DllExport __declspec( dllexport )
#define PULONG_PTR PVOID
#define PLONG_PTR PVOID
#define ULONG_PTR PVOID
#define DEBUG_BREAK() DebugBreak()

export module Monitor.Detours;
import Detours;
import Monitor.Shared;

#include "FunctionsCache.h"
#include "FunctionsOverride.h"

// TODO: This exported method forces a lib to be generated to allow for linking...
// Soup should allow runtime references to dlls that have no external symbols
DllExport int ForceLib()
{
	return 1;
}

void AttachDetours()
{
	ThrowIfFailed(DetourTransactionBegin(), "AttachDetours DetourTransactionBegin Failed");
	ThrowIfFailed(DetourUpdateThread(GetCurrentThread()), "AttachDetours DetourUpdateThread Failed");

	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::EntryPoint, Functions::Override::EntryPoint), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ExitProcess, Functions::Override::ExitProcess), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CopyFileExA, Functions::Override::CopyFileExA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CopyFileExW, Functions::Override::CopyFileExW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::PrivCopyFileExW, Functions::Override::PrivCopyFileExW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateHardLinkA, Functions::Override::CreateHardLinkA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateHardLinkW, Functions::Override::CreateHardLinkW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateDirectoryW, Functions::Override::CreateDirectoryW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateDirectoryExW, Functions::Override::CreateDirectoryExW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateFileW, Functions::Override::CreateFileW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreatePipe, Functions::Override::CreatePipe), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateFileMappingW, Functions::Override::CreateFileMappingW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CloseHandle, Functions::Override::CloseHandle), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::DuplicateHandle, Functions::Override::DuplicateHandle), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateProcessW, Functions::Override::CreateProcessW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::CreateProcessA, Functions::Override::CreateProcessA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::DeleteFileW, Functions::Override::DeleteFileW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::DeviceIoControl, Functions::Override::DeviceIoControl), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::GetFileAttributesW, Functions::Override::GetFileAttributesW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::MoveFileA, Functions::Override::MoveFileA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::MoveFileW, Functions::Override::MoveFileW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::MoveFileExA, Functions::Override::MoveFileExA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::MoveFileExW, Functions::Override::MoveFileExW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::MoveFileWithProgressW, Functions::Override::MoveFileWithProgressW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::SetStdHandle, Functions::Override::SetStdHandle), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LoadLibraryA, Functions::Override::LoadLibraryA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LoadLibraryW, Functions::Override::LoadLibraryW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LoadLibraryExA, Functions::Override::LoadLibraryExA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::LoadLibraryExW, Functions::Override::LoadLibraryExW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::SetFilePointer, Functions::Override::SetFilePointer), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::SetFilePointerEx, Functions::Override::SetFilePointerEx), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ReadFile, Functions::Override::ReadFile), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ReadFileEx, Functions::Override::ReadFileEx), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WriteFile, Functions::Override::WriteFile), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WriteFileEx, Functions::Override::WriteFileEx), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WriteConsoleA, Functions::Override::WriteConsoleA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::WriteConsoleW, Functions::Override::WriteConsoleW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ExpandEnvironmentStringsA, Functions::Override::ExpandEnvironmentStringsA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::ExpandEnvironmentStringsW, Functions::Override::ExpandEnvironmentStringsW), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::GetEnvironmentVariableA, Functions::Override::GetEnvironmentVariableA), "AttachDetours DetourAttach Failed");
	ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Cache::GetEnvironmentVariableW, Functions::Override::GetEnvironmentVariableW), "AttachDetours DetourAttach Failed");

	ThrowIfFailed(DetourTransactionCommit(), "AttachDetours DetourTransactionCommit Failed");
}

LONG DetachDetours(void)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DetourDetach(&(PVOID&)Functions::Cache::EntryPoint, Functions::Override::EntryPoint);
	DetourDetach(&(PVOID&)Functions::Cache::ExitProcess, Functions::Override::ExitProcess);
	DetourDetach(&(PVOID&)Functions::Cache::CopyFileExA, Functions::Override::CopyFileExA);
	DetourDetach(&(PVOID&)Functions::Cache::CopyFileExW, Functions::Override::CopyFileExW);
	DetourDetach(&(PVOID&)Functions::Cache::PrivCopyFileExW, Functions::Override::PrivCopyFileExW);
	DetourDetach(&(PVOID&)Functions::Cache::CreateHardLinkA, Functions::Override::CreateHardLinkA);
	DetourDetach(&(PVOID&)Functions::Cache::CreateHardLinkW, Functions::Override::CreateHardLinkW);
	DetourDetach(&(PVOID&)Functions::Cache::CreateDirectoryW, Functions::Override::CreateDirectoryW);
	DetourDetach(&(PVOID&)Functions::Cache::CreateDirectoryExW, Functions::Override::CreateDirectoryExW);
	DetourDetach(&(PVOID&)Functions::Cache::CreateFileW, Functions::Override::CreateFileW);
	DetourDetach(&(PVOID&)Functions::Cache::CreatePipe, Functions::Override::CreatePipe);
	DetourDetach(&(PVOID&)Functions::Cache::CreateFileMappingW, Functions::Override::CreateFileMappingW);
	DetourDetach(&(PVOID&)Functions::Cache::CloseHandle, Functions::Override::CloseHandle);
	DetourDetach(&(PVOID&)Functions::Cache::DuplicateHandle, Functions::Override::DuplicateHandle);

	// TODO: Why can't I replace create process when debugger is attached in VS?
	// if (!IsDebuggerPresent())
	{
		DetourDetach(&(PVOID&)Functions::Cache::CreateProcessW, Functions::Override::CreateProcessW);
		DetourDetach(&(PVOID&)Functions::Cache::CreateProcessA, Functions::Override::CreateProcessA);
	}

	DetourDetach(&(PVOID&)Functions::Cache::DeleteFileW, Functions::Override::DeleteFileW);
	DetourDetach(&(PVOID&)Functions::Cache::DeviceIoControl, Functions::Override::DeviceIoControl);
	DetourDetach(&(PVOID&)Functions::Cache::GetFileAttributesW, Functions::Override::GetFileAttributesW);
	DetourDetach(&(PVOID&)Functions::Cache::MoveFileA, Functions::Override::MoveFileA);
	DetourDetach(&(PVOID&)Functions::Cache::MoveFileW, Functions::Override::MoveFileW);
	DetourDetach(&(PVOID&)Functions::Cache::MoveFileExA, Functions::Override::MoveFileExA);
	DetourDetach(&(PVOID&)Functions::Cache::MoveFileExW, Functions::Override::MoveFileExW);
	DetourDetach(&(PVOID&)Functions::Cache::MoveFileWithProgressW, Functions::Override::MoveFileWithProgressW);
	DetourDetach(&(PVOID&)Functions::Cache::SetStdHandle, Functions::Override::SetStdHandle);
	DetourDetach(&(PVOID&)Functions::Cache::LoadLibraryA, Functions::Override::LoadLibraryA);
	DetourDetach(&(PVOID&)Functions::Cache::LoadLibraryW, Functions::Override::LoadLibraryW);
	DetourDetach(&(PVOID&)Functions::Cache::LoadLibraryExA, Functions::Override::LoadLibraryExA);
	DetourDetach(&(PVOID&)Functions::Cache::LoadLibraryExW, Functions::Override::LoadLibraryExW);
	DetourDetach(&(PVOID&)Functions::Cache::SetFilePointer, Functions::Override::SetFilePointer);
	DetourDetach(&(PVOID&)Functions::Cache::SetFilePointerEx, Functions::Override::SetFilePointerEx);
	DetourDetach(&(PVOID&)Functions::Cache::ReadFile, Functions::Override::ReadFile);
	DetourDetach(&(PVOID&)Functions::Cache::ReadFileEx, Functions::Override::ReadFileEx);
	DetourDetach(&(PVOID&)Functions::Cache::WriteFile, Functions::Override::WriteFile);
	DetourDetach(&(PVOID&)Functions::Cache::WriteFileEx, Functions::Override::WriteFileEx);
	DetourDetach(&(PVOID&)Functions::Cache::WriteConsoleA, Functions::Override::WriteConsoleA);
	DetourDetach(&(PVOID&)Functions::Cache::WriteConsoleW, Functions::Override::WriteConsoleW);
	DetourDetach(&(PVOID&)Functions::Cache::ExpandEnvironmentStringsA, Functions::Override::ExpandEnvironmentStringsA);
	DetourDetach(&(PVOID&)Functions::Cache::ExpandEnvironmentStringsW, Functions::Override::ExpandEnvironmentStringsW);
	DetourDetach(&(PVOID&)Functions::Cache::GetEnvironmentVariableA, Functions::Override::GetEnvironmentVariableA);
	DetourDetach(&(PVOID&)Functions::Cache::GetEnvironmentVariableW, Functions::Override::GetEnvironmentVariableW);

	return DetourTransactionCommit();
}

bool ProcessAttach(HMODULE hDll)
{
	InitializeCriticalSection(&s_csPipe);
	InitializeCriticalSection(&s_csChildPayload);

	Procs::Initialize();
	EnvVars::Initialize();
	FileNames::Initialize();
	OpenFiles::Initialize();

	s_bLog = false;
	s_nTlsIndent = TlsAlloc();
	s_nTlsThread = TlsAlloc();

	s_hInst = hDll;
	s_hKernel32 = nullptr;

	PBYTE xCreate = (PBYTE)DetourCodeFromPointer((PVOID)Functions::Cache::CreateProcessW, nullptr);
	TBLOG_PAYLOAD* pPayload = nullptr;

	for (HMODULE hMod = nullptr; (hMod = DetourEnumerateModules(hMod)) != nullptr;)
	{
		ULONG cbData;
		PVOID pvData = DetourFindPayload(hMod, s_guidTrace, &cbData);

		if (pvData != nullptr && pPayload == nullptr)
		{
			pPayload = (TBLOG_PAYLOAD*)pvData;
		}

		ULONG cbMod = DetourGetModuleSize(hMod);

		if (((PBYTE)hMod) < xCreate && ((PBYTE)hMod + cbMod) > xCreate)
		{
			s_hKernel32 = hMod;
		}
	}

	ZeroMemory(&s_Payload, sizeof(s_Payload));

	if (pPayload == nullptr)
	{
		return false;
	}

	CopyMemory(&s_Payload, pPayload, sizeof(s_Payload));

	LoadStdHandleName(STD_INPUT_HANDLE, s_Payload.wzStdin, false);
	LoadStdHandleName(STD_OUTPUT_HANDLE, s_Payload.wzStdout, s_Payload.fStdoutAppend);
	LoadStdHandleName(STD_ERROR_HANDLE, s_Payload.wzStderr, s_Payload.fStderrAppend);
	s_nTraceProcessId = s_Payload.nTraceProcessId;

	GetModuleFileNameA(s_hInst, s_szDllPath, ARRAYSIZE(s_szDllPath));

	// Find hidden functions.
	Functions::Cache::PrivCopyFileExW =
		(BOOL (WINAPI *)(LPCWSTR, LPCWSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
		GetProcAddress(s_hKernel32, "PrivCopyFileExW");
	if (Functions::Cache::PrivCopyFileExW == nullptr)
	{
		DEBUG_BREAK();
	}

	try
	{
		AttachDetours();
	}
	catch (const std::exception& ex)
	{
		DEBUG_BREAK();
		Tblog("<!-- Error attaching detours: %s -->\n", ex.what());
	}
	catch (...)
	{
		DEBUG_BREAK();
		Tblog("<!-- Error attaching detours -->\n");
	}

	ThreadAttach(hDll);

	s_bLog = true;
	return true;
}

bool ProcessDetach(HMODULE hDll)
{
	ThreadDetach(hDll);
	s_bLog = false;

	LONG error = DetachDetours();
	if (error != NO_ERROR)
	{
		Tblog("<!-- Error detaching detours: %d -->\n", error);
	}

	TblogClose();

	if (s_nTlsIndent >= 0)
	{
		TlsFree(s_nTlsIndent);
	}

	if (s_nTlsThread >= 0)
	{
		TlsFree(s_nTlsThread);
	}

	return true;
}

bool APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

#ifdef ENABLE_MONITOR_DEBUG
	while (!IsDebuggerPresent())
	{
		Sleep(0);
	}
#endif

	if (DetourIsHelperProcess())
	{
		return true;
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();
		Functions::Cache::EntryPoint = (int (WINAPI *)(void))DetourGetEntryPoint(nullptr);
		return ProcessAttach(hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		return ProcessDetach(hModule);
	}
	else if (dwReason == DLL_THREAD_ATTACH)
	{
		return ThreadAttach(hModule);
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		return ThreadDetach(hModule);
	}

	return true;
}