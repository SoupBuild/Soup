#pragma once

static bool s_bLog = 1;
static LONG s_nTlsIndent = -1;
static LONG s_nTlsThread = -1;
static LONG s_nThreadCnt = 0;

//////////////////////////////////////////////////////////////////////////////
//
// DLL module information
//
bool ThreadAttach(HMODULE hDll)
{
	(void)hDll;

	if (s_nTlsIndent >= 0)
	{
		TlsSetValue(s_nTlsIndent, (PVOID)0);
	}

	if (s_nTlsThread >= 0)
	{
		LONG nThread = InterlockedIncrement(&s_nThreadCnt);
		TlsSetValue(s_nTlsThread, (PVOID)(LONG_PTR)nThread);
	}

	return true;
}

bool ThreadDetach(HMODULE hDll)
{
	if (s_nTlsIndent >= 0)
	{
		TlsSetValue(s_nTlsIndent, (PVOID)0);
	}

	if (s_nTlsThread >= 0)
	{
		TlsSetValue(s_nTlsThread, (PVOID)0);
	}

	return true;
}
