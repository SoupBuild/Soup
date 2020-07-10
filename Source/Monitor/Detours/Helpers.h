#pragma once

#include "EventLogger.h"

static HMODULE s_hInst = nullptr;
static HMODULE s_hKernel32 = nullptr;
static CHAR s_szDllPath[MAX_PATH];
static Monitor::DetourPayload s_Payload;
static Monitor::DetourPayload s_ChildPayload;
static CRITICAL_SECTION s_csChildPayload;
static DWORD s_nTraceProcessId = 0;
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

	DetourCopyPayloadToProcess(hProcess, Monitor::GuidTrace, &s_ChildPayload, sizeof(s_ChildPayload));

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}
