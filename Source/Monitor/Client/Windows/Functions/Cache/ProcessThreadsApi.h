#pragma once

namespace Monitor::Windows::Functions::Cache::ProcessThreadsApi
{
	/// <summary>
	/// Creates a new process and its primary thread. The new process runs
	/// in the security context of the calling process.
	/// </summary>
	BOOL (WINAPI* CreateProcessA)(
		LPCSTR lpApplicationName,
		LPSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCSTR lpCurrentDirectory,
		LPSTARTUPINFOA lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessA;
	BOOL (WINAPI* CreateProcessW)(
		LPCWSTR lpApplicationName,
		LPWSTR lpCommandLine,
		LPSECURITY_ATTRIBUTES lpProcessAttributes,
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		BOOL bInheritHandles,
		DWORD dwCreationFlags,
		LPVOID lpEnvironment,
		LPCWSTR lpCurrentDirectory,
		LPSTARTUPINFOW lpStartupInfo,
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessW;

	/// <summary>
	/// Creates a new process and its primary thread. The new process runs
	/// in the security context of the user represented by the specified token.
	/// </summary>
	BOOL (WINAPI* CreateProcessAsUserA)(
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
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessAsUserA;
	BOOL (WINAPI* CreateProcessAsUserW)(
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
		LPPROCESS_INFORMATION lpProcessInformation) = ::CreateProcessAsUserW;

	/// <summary>
	/// Ends the calling process and all its threads.
	/// </summary>
	void (WINAPI* ExitProcess)(
		UINT uExitCode) = ::ExitProcess;
}