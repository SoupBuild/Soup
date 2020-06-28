#pragma once

#include "EventLogger.h"

static HMODULE s_hInst = nullptr;
static HMODULE s_hKernel32 = nullptr;
static CHAR s_szDllPath[MAX_PATH];
static TBLOG_PAYLOAD s_Payload;
static TBLOG_PAYLOAD s_ChildPayload;
static CRITICAL_SECTION s_csChildPayload;
static DWORD s_nTraceProcessId = 0;
static LONG s_nChildCnt = 0;
static PWCHAR s_pwEnvironment = nullptr;
static DWORD s_cwEnvironment = 0;
static PCHAR s_pbEnvironment = nullptr;
static DWORD s_cbEnvironment = 0;

static EventLogger s_eventLogger;

static CRITICAL_SECTION s_csPipe; // Guards access to hPipe.
static HANDLE s_hPipe = INVALID_HANDLE_VALUE;
static TBLOG_MESSAGE s_rMessage;

PBYTE LoadFile(HANDLE hFile, DWORD cbFile);
static PCHAR RemoveReturns(PCHAR pszBuffer);
static PWCHAR RemoveReturns(PWCHAR pwzBuffer);

void AssertFailed(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine);

static void Copy(PWCHAR pwzDst, PCWSTR pwzSrc)
{
	while (*pwzSrc)
	{
		*pwzDst++ = *pwzSrc++;
	}

	*pwzDst = '\0';
}

static DWORD Size(PCWSTR pwzSrc)
{
	DWORD c = 0;
	while (pwzSrc[c])
	{
		c++;
	}

	return c;
}

static PCWSTR Save(PCWSTR pwzSrc)
{
	DWORD c = (Size(pwzSrc) + 1) * sizeof(WCHAR);
	PWCHAR pwzDst = (PWCHAR)GlobalAlloc(GPTR, c);
	CopyMemory(pwzDst, pwzSrc, c);

	return pwzDst;
}

static bool HasSpace(PCWSTR pwz)
{
	for (; *pwz; pwz++)
	{
		if (*pwz == ' ' || *pwz == '\t' || *pwz == '\r' || *pwz == '\n')
		{
			return true;
		}
	}

	return false;
}

static bool HasChar(PCWSTR pwz, WCHAR w)
{
	for (; *pwz; pwz++)
	{
		if (*pwz == w)
		{
			return true;
		}
	}

	return false;
}

static DWORD Compare(PCWSTR pwzA, PCWSTR pwzB)
{
	for (;;)
	{
		WCHAR cA = *pwzA++;
		WCHAR cB = *pwzB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

static DWORD Compare(PCWSTR pwzA, PCSTR pszB)
{
	for (;;)
	{
		WCHAR cA = *pwzA++;
		WCHAR cB = *pszB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

static DWORD Compare(PCSTR pszA, PCSTR pszB)
{
	for (;;)
	{
		CHAR cA = *pszA++;
		CHAR cB = *pszB++;

		if (cA >= 'A' && cA <= 'Z')
		{
			cA += ('a' - 'A');
		}

		if (cB >= 'A' && cB <= 'Z')
		{
			cB += ('a' - 'A');
		}

		if (cA == 0 && cB == 0)
		{
			return 0;
		}

		if (cA != cB)
		{
			return cA - cB;
		}
	}
}

/////////////////////////////////////////////////////////////
// Detours
//

bool CreateProcessInternals(
	HANDLE hProcess)
{
	EnterCriticalSection(&s_csChildPayload);

	ZeroMemory(&s_ChildPayload, sizeof(s_ChildPayload));
	CopyMemory(&s_ChildPayload, &s_Payload, sizeof(s_ChildPayload));

	s_ChildPayload.nParentProcessId = GetCurrentProcessId();
	s_ChildPayload.rGeneology[s_ChildPayload.nGeneology]
		= (DWORD)InterlockedIncrement(&s_nChildCnt);
	s_ChildPayload.nGeneology++;

	DetourCopyPayloadToProcess(hProcess, s_guidTrace, &s_ChildPayload, sizeof(s_ChildPayload));

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}