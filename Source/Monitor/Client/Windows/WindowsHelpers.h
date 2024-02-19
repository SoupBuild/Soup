#pragma once

static HMODULE s_hInst = nullptr;
static HMODULE s_hKernel32 = nullptr;
static CHAR s_szDllPath[MAX_PATH];
static Monitor::ProcessPayload s_Payload;
static Monitor::ProcessPayload s_ChildPayload;
static CRITICAL_SECTION s_csChildPayload;
static LONG s_nChildCnt = 0;

bool CreateProcessInternals(
	HANDLE hProcess)
{
	EnterCriticalSection(&s_csChildPayload);

	ZeroMemory(&s_ChildPayload, sizeof(s_ChildPayload));
	CopyMemory(&s_ChildPayload, &s_Payload, sizeof(s_ChildPayload));

	s_ChildPayload.nParentProcessId = GetCurrentProcessId();
	s_ChildPayload.rGeneology[s_ChildPayload.nGeneology] =
		(DWORD)InterlockedIncrement(&s_nChildCnt);
	s_ChildPayload.nGeneology++;

	DetourCopyPayloadToProcess(
		hProcess,
		Monitor::ProcessPayloadResourceId,
		&s_ChildPayload,
		sizeof(s_ChildPayload));

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}

bool IsWhiteListProcess(std::string_view applicationName)
{
	// VCTIP is the Visual Studio telemetry service, they really need to stop looking at user folders...
	return EndsWithIgnoreCase(applicationName, "VCTIP.EXE");
}

bool IsWhiteListProcess(std::wstring_view applicationName)
{
	// VCTIP is the Visual Studio telemetry service, they really need to stop looking at user folders...
	return EndsWithIgnoreCase(applicationName, L"VCTIP.EXE");
}