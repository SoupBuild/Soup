module;

#include <fstream>
#include <iostream>
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

#include "OriginalFunctions.h"

// TODO: This exported method forces a lib to be generated to allow for linking...
// Soup should allow runtime references to dlls that have no external symbols
DllExport int ForceLib()
{
	return 1;
}

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

static CRITICAL_SECTION s_csPipe; // Guards access to hPipe.
static HANDLE s_hPipe = INVALID_HANDLE_VALUE;
static TBLOG_MESSAGE s_rMessage;

// Logging Functions.
//
void Tblog(PCSTR pszMsgf, ...);
void TblogV(PCSTR pszMsgf, va_list args);

void VSafePrintf(PCSTR pszMsg, va_list args, PCHAR pszBuffer, LONG cbBuffer);
PCHAR SafePrintf(PCHAR pszBuffer, LONG cbBuffer, PCSTR pszMsg, ...);

LONG EnterFunc();
void ExitFunc();
void Print(PCSTR psz, ...);
void NoteRead(PCSTR psz);
void NoteRead(PCWSTR pwz);
void NoteWrite(PCSTR psz);
void NoteWrite(PCWSTR pwz);
void NoteDelete(PCSTR psz);
void NoteDelete(PCWSTR pwz);
void NoteCleanup(PCSTR psz);
void NoteCleanup(PCWSTR pwz);

PBYTE LoadFile(HANDLE hFile, DWORD cbFile);
static PCHAR RemoveReturns(PCHAR pszBuffer);
static PWCHAR RemoveReturns(PWCHAR pwzBuffer);

void AssertFailed(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine);

int WINAPI Mine_EntryPoint(void);
void WINAPI Mine_ExitProcess(UINT a0);

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

//////////////////////////////////////////////////////////////////////////////

static PCSTR s_rpszMsvcrNames[] = {
	"msvcr80.dll",
	"msvcr80d.dll",
	"msvcr71.dll",
	"msvcr71d.dll",
	"msvcr70.dll",
	"msvcr70d.dll",
	nullptr,
};

HMODULE s_hMsvcr = nullptr;
PCSTR s_pszMsvcr = nullptr;

static BOOL WINAPI ImportFileCallback(PVOID pContext, HMODULE hFile, PCSTR pszFile)
{
	if (pszFile != nullptr)
	{
		for (int i = 0; s_rpszMsvcrNames[i]; i++)
		{
			if (Compare(pszFile, s_rpszMsvcrNames[i]) == 0)
			{
				s_hMsvcr = hFile;
				s_pszMsvcr = s_rpszMsvcrNames[i];
				return false;
			}
		}
	}

	return true;
}

bool FindMsvcr()
{
	DetourEnumerateImports(nullptr, nullptr, ImportFileCallback, nullptr);

	if (s_hMsvcr != nullptr)
	{
		return true;
	}

	return false;
}

bool FindProc(PVOID * ppvCode, PCSTR pwzFunc)
{
	PVOID pv = GetProcAddress(s_hMsvcr, pwzFunc);
	if (pv != nullptr)
	{
		*ppvCode = pv;
		return true;
	}
	else
	{
		*ppvCode = nullptr;
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////
//
struct EnvInfo
{
	DWORD m_nHash;
	DWORD m_nIndex;
	PCWSTR m_pwzVar;
	PCWSTR m_pwzVal;
	bool m_fDefined;
	bool m_fUsed;
	bool m_fOriginal;
};

//////////////////////////////////////////////////////////////////////////////
//
class EnvVars
{
private:
	static CRITICAL_SECTION s_csLock;
	static DWORD s_nVars;
	static DWORD s_nCapacity;
	static EnvInfo** s_pVars;

private:
	static DWORD Hash(PCWSTR pwzVar)
	{
		DWORD hash = 5381;
		while (*pwzVar != 0)
		{
			WCHAR c = *pwzVar++;
			if (c >= 'A' && c <= 'Z')
			{
				c += ('a' - 'A');
			}

			hash = ((hash << 5) + hash) + c;
		}

		return hash;
	}

	static void LockAcquire()
	{
		EnterCriticalSection(&s_csLock);
	}

	static void LockRelease()
	{
		LeaveCriticalSection(&s_csLock);
	}

	static void Resize(DWORD nCapacity);
	static void Set(EnvInfo *info);
	static EnvInfo * Find(PCWSTR pwzVar);

public:
	static bool Equal(PCWSTR pwzA, PCWSTR pwzB)
	{
		return (Compare(pwzA, pwzB) == 0);
	}

public:
	static void Initialize();
	static void Dump();

	static void Add(PCWSTR pwzVar, PCWSTR pwzVal);

	static void Used(PCWSTR pwzVar);
	static void Used(PCSTR pszVar);
};

CRITICAL_SECTION EnvVars::s_csLock;
DWORD EnvVars::s_nVars = 0;
DWORD EnvVars::s_nCapacity = 0;
EnvInfo ** EnvVars::s_pVars = nullptr;

void EnvVars::Initialize()
{
	InitializeCriticalSection(&s_csLock);

	Resize(919);
}

void EnvVars::Resize(DWORD nCapacity)
{
	if (nCapacity > s_nCapacity)
	{
		DWORD nOld = s_nCapacity;
		EnvInfo ** pOld = s_pVars;

		// DEBUG_BREAK();

		s_pVars = (EnvInfo **)GlobalAlloc(GPTR, nCapacity * sizeof(EnvInfo *));
		s_nCapacity = nCapacity;

		if (pOld != nullptr)
		{
			for (DWORD n = 0; n < nOld; n++)
			{
				if (pOld[n] != nullptr)
				{
					Set(pOld[n]);
				}
			}

			GlobalFree((HGLOBAL)pOld);
			pOld = nullptr;
		}
	}
}

void EnvVars::Set(EnvInfo *info)
{
	DWORD hash = info->m_nHash;
	DWORD slot = hash % s_nCapacity;
	DWORD death = 0;

	// Find an empty slot.
	for (; s_pVars[slot] != nullptr; slot = (slot + 1) % s_nCapacity)
	{
		if (++death > s_nCapacity)
		{
			// We should have dropped out at some point...
			DEBUG_BREAK();
		}
	}

	s_pVars[slot] = info;
}

EnvInfo * EnvVars::Find(PCWSTR pwzVar)
{
	DWORD hash = Hash(pwzVar);
	DWORD slot = hash % s_nCapacity;

	LockAcquire();

	// Find the the matching slot, or an empty one.
	for (; s_pVars[slot] != nullptr; slot = (slot + 1) % s_nCapacity)
	{
		if (Equal(s_pVars[slot]->m_pwzVar, pwzVar))
		{
			LockRelease();
			return s_pVars[slot];
		}
	}

	LockRelease();
	return nullptr;
}

void EnvVars::Add(PCWSTR pwzVar, PCWSTR pwzVal)
{
	if (pwzVar == nullptr)
	{
		return;
	}

	WCHAR wzVar[MAX_PATH];
	PWCHAR pwzDst = wzVar;
	while (*pwzVar)
	{
		if (*pwzVar >= 'a' && *pwzVar <= 'z') {
			*pwzDst++ = *pwzVar - ('a' - 'A');
		}
		else {
			*pwzDst++ = *pwzVar;
		}
		pwzVar++;
	}

	*pwzDst = '\0';
	pwzVar = wzVar;

	WCHAR wzVal[] = L"";
	if (pwzVal != nullptr)
	{
		while (*pwzVal == ' ' || *pwzVal == '\t')
		{
			*pwzVal++;
		}
	}
	else
	{
		pwzVal = wzVal;
	}

	// Tblog("<!-- ::Add var=[%le] val=[%le] -->\n", pwzVar, pwzVal);
	LockAcquire();

	// DEBUG_BREAK();

	DWORD hash = Hash(pwzVar);
	DWORD slot = hash % s_nCapacity;
	EnvInfo *info = nullptr;
	DWORD death = 0;

	// Find the the matching slot, or an empty one.
	for (; s_pVars[slot] != nullptr; slot = (slot + 1) % s_nCapacity)
	{
		if (Equal(s_pVars[slot]->m_pwzVar, pwzVar))
		{
			LockRelease();
			return;
		}
		if (++death > s_nCapacity)
		{
			// We should have dropped out at some point...
			DEBUG_BREAK();
		}
	}

	// Add the var to list of known vars.
	info = (EnvInfo *)GlobalAlloc(GPTR, sizeof(EnvInfo));
	info->m_nHash = hash;
	info->m_nIndex = s_nVars++;
	info->m_pwzVar = Save(pwzVar);
	info->m_pwzVal = Save(pwzVal);
	if (pwzVal[0] == '\0')
	{
		info->m_fDefined = false;
		info->m_fUsed = true;
	}
	else
	{
		info->m_fDefined = true;
	}

	s_pVars[slot] = info;

	// Check if we should grow the table.
	if (s_nVars > (s_nCapacity / 2))
	{
		Resize(s_nCapacity * 2 - 1);
	}

	LockRelease();
}

void EnvVars::Used(PCWSTR pwzVar)
{
	if (pwzVar != nullptr)
	{
		// Tblog("<!-- Used [%le] -->\n", pwzVar);
		EnvInfo *pInfo = Find(pwzVar);
		if (pInfo)
		{
			pInfo->m_fUsed = true;
		}
#if 0
		else
		{
			Add(pwzVar, nullptr);
		}
#endif
	}
}

void EnvVars::Used(PCSTR pszVar)
{
	if (pszVar != nullptr)
	{
		WCHAR wzVar[MAX_PATH];
		PWCHAR pwzVar = wzVar;
		while (*pszVar)
		{
			*pwzVar++ = *pszVar++;
		}

		*pwzVar = '\0';

		Used(wzVar);
	}
}

void EnvVars::Dump()
{
	if (s_nVars == 0)
	{
		return;
	}

	LockAcquire();

	Tblog("<t:Vars>\n");

	// Remove any variables that match the original environment.
	PCWSTR pwzz = s_Payload.wzzEnvironment;
	while (*pwzz)
	{
		WCHAR wzVar[MAX_PATH];
		PWCHAR pwzVar = wzVar;

		while (*pwzz && *pwzz != '=')
		{
			*pwzVar++ = *pwzz++;
		}

		*pwzVar = '\0';
		if (*pwzz == '=')
		{
			pwzz++;
		}

		EnvInfo *pInfo = Find(wzVar);
		if (pInfo)
		{
			if (Compare(pwzz, pInfo->m_pwzVal) == 0)
			{
				pInfo->m_fUsed = false;
			}
		}

		pwzz += Size(pwzz) + 1;
	}

	EnvInfo ** pSorted = (EnvInfo **)GlobalAlloc(GPTR, s_nVars * sizeof(EnvInfo *));

	for (DWORD n = 0; n < s_nCapacity; n++)
	{
		if (s_pVars[n] != nullptr) {
			if (s_pVars[n]->m_nIndex > s_nVars) {
				DEBUG_BREAK();
			}
			pSorted[s_pVars[n]->m_nIndex] = s_pVars[n];
		}
	}

	for (DWORD n = 0; n < s_nVars; n++)
	{
		EnvInfo *pInfo = pSorted[n];

		if (pInfo == nullptr)
		{
			Print("<!-- Warning: Missing %d of %d -->\n", n, s_nVars);
			continue;
		}

		if (pInfo->m_fUsed && pInfo->m_pwzVal[0])
		{
			Print("<t:Var var=\"%le\">%le</t:Var>\n", pInfo->m_pwzVar, pInfo->m_pwzVal);
		}
	}

	GlobalFree((HGLOBAL)pSorted);

	Tblog("</t:Vars>\n");

	LockRelease();
}

void SaveEnvironment()
{
	LPWCH pwStrings = GetEnvironmentStringsW();
	PCWSTR pwEnv = (PCWSTR)pwStrings;

	while (*pwEnv != '\0')
	{
		WCHAR wzVar[MAX_PATH];
		PWCHAR pwzDst = wzVar;
		PCWSTR pwzVal = nullptr;

		if (*pwEnv == '=')
		{
			*pwzDst++ = *pwEnv++;
		}

		while (*pwEnv != '\0' && *pwEnv != '=')
		{
			*pwzDst++ = *pwEnv++;
		}

		*pwzDst++ = '\0';

		if (*pwEnv == '=')
		{
			pwEnv++;
		}

		pwzVal = pwEnv;
		while (*pwEnv != '\0')
		{
			pwEnv++;
		}

		if (*pwEnv == '\0')
		{
			pwEnv++;
		}

		if (wzVar[0] != '=')
		{
			EnvVars::Add(wzVar, pwzVal);
		}
	}

	FreeEnvironmentStringsW(pwStrings);
}

//////////////////////////////////////////////////////////////////////////////
//
struct ProcInfo
{
	HANDLE m_hProc;
	DWORD m_nProcId;
	DWORD m_nProc;
};

class Procs
{
private:
	static CRITICAL_SECTION s_csLock;
	static DWORD s_nProcs;
	static ProcInfo s_rProcs[4049];

private:
	static ProcInfo& HashToSlot(HANDLE handle)
	{
		return s_rProcs[((DWORD_PTR)handle) % ARRAYSIZE(s_rProcs)];
	}

	static void LockAcquire()
	{
		EnterCriticalSection(&s_csLock);
	}

	static void LockRelease()
	{
		LeaveCriticalSection(&s_csLock);
	}

public:
	static void Initialize();
	static ProcInfo * Create(HANDLE hProc, DWORD nProcId);
	static bool Close(HANDLE hProc);
};

CRITICAL_SECTION Procs::s_csLock;
DWORD Procs::s_nProcs = 0;
ProcInfo Procs::s_rProcs[4049];

void Procs::Initialize()
{
	InitializeCriticalSection(&s_csLock);
	for (DWORD i = 0; i < ARRAYSIZE(s_rProcs); i++)
	{
		s_rProcs[i].m_hProc = INVALID_HANDLE_VALUE;
	}
}

ProcInfo * Procs::Create(HANDLE hProc, DWORD nProcId)
{
	LockAcquire();
	s_nProcs++;
	ProcInfo& slot = HashToSlot(hProc);
	slot.m_hProc = hProc;
	slot.m_nProcId = nProcId;
	slot.m_nProc = s_nProcs;
	Print("<!-- CreateProcess (%d)-->\n", slot.m_nProc);
	LockRelease();

	return &slot;
}

bool Procs::Close(HANDLE hProc)
{
	bool first = false;

	LockAcquire();
	ProcInfo& slot = HashToSlot(hProc);
	if (slot.m_hProc == hProc)
	{
		first = true;
		Print("<!-- CloseProcess (%d)-->\n", slot.m_nProc);
		slot.m_hProc = INVALID_HANDLE_VALUE;
		slot.m_nProcId = 0;
		slot.m_nProc = 0;
		s_nProcs--;
	}

	LockRelease();

	return first;
}

//////////////////////////////////////////////////////////////////////////////
//
struct FileInfo
{
	DWORD m_nHash;
	DWORD m_nIndex;

	bool m_fCantRead; // Set for file that are opened Create
	bool m_fRead;
	bool m_fWrite;

	bool m_fDelete;
	bool m_fCleanup;
	bool m_fSystemPath;
	bool m_fTemporaryPath;
	bool m_fTemporaryFile;

	DWORD m_cbRead;
	DWORD m_cbWrite;

	bool m_fAppend;
	bool m_fAbsorbed; // Absorbed by TraceBld.
	bool m_fDirectory;

	PCWSTR m_pwzPath;
	PBYTE m_pbContent;
	DWORD m_cbContent;
};

//////////////////////////////////////////////////////////////////////////////
//
class FileNames
{
private:
	static CRITICAL_SECTION s_csLock;
	static DWORD s_nFiles;
	static DWORD s_nCapacity;
	static FileInfo ** s_pFiles;

public:
	static WCHAR s_wzSysPath[MAX_PATH];
	static WCHAR s_wzS64Path[MAX_PATH];
	static WCHAR s_wzTmpPath[MAX_PATH];
	static WCHAR s_wzExePath[MAX_PATH];
	static DWORD s_wcSysPath;
	static DWORD s_wcS64Path;
	static DWORD s_wcTmpPath;
	static DWORD s_wcExePath;

private:
	static DWORD Hash(PCWSTR pwzFile)
	{
		DWORD hash = 5381;
		while (*pwzFile != 0)
		{
			WCHAR c = *pwzFile++;
			if (c >= 'A' && c <= 'Z')
			{
				c += ('a' - 'A');
			}

			hash = ((hash << 5) + hash) + c;
		}

		return hash;
	}

	static void LockAcquire()
	{
		EnterCriticalSection(&s_csLock);
	}

	static void LockRelease()
	{
		LeaveCriticalSection(&s_csLock);
	}

	static void Resize(DWORD nCapacity);
	static void Set(FileInfo *info);
	static void Replace(PWCHAR pwzBuffer, PWCHAR pwzDstEnd, DWORD cwOld, PCWSTR pwzNew);

public:
	static bool Equal(PCWSTR pwzA, PCWSTR pwzB)
	{
		return (Compare(pwzA, pwzB) == 0);
	}

	static bool PrefixMatch(PCWSTR pwzFile, PCWSTR pwzPrefix)
	{
		for (;;)
		{
			WCHAR cFile = *pwzFile++;
			WCHAR cPrefix = *pwzPrefix++;

			if (cFile >= 'A' && cFile <= 'Z')
			{
				cFile += ('a' - 'A');
			}

			if (cPrefix >= 'A' && cPrefix <= 'Z')
			{
				cPrefix += ('a' - 'A');
			}

			if (cPrefix == 0)
			{
				return true;
			}

			if (cFile != cPrefix)
			{
				return false;
			}
		}
	}

	static bool SuffixMatch(PCWSTR pwzFile, PCWSTR pwzSuffix)
	{
		// Move both pointers to the end of the strings.
		PCWSTR pwzFileBeg = pwzFile;
		while (*pwzFile)
		{
			pwzFile++;
		}

		PCWSTR pwzSuffixBeg = pwzSuffix;
		while (*pwzSuffix)
		{
			pwzSuffix++;
		}

		// Now walk backwards comparing strings.
		for (;;)
		{
			WCHAR cFile = (pwzFile > pwzFileBeg) ? *--pwzFile : 0;
			WCHAR cSuffix = (pwzSuffix > pwzSuffixBeg) ? *--pwzSuffix : 0;

			if (cFile >= 'A' && cFile <= 'Z')
			{
				cFile += ('a' - 'A');
			}

			if (cSuffix >= 'A' && cSuffix <= 'Z')
			{
				cSuffix += ('a' - 'A');
			}

			if (cSuffix == 0)
			{
				return true;
			}

			if (cFile != cSuffix)
			{
				return false;
			}
		}
	}

	static void EndInSlash(PWCHAR pwzPath)
	{
		if (*pwzPath)
		{
			while (*pwzPath)
			{
				pwzPath++;
			}

			if (pwzPath[-1] != '\\')
			{
				*pwzPath++ = '\\';
				*pwzPath = '\0';
			}
		}
	}

public:
	static void Initialize();
	static void Dump();
	static FileInfo * FindPartial(PCWSTR pwzPath);
	static FileInfo * FindPartial(PCSTR pszPath);
	static FileInfo * FindFull(PCWSTR pwzPath);
	static PCWSTR ParameterizeName(PWCHAR pwzDst, DWORD cMaxDst, PCWSTR pwzPath);
	static PCWSTR ParameterizeName(PWCHAR pwzDst, DWORD cMaxDst, FileInfo *pInfo);
	static void ParameterizeLine(PWCHAR pwzDst, PWCHAR pwzDstEnd);
};

CRITICAL_SECTION FileNames::s_csLock;
DWORD FileNames::s_nFiles = 0;
DWORD FileNames::s_nCapacity = 0;
FileInfo** FileNames::s_pFiles;
WCHAR FileNames::s_wzSysPath[MAX_PATH];
WCHAR FileNames::s_wzS64Path[MAX_PATH];
WCHAR FileNames::s_wzTmpPath[MAX_PATH];
WCHAR FileNames::s_wzExePath[MAX_PATH];
DWORD FileNames::s_wcSysPath;
DWORD FileNames::s_wcS64Path;
DWORD FileNames::s_wcTmpPath;
DWORD FileNames::s_wcExePath;

void FileNames::Initialize()
{
	InitializeCriticalSection(&s_csLock);

	s_wzSysPath[0] = '\0';
	GetSystemDirectoryW(s_wzSysPath, ARRAYSIZE(s_wzSysPath));
	EndInSlash(s_wzSysPath);

	s_wzS64Path[0] = '\0';
	GetWindowsDirectoryW(s_wzS64Path, ARRAYSIZE(s_wzS64Path));
	EndInSlash(s_wzS64Path);
	Copy(s_wzS64Path + Size(s_wzS64Path), L"SysWOW64\\");

	s_wzTmpPath[0] = '\0';
	GetTempPathW(ARRAYSIZE(s_wzTmpPath), s_wzTmpPath);
	EndInSlash(s_wzTmpPath);

	s_wzExePath[0] = '\0';
	GetModuleFileNameW(nullptr, s_wzExePath, ARRAYSIZE(s_wzExePath));
	PWCHAR pwzLast = s_wzExePath;
	for (PWCHAR pwz = s_wzExePath; *pwz; pwz++)
	{
		if (*pwz == '\\')
		{
			pwzLast = pwz;
		}
	}

	if (*pwzLast == '\\')
	{
		*++pwzLast = '\0';
	}

	s_wcSysPath = Size(s_wzSysPath);
	s_wcS64Path = Size(s_wzS64Path);
	s_wcTmpPath = Size(s_wzTmpPath);
	s_wcExePath = Size(s_wzExePath);

	Resize(4049);
}

void FileNames::Resize(DWORD nCapacity)
{
	if (nCapacity > s_nCapacity)
	{
		DWORD nOld = s_nCapacity;
		FileInfo ** pOld = s_pFiles;

		s_pFiles = (FileInfo **)GlobalAlloc(GPTR, nCapacity * sizeof(FileInfo *));
		s_nCapacity = nCapacity;

		if (pOld != nullptr)
		{
			for (DWORD n = 0; n < nOld; n++)
			{
				if (pOld[n] != nullptr)
				{
					Set(pOld[n]);
				}
			}

			GlobalFree((HGLOBAL)pOld);
			pOld = nullptr;
		}

		s_nCapacity = nCapacity;
	}
}

void FileNames::Set(FileInfo *info)
{
	DWORD hash = info->m_nHash;
	DWORD slot = hash % s_nCapacity;
	DWORD death = 0;

	// Find an empty slot.
	for (; s_pFiles[slot] != nullptr; slot = (slot + 1) % s_nCapacity)
	{
		if (++death > s_nCapacity)
		{
			// We should have dropped out at some point...
			DEBUG_BREAK();
		}
	}

	s_pFiles[slot] = info;
}

FileInfo* FileNames::FindFull(PCWSTR pwzPath)
{
	if (pwzPath == nullptr)
	{
		return nullptr;
	}

	LockAcquire();

	DWORD hash = Hash(pwzPath);
	DWORD slot = hash % s_nCapacity;
	FileInfo *info = nullptr;
	DWORD death = 0;

	// Find the the matching slot, or an empty one.
	for (; s_pFiles[slot] != nullptr; slot = (slot + 1) % s_nCapacity)
	{
		if (Equal(s_pFiles[slot]->m_pwzPath, pwzPath))
		{
			info = s_pFiles[slot];
			goto succeed;
		}

		if (++death > s_nCapacity)
		{
			// We should have dropped out at some point...
			DEBUG_BREAK();
		}
	}

	// Add the file to list of known files.
	info = (FileInfo *)GlobalAlloc(GPTR, sizeof(FileInfo));
	info->m_nHash = hash;
	info->m_nIndex = s_nFiles++;
	info->m_pwzPath = Save(pwzPath);
	info->m_fSystemPath = (PrefixMatch(info->m_pwzPath, s_wzSysPath) ||
							PrefixMatch(info->m_pwzPath, s_wzS64Path));
	info->m_fTemporaryPath = PrefixMatch(info->m_pwzPath, s_wzTmpPath);
	info->m_fTemporaryFile = SuffixMatch(info->m_pwzPath, L".tmp");

	s_pFiles[slot] = info;

	// Check if we should grow the table.
	if (s_nFiles > (s_nCapacity / 2))
	{
		Resize(s_nCapacity * 2 - 1);
	}

succeed:
	LockRelease();

	return info;
}

FileInfo * FileNames::FindPartial(PCWSTR pwzPath)
{
	WCHAR wzPath[MAX_PATH];
	PWCHAR pwzFile = nullptr;

	if (!GetFullPathNameW(pwzPath, ARRAYSIZE(wzPath), wzPath, &pwzFile))
	{
		return FindFull(pwzPath);
	}
	else
	{
		return FindFull(wzPath);
	}
}

FileInfo * FileNames::FindPartial(PCSTR pwzPath)
{
	WCHAR wzPath[MAX_PATH];
	PWCHAR pwzFile = wzPath;

	while (*pwzPath)
	{
		*pwzFile++ = *pwzPath++;
	}

	*pwzFile = '\0';

	return FindPartial(wzPath);
}

PCWSTR FileNames::ParameterizeName(PWCHAR pwzDst, DWORD cMaxDst, FileInfo *pInfo)
{
	return ParameterizeName(pwzDst, cMaxDst, pInfo->m_pwzPath);
}

PCWSTR FileNames::ParameterizeName(PWCHAR pwzDst, DWORD cMaxDst, PCWSTR pwzPath)
{
	if (PrefixMatch(pwzPath, s_wzSysPath))
	{
		Copy(pwzDst, L"%SYSDIR%\\");
		Copy(pwzDst + Size(pwzDst), pwzPath + s_wcSysPath);
		goto finish;
	}
	else if (PrefixMatch(pwzPath, s_wzS64Path))
	{
		Copy(pwzDst, L"%SYSDIR%\\");
		Copy(pwzDst + Size(pwzDst), pwzPath + s_wcS64Path);
		goto finish;
	}
	else if (PrefixMatch(pwzPath, s_wzTmpPath))
	{
		Copy(pwzDst, L"%TMPDIR%\\");
		Copy(pwzDst + Size(pwzDst), pwzPath + s_wcTmpPath);
		goto finish;
	}
	else
	{
		Copy(pwzDst, pwzPath);

		finish:
#if 0 // to convert to all lower case.
		for (PWCHAR pwz = pwzDst; *pwz && pwz < pwzDst + cMaxDst; pwz++)
		{
			if (*pwz >= 'A' && *pwz <= 'Z')
			{
				*pwz = 'a' + (*pwz - 'A');
			}
		}
#else
		(void)cMaxDst;
#endif
		return pwzDst;
	}
}

void FileNames::Replace(PWCHAR pwzDst, PWCHAR pwzDstEnd, DWORD cwOld, PCWSTR pwzNew)
{
	DWORD cwNew = Size(pwzNew);
	DWORD cwDst = Size(pwzDst);

	if (cwOld < cwNew)
	{
		// We have to insert.
		if ((cwDst + cwNew - cwOld) >= (DWORD)(pwzDstEnd - pwzDst))
		{
			// Won't fit, so abort.
			return;
		}

		PWCHAR pwzTo = pwzDst + cwDst + (cwNew - cwOld);
		PWCHAR pwzFm = pwzDst + cwDst;

		while (pwzTo >= pwzDst)
		{
			*pwzTo-- = *pwzFm--;
		}
	}
	else if (cwOld > cwNew)
	{
		// We have to remove.
		PWCHAR pwzTo = pwzDst + cwNew;
		PWCHAR pwzFm = pwzDst + cwOld;

		while (*pwzFm)
		{
			*pwzTo++ = *pwzFm++;
		}

		*pwzTo = '\0';
	}

	// Now write the new string.
	while (*pwzNew)
	{
		*pwzDst++ = *pwzNew++;
	}
}

void FileNames::ParameterizeLine(PWCHAR pwzDst, PWCHAR pwzDstEnd)
{
	for (; *pwzDst != '\0'; pwzDst++)
	{
		if (PrefixMatch(pwzDst, s_wzSysPath))
		{
			Replace(pwzDst, pwzDstEnd, s_wcSysPath, L"%SYSDIR%\\");
		}
		else if (PrefixMatch(pwzDst, s_wzS64Path))
		{
			Replace(pwzDst, pwzDstEnd, s_wcS64Path, L"%SYSDIR%\\");
		}
		else if (PrefixMatch(pwzDst, s_wzTmpPath))
		{
			Replace(pwzDst, pwzDstEnd, s_wcTmpPath, L"%TMPDIR%\\");
		}
	}
}

void FileNames::Dump()
{
	WCHAR wzPath[MAX_PATH];

	if (s_nFiles == 0)
	{
		return;
	}

	LockAcquire();

	Tblog("<t:Files>\n");

	FileInfo ** pSorted = (FileInfo **)GlobalAlloc(GPTR, s_nFiles * sizeof(FileInfo *));

	for (DWORD n = 0; n < s_nCapacity; n++)
	{
		if (s_pFiles[n] != nullptr)
		{
			if (s_pFiles[n]->m_nIndex > s_nFiles)
			{
				DEBUG_BREAK();
			}

			pSorted[s_pFiles[n]->m_nIndex] = s_pFiles[n];
		}
	}

	for (DWORD n = 0; n < s_nFiles; n++)
	{
		FileInfo *pInfo = pSorted[n];

		if (pInfo == nullptr)
		{
			Print("<!-- Warning: Missing %d of %d -->\n", n, s_nFiles);
			continue;
		}

		bool fRead = pInfo->m_fRead;
		bool fWrite = pInfo->m_fWrite;
		bool fDelete = (pInfo->m_fDelete);
		bool fCleanup = (pInfo->m_fCleanup);
		bool fAppend = (pInfo->m_fAppend);

	#if 0
		if (fDelete && !fRead && !fWrite)
		{
			Print("<!-- Discarding: %ls -->\n", pInfo->m_pwzPath);
			// Discard pipe files only passed to children.
			continue;
		}
	#endif
		if (pInfo->m_fAbsorbed)
		{
			// Discard response fles
			continue;
		}

		if (PrefixMatch(pInfo->m_pwzPath, s_wzExePath) ||
			PrefixMatch(pInfo->m_pwzPath, s_wzSysPath) ||
			PrefixMatch(pInfo->m_pwzPath, s_wzS64Path))
		{
			// Discard files from exec directory (because considered internal to code).
			continue;
		}

	#if 1 // Ignore PIPEs.
		if (FileNames::PrefixMatch(pInfo->m_pwzPath, L"\\\\.\\PIPE\\"))
		{
			continue;
		}
	#endif
		if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\conout$"))
		{
			continue;
		}

		if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\conin$"))
		{
			continue;
		}

		if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\nul"))
		{
			continue;
		}

		ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo);

		if (pInfo->m_fDirectory)
		{
			Print("<t:File mkdir=\"true\">%ls</t:File>\n", wzPath);
			continue;
		}

		if (!fRead && !fWrite && !fDelete && !fCleanup)
		{
			// Discard do "none" files.
			continue;
		}

		if (pInfo->m_pbContent == nullptr ||
			pInfo->m_fDelete ||
			pInfo->m_fCleanup ||
			pInfo->m_fWrite)
		{
			Print(
				"<t:File%s%s%s%s%s>%ls</t:File>\n",
				fRead ? " read=\"true\"" : "",
				fWrite ? " write=\"true\"" : "",
				fDelete ? " delete=\"true\"" : "",
				fCleanup ? " cleanup=\"true\"" : "",
				fAppend ? " append=\"true\"" : "",
				// size=\"%d\" pInfo->m_cbContent,
				wzPath);
		}
		else if ((pInfo->m_pbContent)[0] == 0xff && (pInfo->m_pbContent)[1] == 0xfe)
		{
			// Unicode
			Print(
				"<t:File%s%s%s%s%s>%ls<t:Data>%le</t:Data></t:File>\n",
				fRead ? " read=\"true\"" : "",
				fWrite ? " write=\"true\"" : "",
				fDelete ? " delete=\"true\"" : "",
				fCleanup ? " cleanup=\"true\"" : "",
				fAppend ? " append=\"true\"" : "",
				//  size=\"%d\" pInfo->m_cbContent,
				wzPath,
				RemoveReturns((PWCHAR)pInfo->m_pbContent));
		}
		else
		{
			// Ascii
			Print(
				"<t:File%s%s%s%s%s>%ls<t:Data>%he</t:Data></t:File>\n",
				fRead ? " read=\"true\"" : "",
				fWrite ? " write=\"true\"" : "",
				fDelete ? " delete=\"true\"" : "",
				fCleanup ? " cleanup=\"true\"" : "",
				fAppend ? " append=\"true\"" : "",
				//  size=\"%d\" pInfo->m_cbContent,
				wzPath,
				RemoveReturns((PCHAR)pInfo->m_pbContent));
		}

		if (pInfo->m_pbContent != nullptr)
		{
			GlobalFree((HGLOBAL)pInfo->m_pbContent);
			pInfo->m_pbContent = nullptr;
		}
	}

	GlobalFree((HGLOBAL)pSorted);

	Tblog("</t:Files>\n");

	LockRelease();
}


//////////////////////////////////////////////////////////////////////////////
//
class OpenFiles
{
private:
	struct SLOT
	{
		HANDLE m_hHandle;
		FileInfo* m_pFile;
		ProcInfo* m_pProc;
	};

private:
	static CRITICAL_SECTION s_csLock;
	static DWORD s_nHandles;
	static SLOT s_rHandles[4049];

private:
	static SLOT& HashToSlot(HANDLE handle)
	{
		return s_rHandles[((DWORD_PTR)handle) % ARRAYSIZE(s_rHandles)];
	}

	static void LockAcquire()
	{
		EnterCriticalSection(&s_csLock);
	}

	static void LockRelease()
	{
		LeaveCriticalSection(&s_csLock);
	}

public:
	static void Initialize();

	static void SetWrite(HANDLE hFile, DWORD cbData)
	{
		SLOT& slot = HashToSlot(hFile);
		if (slot.m_hHandle == hFile)
		{
			slot.m_pFile->m_fWrite = true;
			slot.m_pFile->m_cbWrite += cbData;
		}
	}

	static void SetRead(HANDLE hFile, DWORD cbData)
	{
		SLOT& slot = HashToSlot(hFile);
		if (slot.m_hHandle == hFile)
		{
			slot.m_pFile->m_fRead = true;
			slot.m_pFile->m_cbRead += cbData;
		}
	}

	static bool Forget(HANDLE handle);
	static bool Remember(HANDLE hFile, FileInfo *pInfo);
	static bool Remember(HANDLE hProc, ProcInfo *pInfo);
	static FileInfo* RecallFile(HANDLE hFile);
	static ProcInfo* RecallProc(HANDLE hProc);
};

CRITICAL_SECTION OpenFiles::s_csLock; // Guards access to OpenFile stuctures.
DWORD OpenFiles::s_nHandles = 0;
OpenFiles::SLOT OpenFiles::s_rHandles[4049];

void OpenFiles::Initialize()
{
	InitializeCriticalSection(&s_csLock);
	for (DWORD n = 0; n < ARRAYSIZE(s_rHandles); n++)
	{
		s_rHandles[n].m_hHandle = INVALID_HANDLE_VALUE;
		s_rHandles[n].m_pFile = nullptr;
		s_rHandles[n].m_pProc = nullptr;
	}
}

bool OpenFiles::Forget(HANDLE handle)
{
	LockAcquire();
	OpenFiles::SLOT& slot = HashToSlot(handle);

	if (slot.m_hHandle == handle)
	{
		slot.m_hHandle = INVALID_HANDLE_VALUE;
		slot.m_pFile = nullptr;
		slot.m_pProc = nullptr;
		s_nHandles--;
	}

	LockRelease();
	return false;
}

bool OpenFiles::Remember(HANDLE hFile, FileInfo *pFile)
{
	LockAcquire();

	OpenFiles::SLOT& slot = HashToSlot(hFile);
	if (slot.m_hHandle != hFile && slot.m_hHandle != INVALID_HANDLE_VALUE)
	{
		// hash collision
		DEBUG_BREAK();
	}

	slot.m_hHandle = hFile;
	slot.m_pFile = pFile;
	slot.m_pProc = nullptr;
	s_nHandles++;

	LockRelease();

	return true;
}

bool OpenFiles::Remember(HANDLE hProc, ProcInfo *pProc)
{
	LockAcquire();

	OpenFiles::SLOT& slot = HashToSlot(hProc);
	if (slot.m_hHandle != hProc && slot.m_hHandle != INVALID_HANDLE_VALUE)
	{
		// hash collision
		DEBUG_BREAK();
	}

	slot.m_hHandle = hProc;
	slot.m_pProc = pProc;
	slot.m_pFile = nullptr;
	s_nHandles++;

	LockRelease();

	return true;
}

FileInfo * OpenFiles::RecallFile(HANDLE hFile)
{
	LockAcquire();

	OpenFiles::SLOT& slot = HashToSlot(hFile);

	if (slot.m_hHandle == hFile)
	{
		LockRelease();
		return slot.m_pFile;
	}

	LockRelease();
	return nullptr;
}

ProcInfo * OpenFiles::RecallProc(HANDLE hProc)
{
	LockAcquire();

	OpenFiles::SLOT& slot = HashToSlot(hProc);

	if (slot.m_hHandle == hProc)
	{
		LockRelease();
		return slot.m_pProc;
	}

	LockRelease();
	return nullptr;
}

///////////////////////////////////////////////////////////////////// VPrintf.
//
// Completely side-effect free printf replacement (but no FP numbers).
//
static PCHAR do_base(PCHAR pszOut, UINT64 nValue, UINT nBase, PCSTR pszDigits)
{
	CHAR szTmp[96];
	int nDigit = sizeof(szTmp)-2;
	for (; nDigit >= 0; nDigit--)
	{
		szTmp[nDigit] = pszDigits[nValue % nBase];
		nValue /= nBase;
	}

	for (nDigit = 0; nDigit < sizeof(szTmp) - 2 && szTmp[nDigit] == '0'; nDigit++)
	{
		// skip leading zeros.
	}

	for (; nDigit < sizeof(szTmp) - 1; nDigit++)
	{
		*pszOut++ = szTmp[nDigit];
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_str(PCHAR pszOut, PCHAR pszEnd, PCSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		*pszOut++ = *pszIn++;
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_wstr(PCHAR pszOut, PCHAR pszEnd, PCWSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		*pszOut++ = (CHAR)*pszIn++;
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_estr(PCHAR pszOut, PCHAR pszEnd, PCSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		if (*pszIn == '<')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'l';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '>')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'g';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '&')
		{
			if (pszOut + 5 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'm';
			*pszOut++ = 'p';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\"')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'q';
			*pszOut++ = 'u';
			*pszOut++ = 'o';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\'')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'p';
			*pszOut++ = 'o';
			*pszOut++ = 's';
			*pszOut++ = ';';
		}
		else if (*pszIn  < ' ')
		{
			BYTE c = (BYTE)(*pszIn++);
			if (c < 10 && pszOut + 4 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else if (c < 100 && pszOut + 5 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + ((c / 10) % 10);
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else if (c < 1000 && pszOut + 6 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + ((c / 100) % 10);
				*pszOut++ = '0' + ((c / 10) % 10);
				*pszOut++ = '0' + (c % 10);
				*pszOut++ = ';';
			}
			else
			{
				break;
			}
		}
		else
		{
			*pszOut++ = *pszIn++;
		}
	}

	*pszOut = '\0';
	return pszOut;
}

static PCHAR do_ewstr(PCHAR pszOut, PCHAR pszEnd, PCWSTR pszIn)
{
	while (*pszIn && pszOut < pszEnd)
	{
		if (*pszIn == '<')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'l';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '>')
		{
			if (pszOut + 4 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'g';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '&')
		{
			if (pszOut + 5 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'm';
			*pszOut++ = 'p';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\"')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'q';
			*pszOut++ = 'u';
			*pszOut++ = 'o';
			*pszOut++ = 't';
			*pszOut++ = ';';
		}
		else if (*pszIn == '\'')
		{
			if (pszOut + 6 > pszEnd)
			{
				break;
			}

			pszIn++;
			*pszOut++ = '&';
			*pszOut++ = 'a';
			*pszOut++ = 'p';
			*pszOut++ = 'o';
			*pszOut++ = 's';
			*pszOut++ = ';';
		}
		else if (*pszIn  < ' ' || *pszIn > 127)
		{
			WCHAR c = *pszIn++;
			if (c < 10 && pszOut + 4 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else if (c < 100 && pszOut + 5 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)((c / 10) % 10);
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else if (c < 1000 && pszOut + 6 <= pszEnd)
			{
				*pszOut++ = '&';
				*pszOut++ = '#';
				*pszOut++ = '0' + (CHAR)((c / 100) % 10);
				*pszOut++ = '0' + (CHAR)((c / 10) % 10);
				*pszOut++ = '0' + (CHAR)(c % 10);
				*pszOut++ = ';';
			}
			else
			{
				break;
			}
		}
		else
		{
			*pszOut++ = (CHAR)*pszIn++;
		}
	}

	*pszOut = '\0';
	return pszOut;
}

#if _MSC_VER >= 1900
#pragma warning(push)
#pragma warning(disable:4456) // declaration hides previous local declaration
#endif

void VSafePrintf(PCSTR pszMsg, va_list args, PCHAR pszBuffer, LONG cbBuffer)
{
	PCHAR pszOut = pszBuffer;
	PCHAR pszEnd = pszBuffer + cbBuffer - 1;
	pszBuffer[0] = '\0';

	__try
	{
		while (*pszMsg && pszOut < pszEnd)
		{
			if (*pszMsg == '%')
			{
				CHAR szHead[4] = "";
				INT nLen;
				INT nWidth = 0;
				INT nPrecision = 0;
				bool fLeft = false;
				bool fPositive = false;
				bool fPound = false;
				bool fBlank = false;
				bool fZero = false;
				bool fDigit = false;
				bool fSmall = false;
				bool fLarge = false;
				bool f64Bit = false;
				PCSTR pszArg = pszMsg;

				pszMsg++;

				for (; (*pszMsg == '-' ||
						*pszMsg == '+' ||
						*pszMsg == '#' ||
						*pszMsg == ' ' ||
						*pszMsg == '0'); pszMsg++)
				{
					switch (*pszMsg)
					{
						case '-': fLeft = true; break;
						case '+': fPositive = true; break;
						case '#': fPound = true; break;
						case ' ': fBlank = true; break;
						case '0': fZero = true; break;
					}
				}

				if (*pszMsg == '*')
				{
					nWidth = va_arg(args, INT);
					pszMsg++;
				}
				else
				{
					while (*pszMsg >= '0' && *pszMsg <= '9')
					{
						nWidth = nWidth * 10 + (*pszMsg++ - '0');
					}
				}
				if (*pszMsg == '.')
				{
					pszMsg++;
					fDigit = true;
					if (*pszMsg == '*')
					{
						nPrecision = va_arg(args, INT);
						pszMsg++;
					}
					else
					{
						while (*pszMsg >= '0' && *pszMsg <= '9')
						{
							nPrecision = nPrecision * 10 + (*pszMsg++ - '0');
						}
					}
				}

				if (*pszMsg == 'h')
				{
					fSmall = true;
					pszMsg++;
				}
				else if (*pszMsg == 'l')
				{
					fLarge = true;
					pszMsg++;
				}
				else if (*pszMsg == 'I' && pszMsg[1] == '6' && pszMsg[2] == '4')
				{
					f64Bit = true;
					pszMsg += 3;
				}

				if (*pszMsg == 's' || *pszMsg == 'e' || *pszMsg == 'c')
				{
					// We ignore the length, precision, and alignment
					// to avoid using a temporary buffer.

					if (*pszMsg == 's')
					{
						// [GalenH] need to not use temp.
						PVOID pvData = va_arg(args, PVOID);

						pszMsg++;

						if (fSmall)
						{
							fLarge = false;
						}

						__try
						{
							if (pvData == nullptr)
							{
								pszOut = do_str(pszOut, pszEnd, "-nullptr-");
							}
							else if (fLarge)
							{
								pszOut = do_wstr(pszOut, pszEnd, (PWCHAR)pvData);
							}
							else
							{
								pszOut = do_str(pszOut, pszEnd, (PCHAR)pvData);
							}
						}
						__except(EXCEPTION_EXECUTE_HANDLER)
						{
							pszOut = do_str(pszOut, pszEnd, "-");
							pszOut = do_base(
								pszOut,
								(UINT64)pvData,
								16,
								"0123456789ABCDEF");
							pszOut = do_str(pszOut, pszEnd, "-");
						}
					}
					else if (*pszMsg == 'e')
					{
						// Escape the string.
						PVOID pvData = va_arg(args, PVOID);

						pszMsg++;

						if (fSmall)
						{
							fLarge = false;
						}

						__try
						{
							if (pvData == nullptr)
							{
								pszOut = do_str(pszOut, pszEnd, "-nullptr-");
							}
							else if (fLarge)
							{
								pszOut = do_ewstr(pszOut, pszEnd, (PWCHAR)pvData);
							}
							else
							{
								pszOut = do_estr(pszOut, pszEnd, (PCHAR)pvData);
							}
						}
						__except(EXCEPTION_EXECUTE_HANDLER)
						{
							pszOut = do_str(pszOut, pszEnd, "-");
							pszOut = do_base(
								pszOut,
								(UINT64)pvData,
								16,
								"0123456789ABCDEF");
							pszOut = do_str(pszOut, pszEnd, "-");
						}
					}
					else
					{
						CHAR szTemp[2];
						pszMsg++;

						szTemp[0] = (CHAR)va_arg(args, INT);
						szTemp[1] = '\0';
						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
				}
				else if (*pszMsg == 'd' || *pszMsg == 'i' || *pszMsg == 'o' ||
							*pszMsg == 'x' || *pszMsg == 'X' || *pszMsg == 'b' ||
							*pszMsg == 'u')
				{
					CHAR szTemp[128];
					UINT64 value;
					if (f64Bit)
					{
						value = va_arg(args, UINT64);
					}
					else
					{
						value = va_arg(args, UINT);
					}

					if (*pszMsg == 'x')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 16, "0123456789abcdef") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}
					else if (*pszMsg == 'X')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 16, "0123456789ABCDEF") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0X");
						}
					}
					else if (*pszMsg == 'd')
					{
						pszMsg++;
						if ((INT64)value < 0)
						{
							value = -(INT64)value;
							do_str(szHead, szHead + sizeof(szHead) - 1, "-");
						}
						else if (fPositive)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, "+");
							}
						}
						else if (fBlank)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, " ");
							}
						}
						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}
					else if (*pszMsg == 'u')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}
					else if (*pszMsg == 'o')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 8, "01234567") - szTemp);
						nPrecision = 0;

						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0");
						}
					}
					else if (*pszMsg == 'b')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, value, 2, "01") - szTemp);
						nPrecision = 0;

						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0b");
						}
					}
					else
					{
						pszMsg++;
						if ((INT64)value < 0)
						{
							value = -(INT64)value;
							do_str(szHead, szHead + sizeof(szHead) - 1, "-");
						}
						else if (fPositive)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, "+");
							}
						}
						else if (fBlank)
						{
							if (value > 0)
							{
								do_str(szHead, szHead + sizeof(szHead) - 1, " ");
							}
						}

						nLen = (int)(do_base(szTemp, value, 10, "0123456789") - szTemp);
						nPrecision = 0;
					}

					INT nHead = 0;
					for (; szHead[nHead]; nHead++)
					{
						// Count characters in head string.
					}

					if (fLeft)
					{
						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
							nLen += nHead;
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = ' ';
						}
					}
					else if (fZero)
					{
						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
							nLen += nHead;
						}

						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = '0';
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
					else
					{
						if (nHead)
						{
							nLen += nHead;
						}

						for (; nLen < nWidth && pszOut < pszEnd; nLen++)
						{
							*pszOut++ = ' ';
						}

						if (nHead)
						{
							pszOut = do_str(pszOut, pszEnd, szHead);
						}

						pszOut = do_str(pszOut, pszEnd, szTemp);
					}
				}
				else if (*pszMsg == 'p')
				{
					CHAR szTemp[64];
					ULONG_PTR value;
					value = va_arg(args, ULONG_PTR);

					if (*pszMsg == 'p')
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, (UINT64)value, 16, "0123456789abcdef") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}
					else
					{
						pszMsg++;
						nLen = (int)(do_base(szTemp, (UINT64)value, 16, "0123456789ABCDEF") - szTemp);
						if (fPound && value)
						{
							do_str(szHead, szHead + sizeof(szHead) - 1, "0x");
						}
					}

					INT nHead = 0;
					for (; szHead[nHead]; nHead++)
					{
						// Count characters in head string.
					}

					if (nHead)
					{
						pszOut = do_str(pszOut, pszEnd, szHead);
						nLen += nHead;
					}

					for (; nLen < nWidth && pszOut < pszEnd; nLen++)
					{
						*pszOut++ = '0';
					}

					pszOut = do_str(pszOut, pszEnd, szTemp);
				}
				else
				{
					pszMsg++;
					while (pszArg < pszMsg && pszOut < pszEnd)
					{
						*pszOut++ = *pszArg++;
					}
				}
			}
			else
			{
				if (pszOut < pszEnd)
				{
					*pszOut++ = *pszMsg++;
				}
			}
		}

		*pszOut = '\0';
		pszBuffer[cbBuffer - 1] = '\0';
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		PCHAR pszOut = pszBuffer;
		*pszOut = '\0';
		pszOut = do_str(pszOut, pszEnd, "-exception:");
		pszOut = do_base(pszOut, (UINT64)GetExceptionCode(), 10, "0123456789");
		pszOut = do_str(pszOut, pszEnd, "-");
	}
}

#if _MSC_VER >= 1900
#pragma warning(pop)
#endif

PCHAR SafePrintf(PCHAR pszBuffer, LONG cbBuffer, PCSTR pszMsg, ...)
{
	va_list args;
	va_start(args, pszMsg);
	VSafePrintf(pszMsg, args, pszBuffer, cbBuffer);
	va_end(args);

	while (*pszBuffer)
	{
		pszBuffer++;
	}

	return pszBuffer;
}

//////////////////////////////////////////////////////////////////////////////
//
bool TblogOpen()
{
	EnterCriticalSection(&s_csPipe);

	WCHAR wzPipe[256];
	StringCchPrintfW(wzPipe, ARRAYSIZE(wzPipe), L"%ls.%d", TBLOG_PIPE_NAMEW, s_nTraceProcessId);

	for (int retries = 0; retries < 10; retries++)
	{
		WaitNamedPipeW(wzPipe, 10000); // Wait up to 10 seconds for a pipe to appear.

		s_hPipe = Functions::Original::CreateFileW(wzPipe, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (s_hPipe != INVALID_HANDLE_VALUE)
		{
			DWORD dwMode = PIPE_READMODE_MESSAGE;
			if (SetNamedPipeHandleState(s_hPipe, &dwMode, nullptr, nullptr))
			{
				LeaveCriticalSection(&s_csPipe);
				return true;
			}
		}
	}

	LeaveCriticalSection(&s_csPipe);

	// Couldn't open pipe.
	DEBUG_BREAK();
	Functions::Original::ExitProcess(9990);
	return false;
}

void TblogV(PCSTR pszMsgf, va_list args)
{
	if (s_hPipe == INVALID_HANDLE_VALUE) {
		return;
	}

	EnterCriticalSection(&s_csPipe);

	DWORD cbWritten = 0;

	PCHAR pszBuf = s_rMessage.szMessage;
	VSafePrintf(
		pszMsgf,
		args,
		pszBuf,
		(int)(s_rMessage.szMessage + sizeof(s_rMessage.szMessage) - pszBuf));

	PCHAR pszEnd = s_rMessage.szMessage;
	for (; *pszEnd; pszEnd++)
	{
		// no internal contents.
	}

	s_rMessage.nBytes = (DWORD)(pszEnd - ((PCSTR)&s_rMessage));

	// If the write fails, then we abort
	if (s_hPipe != INVALID_HANDLE_VALUE)
	{
		if (!Functions::Original::WriteFile(s_hPipe, &s_rMessage, s_rMessage.nBytes, &cbWritten, nullptr))
		{
			Functions::Original::ExitProcess(9991);
		}
	}

	LeaveCriticalSection(&s_csPipe);
}

void Tblog(PCSTR pszMsgf, ...)
{
	if (s_hPipe == INVALID_HANDLE_VALUE)
	{
		return;
	}

	va_list args;
	va_start(args, pszMsgf);
	TblogV(pszMsgf, args);
	va_end(args);
}

void TblogClose()
{
	EnterCriticalSection(&s_csPipe);

	if (s_hPipe != INVALID_HANDLE_VALUE)
	{
		DWORD cbWritten = 0;

		s_rMessage.nBytes = 0;

		Functions::Original::WriteFile(s_hPipe, &s_rMessage, 4, &cbWritten, nullptr);
		FlushFileBuffers(s_hPipe);
		Functions::Original::CloseHandle(s_hPipe);
		s_hPipe = INVALID_HANDLE_VALUE;
	}

	LeaveCriticalSection(&s_csPipe);
}

/////////////////////////////////////////////////////////////
// Detours
//
static bool IsInherited(HANDLE hHandle)
{
	DWORD dwFlags;

	if (GetHandleInformation(hHandle, &dwFlags))
	{
		return (dwFlags & HANDLE_FLAG_INHERIT) ? true : false;
	}

	return false;
}

static void SaveStdHandleName(HANDLE hFile, PWCHAR pwzBuffer, BOOL* fAppend)
{
	pwzBuffer[0] = '\0';

	if ((hFile != INVALID_HANDLE_VALUE) && IsInherited(hFile))
	{
		FileInfo * pInfo = OpenFiles::RecallFile(hFile);
		if (pInfo)
		{
			Copy(pwzBuffer, pInfo->m_pwzPath);
			if (pInfo->m_fAppend && fAppend != nullptr)
			{
				*fAppend = true;
			}
		}
	}
}

static void LoadStdHandleName(DWORD id, PCWSTR pwzBuffer, bool fAppend)
{
	HANDLE hFile = GetStdHandle(id);

	if ((hFile != INVALID_HANDLE_VALUE) && pwzBuffer[0] != '\0')
	{
		FileInfo *pInfo = FileNames::FindPartial(pwzBuffer);
		if (fAppend)
		{
			pInfo->m_fAppend = true;
		}

		OpenFiles::Remember(hFile, pInfo);
	}
}

bool CreateProcessInternals(
	HANDLE hProcess,
	DWORD nProcessId,
	PCHAR pszId,
	HANDLE hStdin,
	HANDLE hStdout,
	HANDLE hStderr)
{
	EnterCriticalSection(&s_csChildPayload);

	ProcInfo *proc = Procs::Create(hProcess, nProcessId);
	OpenFiles::Remember(hProcess, proc);

	ZeroMemory(&s_ChildPayload, sizeof(s_ChildPayload));
	CopyMemory(&s_ChildPayload, &s_Payload, sizeof(s_ChildPayload));

	s_ChildPayload.nParentProcessId = GetCurrentProcessId();
	s_ChildPayload.rGeneology[s_ChildPayload.nGeneology]
		= (DWORD)InterlockedIncrement(&s_nChildCnt);
	s_ChildPayload.nGeneology++;

	SaveStdHandleName(hStdin, s_ChildPayload.wzStdin, nullptr);
	SaveStdHandleName(hStdout, s_ChildPayload.wzStdout, &s_ChildPayload.fStdoutAppend);
	SaveStdHandleName(hStderr, s_ChildPayload.wzStderr, &s_ChildPayload.fStderrAppend);

	DetourCopyPayloadToProcess(hProcess, s_guidTrace, &s_ChildPayload, sizeof(s_ChildPayload));

	for (DWORD i = 0; i < s_ChildPayload.nGeneology; i++)
	{
		pszId = SafePrintf(pszId, 16, "%d.", s_ChildPayload.rGeneology[i]);
	}

	*pszId = '\0';

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}

BOOL WINAPI Mine_CreateProcessW(
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
	EnterFunc();

	if (lpCommandLine == nullptr)
	{
		lpCommandLine = (LPWSTR)lpApplicationName;
	}

	CHAR szProc[MAX_PATH];
	bool rv = 0;
	__try
	{
		LPPROCESS_INFORMATION ppi = lpProcessInformation;
		PROCESS_INFORMATION pi;
		if (ppi == nullptr)
		{
			ppi = &pi;
		}

		rv = DetourCreateProcessWithDllExW(
			lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags | CREATE_SUSPENDED,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			ppi,
			s_szDllPath,
			Functions::Original::CreateProcessW);

		if (rv)
		{
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);

			if (lpStartupInfo != nullptr && (lpStartupInfo->dwFlags & STARTF_USESTDHANDLES) != 0)
			{
				hStdin = lpStartupInfo->hStdInput;
				hStdout = lpStartupInfo->hStdOutput;
				hStderr = lpStartupInfo->hStdError;
			}

			CreateProcessInternals(
				ppi->hProcess,
				ppi->dwProcessId,
				szProc,
				hStdin,
				hStdout,
				hStderr);

			Print("<t:Child id=\"::%hs::\">\n", szProc);

			WCHAR wzPath[MAX_PATH];
			FileInfo *pInfo = nullptr;
			if (lpApplicationName == nullptr)
			{
				PWCHAR pwzDst = wzPath;
				PWCHAR pwzSrc = lpCommandLine;

				if (*pwzSrc == '\"')
				{
					WCHAR cQuote = *pwzSrc++;

					while (*pwzSrc && *pwzSrc != cQuote)
					{
						*pwzDst++ = *pwzSrc++;
					}

					*pwzDst++ = '\0';
				}
				else
				{
					while (*pwzSrc && *pwzSrc != ' ' && *pwzSrc != '\t')
					{
						if (*pwzSrc == '\t')
						{
							*pwzSrc = ' ';
						}

						*pwzDst++ = *pwzSrc++;
					}

					*pwzDst++ = '\0';
				}

				pInfo = FileNames::FindPartial(wzPath);
			}
			else
			{
				pInfo = FileNames::FindPartial(lpApplicationName);
			}

			Print(
				"<t:Executable>%ls</t:Executable>\n",
				FileNames::ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo));
			Print("<t:Line>%le</t:Line>\n", lpCommandLine);
			Print("</t:Child>\n");

			if (pInfo)
			{
				pInfo->m_fAbsorbed = true;
			}

			if (!(dwCreationFlags & CREATE_SUSPENDED))
			{
				ResumeThread(ppi->hThread);
			}

			if (ppi == &pi)
			{
				Functions::Original::CloseHandle(ppi->hThread);
				Functions::Original::CloseHandle(ppi->hProcess);
			}
		}
	}
	__finally
	{
		ExitFunc();
		if (!rv)
		{
			Print(
				"<!-- Warning: CreateProcessW failed %d: %ls; %ls -->\n",
				GetLastError(),
				lpApplicationName,
				lpCommandLine);
		}
	}

	return rv;
}

BOOL WINAPI Mine_CreateProcessA(
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
	EnterFunc();

	if (lpCommandLine == nullptr)
	{
		lpCommandLine = (LPSTR)lpApplicationName;
	}

	CHAR szProc[MAX_PATH];
	bool rv = 0;
	__try
	{
		LPPROCESS_INFORMATION ppi = lpProcessInformation;
		PROCESS_INFORMATION pi;
		if (ppi == nullptr)
		{
			ppi = &pi;
		}

		rv = DetourCreateProcessWithDllExA(
			lpApplicationName,
			lpCommandLine,
			lpProcessAttributes,
			lpThreadAttributes,
			bInheritHandles,
			dwCreationFlags | CREATE_SUSPENDED,
			lpEnvironment,
			lpCurrentDirectory,
			lpStartupInfo,
			ppi,
			s_szDllPath,
			Functions::Original::CreateProcessA);

		if (rv)
		{
			HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
			HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			HANDLE hStderr = GetStdHandle(STD_ERROR_HANDLE);

			if (lpStartupInfo != nullptr && (lpStartupInfo->dwFlags & STARTF_USESTDHANDLES) != 0)
			{
				hStdin = lpStartupInfo->hStdInput;
				hStdout = lpStartupInfo->hStdOutput;
				hStderr = lpStartupInfo->hStdError;
			}

			CreateProcessInternals(
				ppi->hProcess,
				ppi->dwProcessId,
				szProc,
				hStdin,
				hStdout,
				hStderr);

			Print("<t:Child id=\"::%hs::\">\n", szProc);

			WCHAR wzPath[MAX_PATH];
			FileInfo *pInfo = nullptr;
			if (lpApplicationName == nullptr)
			{
				PCHAR pszDst = szProc;
				PCHAR pszSrc = lpCommandLine;

				if (*pszSrc == '\"')
				{
					CHAR cQuote = *pszSrc++;

					while (*pszSrc && *pszSrc != cQuote)
					{
						*pszDst++ = *pszSrc++;
					}

					*pszDst++ = '\0';
				}
				else
				{
					while (*pszSrc && *pszSrc != ' ' && *pszSrc != '\t')
					{
						if (*pszSrc == '\t')
						{
							*pszSrc = ' ';
						}

						*pszDst++ = *pszSrc++;
					}

					*pszDst++ = '\0';
				}

				pInfo = FileNames::FindPartial(szProc);
			}
			else
			{
				pInfo = FileNames::FindPartial(lpApplicationName);
			}

			Print(
				"<t:Executable>%ls</t:Executable>\n",
				FileNames::ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo));
			Print("<t:Line>%he</t:Line>\n", lpCommandLine);
			Print("</t:Child>\n");

			if (pInfo)
			{
				pInfo->m_fAbsorbed = true;
			}

			if (!(dwCreationFlags & CREATE_SUSPENDED))
			{
				ResumeThread(ppi->hThread);
			}

			if (ppi == &pi)
			{
				Functions::Original::CloseHandle(ppi->hThread);
				Functions::Original::CloseHandle(ppi->hProcess);
			}
		}
	}
	__finally
	{
		ExitFunc();
		if (!rv)
		{
			Print(
				"<!-- Warning: CreateProcessA failed %d: %hs; %hs -->\n",
				GetLastError(),
				lpApplicationName,
				lpCommandLine);
		}
	}

	return rv;
}

//
//////////////////////////////////////////////////////////////////////////////

BOOL WINAPI Mine_CopyFileExA(
	LPCSTR a0,
	LPCSTR a1,
	LPPROGRESS_ROUTINE a2,
	LPVOID a3,
	LPBOOL a4,
	DWORD a5)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::CopyFileExA(a0, a1, a2, a3, a4, a5);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
#if 0
			Print("<!-- CopyFileExA %he to %he -->\n", a0, a1);
#endif
			NoteRead(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_CopyFileExW(
	LPCWSTR a0,
	LPCWSTR a1,
	LPPROGRESS_ROUTINE a2,
	LPVOID a3,
	LPBOOL a4,
	DWORD a5)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
#if 0
		Print("\n");
		Print("<!-- CopyFileExW %le to %le before -->\n", a0, a1);
#endif
		rv = Functions::Original::CopyFileExW(a0, a1, a2, a3, a4, a5);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
#if 0
			Print("<!-- CopyFileExW %le to %le -->\n", a0, a1);
#endif
			NoteRead(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_PrivCopyFileExW(
	LPCWSTR a0,
	LPCWSTR a1,
	LPPROGRESS_ROUTINE a2,
	LPVOID a3,
	LPBOOL a4,
	DWORD a5)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::PrivCopyFileExW(a0, a1, a2, a3, a4, a5);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
#if 0
			Print("<!-- PrivCopyFileExW %le to %le -->\n", a0, a1);
#endif
			NoteRead(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_CreateHardLinkA(
	LPCSTR a0,
	LPCSTR a1,
	LPSECURITY_ATTRIBUTES a2)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::CreateHardLinkA(a0, a1, a2);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
	#if 0
			Print("<!-- CreateHardLinkA %he to %he -->\n", a0, a1);
	#endif
			NoteRead(a1);
			NoteWrite(a0);
		}
	};

	return rv;
}

BOOL WINAPI Mine_CreateHardLinkW(
	LPCWSTR a0,
	LPCWSTR a1,
	LPSECURITY_ATTRIBUTES a2)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::CreateHardLinkW(a0, a1, a2);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
#if 0
			Print("<!-- CreateHardLinkW %le to %le -->\n", a0, a1);
#endif
			NoteRead(a1);
			NoteWrite(a0);
		}
	};

	return rv;
}

BOOL WINAPI Mine_CloseHandle(HANDLE a0)
{
	/*int nIndent =*/ EnterFunc();

	bool rv = 0;
	__try
	{
		ProcInfo * pProc = OpenFiles::RecallProc(a0);
		if (pProc != nullptr)
		{
			Procs::Close(pProc->m_hProc);
		}

		FileInfo * pFile = OpenFiles::RecallFile(a0);
		if (pFile != nullptr)
		{
			DWORD dwErr = GetLastError();
			pFile->m_cbContent = GetFileSize(a0, nullptr);
			if (pFile->m_cbContent == INVALID_FILE_SIZE)
			{
				pFile->m_cbContent = 0;
			}

			if (pFile->m_fCantRead)
			{
				if (pFile->m_fRead)
				{
	#if 0
					Print("<!-- Warning: Removing read from %le -->\n", pFile->m_pwzPath);
	#endif
					pFile->m_fRead = false;
				}
			}

			// Here we should think about reading the file contents as appropriate.
			if (pFile->m_fTemporaryPath && pFile->m_fRead && !pFile->m_fAbsorbed &&
				!pFile->m_fDelete && !pFile->m_fCleanup && !pFile->m_fWrite &&
				pFile->m_pbContent == nullptr &&
				pFile->m_cbContent < 16384)
			{
				pFile->m_pbContent = LoadFile(a0, pFile->m_cbContent);
			}

			SetLastError(dwErr);
		}

		rv = Functions::Original::CloseHandle(a0);
	}
	__finally
	{
		ExitFunc();
		if (rv /* && nIndent == 0*/)
		{
			OpenFiles::Forget(a0);
		}
	};

	return rv;
}

BOOL WINAPI Mine_DuplicateHandle(
	HANDLE hSourceProcessHandle,
	HANDLE hSourceHandle,
	HANDLE hTargetProcessHandle,
	LPHANDLE lpTargetHandle,
	DWORD dwDesiredAccess,
	bool bInheritHandle,
	DWORD dwOptions)
{
	HANDLE hTemp = INVALID_HANDLE_VALUE;
	EnterFunc();

	bool rv = 0;
	__try
	{
		if (lpTargetHandle == nullptr)
		{
			lpTargetHandle = &hTemp;
		}

		*lpTargetHandle = INVALID_HANDLE_VALUE;

		rv = Functions::Original::DuplicateHandle(
			hSourceProcessHandle,
			hSourceHandle,
			hTargetProcessHandle,
			lpTargetHandle,
			dwDesiredAccess,
			bInheritHandle,
			dwOptions);
	}
	__finally
	{
		ExitFunc();
		if (*lpTargetHandle != INVALID_HANDLE_VALUE)
		{
			FileInfo *pInfo = OpenFiles::RecallFile(hSourceHandle);
			if (pInfo)
			{
				OpenFiles::Remember(*lpTargetHandle, pInfo);
			}
		}
	};

	return rv;
}

static LONG s_nPipeCnt = 0;

BOOL WINAPI Mine_CreatePipe(
	PHANDLE hReadPipe,
	PHANDLE hWritePipe,
	LPSECURITY_ATTRIBUTES lpPipeAttributes,
	DWORD nSize)
{
	HANDLE hRead = INVALID_HANDLE_VALUE;
	HANDLE hWrite = INVALID_HANDLE_VALUE;

	if (hReadPipe == nullptr)
	{
		hReadPipe = &hRead;
	}

	if (hWritePipe == nullptr)
	{
		hWritePipe = &hWrite;
	}

	/*int nIndent = */ EnterFunc();
	bool rv = 0;
	__try
	{
		rv = Functions::Original::CreatePipe(hReadPipe, hWritePipe, lpPipeAttributes, nSize);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			CHAR szPipe[128];

			SafePrintf(
				szPipe,
				ARRAYSIZE(szPipe),
				"\\\\.\\PIPE\\Temp.%d.%d",
				GetCurrentProcessId(),
				InterlockedIncrement(&s_nPipeCnt));

			FileInfo *pInfo = FileNames::FindPartial(szPipe);

			pInfo->m_fCleanup = true;
			OpenFiles::Remember(*hReadPipe, pInfo);
			OpenFiles::Remember(*hWritePipe, pInfo);
		}
	};

	return rv;
}

BOOL WINAPI Mine_CreateDirectoryW(
	LPCWSTR a0,
	LPSECURITY_ATTRIBUTES a1)
{
	/* int nIndent = */ EnterFunc();
	bool rv = 0;
	__try
	{
		rv = Functions::Original::CreateDirectoryW(a0, a1);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			FileInfo *pInfo = FileNames::FindPartial(a0);
			pInfo->m_fDirectory = true;
		}
	};

	return rv;
}

BOOL WINAPI Mine_CreateDirectoryExW(
	LPCWSTR a0,
	LPCWSTR a1,
	LPSECURITY_ATTRIBUTES a2)
{
	/* int nIndent = */ EnterFunc();
	bool rv = 0;
	__try
	{
		rv = Functions::Original::CreateDirectoryExW(a0, a1, a2);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			FileInfo *pInfo = FileNames::FindPartial(a1);
			pInfo->m_fDirectory = true;
		}
	};

	return rv;
}

HANDLE WINAPI Mine_CreateFileW(
	LPCWSTR a0,
	DWORD access,
	DWORD share,
	LPSECURITY_ATTRIBUTES a3,
	DWORD create,
	DWORD flags,
	HANDLE a6)
{
	/* int nIndent = */ EnterFunc();
	HANDLE rv = 0;
	__try
	{
		rv = Functions::Original::CreateFileW(a0, access, share, a3, create, flags, a6);
	}
	__finally
	{
		ExitFunc();
#if 0
			Print("<!-- CreateFileW(%le, ac=%08x, cr=%08x, fl=%08x -->\n",
					a0,
					access,
					create,
					flags);
#endif
		if (access != 0 && /* nIndent == 0 && */ rv != INVALID_HANDLE_VALUE)
		{

			FileInfo *pInfo = FileNames::FindPartial(a0);

			// FILE_FLAG_WRITE_THROUGH              0x80000000
			// FILE_FLAG_OVERLAPPED                 0x40000000
			// FILE_FLAG_NO_BUFFERING               0x20000000
			// FILE_FLAG_RANDOM_ACCESS              0x10000000
			// FILE_FLAG_SEQUENTIAL_SCAN            0x08000000
			// FILE_FLAG_DELETE_ON_CLOSE            0x04000000
			// FILE_FLAG_BACKUP_SEMANTICS           0x02000000
			// FILE_FLAG_POSIX_SEMANTICS            0x01000000
			// FILE_FLAG_OPEN_REPARSE_POINT         0x00200000
			// FILE_FLAG_OPEN_NO_RECALL             0x00100000
			// FILE_FLAG_FIRST_PIPE_INSTANCE        0x00080000
			// FILE_ATTRIBUTE_ENCRYPTED             0x00004000
			// FILE_ATTRIBUTE_NOT_CONTENT_INDEXED   0x00002000
			// FILE_ATTRIBUTE_OFFLINE               0x00001000
			// FILE_ATTRIBUTE_COMPRESSED            0x00000800
			// FILE_ATTRIBUTE_REPARSE_POINT         0x00000400
			// FILE_ATTRIBUTE_SPARSE_FILE           0x00000200
			// FILE_ATTRIBUTE_TEMPORARY             0x00000100
			// FILE_ATTRIBUTE_NORMAL                0x00000080
			// FILE_ATTRIBUTE_DEVICE                0x00000040
			// FILE_ATTRIBUTE_ARCHIVE               0x00000020
			// FILE_ATTRIBUTE_DIRECTORY             0x00000010
			// FILE_ATTRIBUTE_SYSTEM                0x00000004
			// FILE_ATTRIBUTE_HIDDEN                0x00000002
			// FILE_ATTRIBUTE_READONLY              0x00000001

			// CREATE_NEW          1
			// CREATE_ALWAYS       2
			// OPEN_EXISTING       3
			// OPEN_ALWAYS         4
			// TRUNCATE_EXISTING   5

			if (create == CREATE_NEW ||
				create == CREATE_ALWAYS ||
				create == TRUNCATE_EXISTING)
			{
				if (!pInfo->m_fRead)
				{
					pInfo->m_fCantRead = true;
				}
			}
			else if (create == OPEN_EXISTING)
			{
			}
			else if (create == OPEN_ALWAYS)
			{
				// pInfo->m_fAppend = true;    // !!!
			}

			if ((flags & FILE_FLAG_DELETE_ON_CLOSE))
			{
				pInfo->m_fCleanup = true;
			}

			OpenFiles::Remember(rv, pInfo);
		}
	};

	return rv;
}

HANDLE WINAPI Mine_CreateFileMappingW(
	HANDLE hFile,
	LPSECURITY_ATTRIBUTES a1,
	DWORD flProtect,
	DWORD a3,
	DWORD a4,
	LPCWSTR a5)
{
	/* int nIndent = */ EnterFunc();
	HANDLE rv = 0;
	__try
	{
		rv = Functions::Original::CreateFileMappingW(hFile, a1, flProtect, a3, a4, a5);
	}
	__finally
	{
		ExitFunc();
		if (rv != INVALID_HANDLE_VALUE)
		{
			FileInfo *pInfo = OpenFiles::RecallFile(hFile);

			if (pInfo != nullptr)
			{
				switch (flProtect)
				{
					case PAGE_READONLY:
						pInfo->m_fRead = true;
						break;
					case PAGE_READWRITE:
						pInfo->m_fRead = true;
						pInfo->m_fWrite = true;
						break;
					case PAGE_WRITECOPY:
						pInfo->m_fRead = true;
						break;
					case PAGE_EXECUTE_READ:
						pInfo->m_fRead = true;
						break;
					case PAGE_EXECUTE_READWRITE:
						pInfo->m_fRead = true;
						pInfo->m_fWrite = true;
						break;
				}
			}
		}
	};

	return rv;
}

BOOL WINAPI Mine_DeleteFileW(LPCWSTR a0)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::DeleteFileW(a0);
	}
	__finally
	{
		ExitFunc();
#if 0
		Print("<!-- DeleteFileW(%le -->\n", a0);
#endif
		NoteDelete(a0);
	};

	return rv;
}

static void Dump(LPVOID pvData, DWORD cbData)
{
	CHAR szBuffer[128];
	PBYTE pbData = (PBYTE)pvData;

	for (DWORD i = 0; i < cbData; i += 16)
	{
		PCHAR psz = szBuffer;
		psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), "%4d: ", i);

		for (DWORD j = i; j < i + 16; j++)
		{
			if (j < cbData)
			{
				psz = SafePrintf(
					psz,
					(LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz),
					"%02x",
					pbData[j]);
			}
			else
			{
				psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), "  ");
			}
		}

		for (DWORD j = i; j < i + 16; j++)
		{
			if (j < cbData)
			{
				if (pbData[j] >= ' ' && pbData[j] <= 127)
				{
					psz = SafePrintf(
						psz,
						(LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz),
						"%c",
						pbData[j]);
				}
				else
				{
					psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), ".");
				}
			}
			else
			{
				psz = SafePrintf(psz, (LONG)(szBuffer + ARRAYSIZE(szBuffer) - psz), " ");
			}
		}

		Print("%s\n", szBuffer);
	}
}

BOOL WINAPI Mine_DeviceIoControl(
	HANDLE a0,
	DWORD a1,
	LPVOID a2,
	DWORD a3,
	LPVOID a4,
	DWORD a5,
	LPDWORD a6,
	LPOVERLAPPED a7)
{
	EnterFunc();
	DWORD d6 = 0;
	if (a6 == nullptr)
	{
		a6 = &d6;
	}

	bool rv = 0;
	__try
	{
		rv = Functions::Original::DeviceIoControl(a0, a1, a2, a3, a4, a5, a6, a7);
	}
	__finally
	{
		ExitFunc();
		OpenFiles::SetRead(a0, 0);
		OpenFiles::SetWrite(a0, 0);
		if (rv && a1 != 0x390008 && a1 != 0x4d0008 && a1 != 0x6d0008)
		{
			FileInfo *pInfo = OpenFiles::RecallFile(a0);

			DWORD DeviceType = (a1 & 0xffff0000) >> 16;
			DWORD Access = (a1 & 0x0000c000) >> 14;
			DWORD Function = (a1 & 0x00003ffc) >> 2;
			DWORD Method = (a1 & 0x00000003) >> 0;

			if (pInfo)
			{
				Print(
					"<!-- DeviceIoControl %x [dev=%x,acc=%x,fun=%x,mth=%x] on %ls! -->\n",
					a1,
					DeviceType,
					Access,
					Function,
					Method,
					pInfo->m_pwzPath);
			}
			else
			{
				Print(
					"<!-- DeviceIoControl %x [dev=%x,acc=%x,fun=%x,mth=%x,in=%d,out=%d/%d] on (%x)! -->\n",
					a1,
					DeviceType,
					Access,
					Function,
					Method,
					a3,
					*a6,
					a5,
					a0);

				if (a3 > 0)
				{
					Dump(a2, a3);
				}

				if (a5 > 0)
				{
					Dump(a4, (*a6 < a5) ? *a6 : a5);
				}
			}
		}
	};

	return rv;
}

DWORD WINAPI Mine_GetFileAttributesW(LPCWSTR a0)
{
	EnterFunc();

	DWORD rv = 0;
	__try
	{
		rv = Functions::Original::GetFileAttributesW(a0);
	}
	__finally
	{
		ExitFunc();
	};

	return rv;
}

BOOL WINAPI Mine_MoveFileWithProgressW(
	LPCWSTR a0,
	LPCWSTR a1,
	LPPROGRESS_ROUTINE a2,
	LPVOID a3,
	DWORD a4)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::MoveFileWithProgressW(a0, a1, a2, a3, a4);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			NoteRead(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_MoveFileA(
	LPCSTR a0,
	LPCSTR a1)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::MoveFileA(a0, a1);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			NoteRead(a0);
			NoteCleanup(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_MoveFileW(
	LPCWSTR a0,
	LPCWSTR a1)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::MoveFileW(a0, a1);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			NoteRead(a0);
			NoteCleanup(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_MoveFileExA(
	LPCSTR a0,
	LPCSTR a1,
	DWORD a2)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::MoveFileExA(a0, a1, a2);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			NoteRead(a0);
			NoteCleanup(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

BOOL WINAPI Mine_MoveFileExW(
	LPCWSTR a0,
	LPCWSTR a1,
	DWORD a2)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::MoveFileExW(a0, a1, a2);
	}
	__finally
	{
		ExitFunc();
		if (rv)
		{
			NoteRead(a0);
			NoteCleanup(a0);
			NoteWrite(a1);
		}
	};

	return rv;
}

void SetHandle(PCSTR pszName, HANDLE h)
{
#if 0
	FileInfo *pInfo = OpenFiles::RecallFile(h);

	if (pInfo != nullptr)
	{
		Tblog("<!-- hset: %hs (%x) %ls -->\n", pszName, h, pInfo->m_pwzPath);
	}
	else
	{
		Tblog("<!-- hset: %hs (%x) ***Unknown*** -->\n", pszName, h);
	}
#else
	(void)pszName;
	(void)h;
#endif
}

BOOL WINAPI Mine_SetStdHandle(
	DWORD a0,
	HANDLE a1)
{
	EnterFunc();

	bool rv = 0;
	__try
	{
		rv = Functions::Original::SetStdHandle(a0, a1);
		if (rv && a1 != 0)
		{
			switch (a0)
			{
				case STD_INPUT_HANDLE:
					SetHandle("stdin", a1);
					break;
				case STD_OUTPUT_HANDLE:
					SetHandle("stdout", a1);
					break;
				case STD_ERROR_HANDLE:
					SetHandle("stderr", a1);
					break;
			}
		}
	}
	__finally
	{
		ExitFunc();
	};

	return rv;
}

HMODULE WINAPI Mine_LoadLibraryA(LPCSTR a0)
{
    EnterFunc();

    HMODULE rv = 0;
    __try {
        rv = Functions::Original::LoadLibraryA(a0);
    } __finally {
        ExitFunc();
    };
    return rv;
}

HMODULE WINAPI Mine_LoadLibraryW(LPCWSTR a0)
{
    EnterFunc();

    HMODULE rv = 0;
    __try {
        rv = Functions::Original::LoadLibraryW(a0);
    } __finally {
        ExitFunc();
    };
    return rv;
}

HMODULE WINAPI Mine_LoadLibraryExA(LPCSTR a0,
                                   HANDLE a1,
                                   DWORD a2)
{
    EnterFunc();

    HMODULE rv = 0;
    __try {
        rv = Functions::Original::LoadLibraryExA(a0, a1, a2);
    } __finally {
        ExitFunc();
    };
    return rv;
}

HMODULE WINAPI Mine_LoadLibraryExW(LPCWSTR a0,
                                   HANDLE a1,
                                   DWORD a2)
{
    EnterFunc();

    HMODULE rv = 0;
    __try {
        rv = Functions::Original::LoadLibraryExW(a0, a1, a2);
    } __finally {
        ExitFunc();
    };
    return rv;
}

DWORD WINAPI Mine_SetFilePointer(HANDLE hFile,
                                 LONG lDistanceToMove,
                                 PLONG lpDistanceToMoveHigh,
                                 DWORD dwMoveMethod)
{
    EnterFunc();

    DWORD rv = 0;
    __try {
        rv = Functions::Original::SetFilePointer(hFile,
                                 lDistanceToMove,
                                 lpDistanceToMoveHigh,
                                 dwMoveMethod);
    } __finally {
        LONG high = 0;
        if (lpDistanceToMoveHigh == nullptr) {
            lpDistanceToMoveHigh = &high;
        }

        FileInfo * pInfo = OpenFiles::RecallFile(hFile);
        if (pInfo != nullptr) {
            if (dwMoveMethod == FILE_END && lDistanceToMove == 0xffffffff) {
#if 0
                Print("<!-- SetFilePointer(APPEND, %le) -->\n",
                      pInfo->m_pwzPath);
#endif
                pInfo->m_fAppend = true;
            }
#if 0
            else if (dwMoveMethod == FILE_END) {
                Print("<!-- SetFilePointer(END:%08x:%08x, %le) -->\n",
                      (int)lDistanceToMove,
                      *lpDistanceToMoveHigh,
                      pInfo->m_pwzPath);
            }
            else if (dwMoveMethod == FILE_BEGIN) {
                Print("<!-- SetFilePointer(BEG:%08x:%08x, %le) -->\n",
                      (int)lDistanceToMove,
                      *lpDistanceToMoveHigh,
                      pInfo->m_pwzPath);
            }
            else if (dwMoveMethod == FILE_CURRENT) {
                Print("<!-- SetFilePointer(CUR:%08x:%08x, %le) -->\n",
                      (int)lDistanceToMove,
                      *lpDistanceToMoveHigh,
                      pInfo->m_pwzPath);
            }
#endif
        }
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_SetFilePointerEx(HANDLE hFile,
                                  LARGE_INTEGER liDistanceToMove,
                                  PLARGE_INTEGER lpNewFilePointer,
                                  DWORD dwMoveMethod)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::SetFilePointerEx(hFile,
                                   liDistanceToMove,
                                   lpNewFilePointer,
                                   dwMoveMethod);
    } __finally {
#if 0
        FileInfo * pInfo = OpenFiles::RecallFile(hFile);
        if (pInfo != nullptr) {
            if (dwMoveMethod == FILE_END) {
                Print("<!-- SetFilePointerEx(END:%I64d, %le) -->\n",
                      liDistanceToMove.QuadPart,
                      pInfo->m_pwzPath);
            }
            else if (dwMoveMethod == FILE_BEGIN) {
                Print("<!-- SetFilePointerEx(BEG:%I64d, %le) -->\n",
                      liDistanceToMove.QuadPart,
                      pInfo->m_pwzPath);
            }
            else if (dwMoveMethod == FILE_CURRENT) {
                Print("<!-- SetFilePointerEx(CUR:%I64d, %le) -->\n",
                      liDistanceToMove.QuadPart,
                      pInfo->m_pwzPath);
            }
        }
#endif
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_ReadFile(HANDLE a0,
                          LPVOID a1,
                          DWORD a2,
                          LPDWORD a3,
                          LPOVERLAPPED a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::ReadFile(a0, a1, a2, a3, a4);
    } __finally {
        if (rv) {
            OpenFiles::SetRead(a0, a2);
        }
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_ReadFileEx(HANDLE a0,
                            LPVOID a1,
                            DWORD a2,
                            LPOVERLAPPED a3,
                            LPOVERLAPPED_COMPLETION_ROUTINE a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::ReadFileEx(a0, a1, a2, a3, a4);
    } __finally {
        if (rv) {
            OpenFiles::SetRead(a0, a2);
        }
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_WriteFile(HANDLE a0,
                           LPCVOID a1,
                           DWORD a2,
                           LPDWORD a3,
                           LPOVERLAPPED a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::WriteFile(a0, a1, a2, a3, a4);
    } __finally {
        OpenFiles::SetWrite(a0, a2);
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_WriteFileEx(HANDLE a0,
                             LPCVOID a1,
                             DWORD a2,
                             LPOVERLAPPED a3,
                             LPOVERLAPPED_COMPLETION_ROUTINE a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::WriteFileEx(a0, a1, a2, a3, a4);
    } __finally {
        OpenFiles::SetWrite(a0, a2);
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_WriteConsoleA(HANDLE a0,
                                  const void* a1,
                                  DWORD a2,
                                  LPDWORD a3,
                                  LPVOID a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::WriteConsoleA(a0, a1, a2, a3, a4);
    } __finally {
        OpenFiles::SetWrite(a0, a2);
        ExitFunc();
    };
    return rv;
}

BOOL WINAPI Mine_WriteConsoleW(HANDLE a0,
                                  const void* a1,
                                  DWORD a2,
                                  LPDWORD a3,
                                  LPVOID a4)
{
    EnterFunc();

    bool rv = 0;
    __try {
        rv = Functions::Original::WriteConsoleW(a0, a1, a2, a3, a4);
    } __finally {
        OpenFiles::SetWrite(a0, a2);
        ExitFunc();
    };
    return rv;
}

//////////////////////////////////////////////////////////////////////////////
//
DWORD WINAPI Mine_ExpandEnvironmentStringsA(PCSTR lpSrc, PCHAR lpDst, DWORD nSize)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        rv = Functions::Original::ExpandEnvironmentStringsA(lpSrc, lpDst, nSize);
    }
    __finally {
        if (rv > 0) {
#if 0
            Print("<!-- ExpandEnvironmentStringsA(%he) -->\n", lpSrc);
#endif
        }
        ExitFunc();
    };
    return rv;
}

DWORD WINAPI Mine_ExpandEnvironmentStringsW(PCWSTR lpSrc, PWCHAR lpDst, DWORD nSize)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        rv = Functions::Original::ExpandEnvironmentStringsW(lpSrc, lpDst, nSize);
    }
    __finally {
        if (rv > 0) {
#if 0
            Print("<!-- ExpandEnvironmentStringsW(%le) -->\n", lpSrc);
#endif
        }
        ExitFunc();
    };
    return rv;
}

DWORD WINAPI Mine_GetEnvironmentVariableA(PCSTR lpName, PCHAR lpBuffer, DWORD nSize)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        rv = Functions::Original::GetEnvironmentVariableA(lpName, lpBuffer, nSize);
        //        if (rv > 0 && rv < nSize && lpBuffer != nullptr) {
        //            EnvVars::Used(lpName);
        //        }
    }
    __finally {
        EnvVars::Used(lpName);
        ExitFunc();
    };
    return rv;
}

DWORD WINAPI Mine_GetEnvironmentVariableW(PCWSTR lpName, PWCHAR lpBuffer, DWORD nSize)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        rv = Functions::Original::GetEnvironmentVariableW(lpName, lpBuffer, nSize);
        //        if (rv > 0 && rv < nSize && lpBuffer != nullptr) {
        //            EnvVars::Used(lpName);
        //        }
    }
    __finally {
        EnvVars::Used(lpName);
        ExitFunc();
    };
    return rv;
}

PCWSTR CDECL Mine_wgetenv(PCWSTR var)
{
    EnterFunc();
    PCWSTR rv = 0;
    __try {
        rv = Functions::Original::wgetenv(var);
        //        if (rv != nullptr) {
        //            EnvVars::Used(var);
        //        }
    }
    __finally {
        EnvVars::Used(var);
        ExitFunc();
    }
    return rv;
}

PCSTR CDECL Mine_getenv(PCSTR var)
{
	EnterFunc();
	PCSTR rv = 0;
	__try
	{
		rv = Functions::Original::getenv(var);
		//        if (rv) {
		//            EnvVars::Used(var);
		//        }
	}
	__finally
	{
		EnvVars::Used(var);
		ExitFunc();
	}

	return rv;
}

DWORD CDECL Mine_getenv_s(DWORD *pValue, PCHAR pBuffer, DWORD cBuffer, PCSTR varname)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        DWORD value;
        if (pValue == nullptr) {
            pValue = &value;
        }
        rv = Functions::Original::getenv_s(pValue, pBuffer, cBuffer, varname);
        //        if (rv == 0 && *pValue > 0) {
        //            EnvVars::Used(varname);
        //        }
    }
    __finally {
        EnvVars::Used(varname);
        ExitFunc();
    }
    return rv;
}

DWORD CDECL Mine_wgetenv_s(DWORD *pValue, PWCHAR pBuffer, DWORD cBuffer, PCWSTR varname)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        DWORD value;
        if (pValue == nullptr) {
            pValue = &value;
        }
        rv = Functions::Original::wgetenv_s(pValue, pBuffer, cBuffer, varname);
        //        if (rv == 0 && *pValue > 0) {
        //            EnvVars::Used(varname);
        //        }
    }
    __finally {
        EnvVars::Used(varname);
        ExitFunc();
    }
    return rv;
}

DWORD CDECL Mine_dupenv_s(PCHAR *ppBuffer, DWORD *pcBuffer, PCSTR varname)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        PCHAR pb;
        DWORD cb;
        if (ppBuffer == nullptr) {
            ppBuffer = &pb;
        }
        if (pcBuffer == nullptr) {
            pcBuffer = &cb;
        }
        rv = Functions::Original::dupenv_s(ppBuffer, pcBuffer, varname);
        //        if (rv == 0 && *pcBuffer > 0 && *ppBuffer != nullptr) {
        //            EnvVars::Used(varname);
        //        }
    }
    __finally {
        EnvVars::Used(varname);
        ExitFunc();
    }
    return rv;
}

DWORD CDECL Mine_wdupenv_s(PWCHAR *ppBuffer, DWORD *pcBuffer, PCWSTR varname)
{
    EnterFunc();
    DWORD rv = 0;
    __try {
        PWCHAR pb;
        DWORD cb;
        if (ppBuffer == nullptr) {
            ppBuffer = &pb;
        }
        if (pcBuffer == nullptr) {
            pcBuffer = &cb;
        }
        rv = Functions::Original::wdupenv_s(ppBuffer, pcBuffer, varname);
        //        if (rv == 0 && *pcBuffer > 0 && *ppBuffer != nullptr) {
        //            EnvVars::Used(varname);
        //        }
    }
    __finally {
        EnvVars::Used(varname);
        ExitFunc();
    }
    return rv;
}


/////////////////////////////////////////////////////////////
// AttachDetours
//
void AttachDetours()
{
    ThrowIfFailed(DetourTransactionBegin(), "AttachDetours DetourTransactionBegin Failed");
    ThrowIfFailed(DetourUpdateThread(GetCurrentThread()), "AttachDetours DetourUpdateThread Failed");

    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::EntryPoint, Mine_EntryPoint), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::ExitProcess, Mine_ExitProcess), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CopyFileExA, Mine_CopyFileExA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CopyFileExW, Mine_CopyFileExW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::PrivCopyFileExW, Mine_PrivCopyFileExW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateHardLinkA, Mine_CreateHardLinkA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateHardLinkW, Mine_CreateHardLinkW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateDirectoryW, Mine_CreateDirectoryW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateDirectoryExW, Mine_CreateDirectoryExW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateFileW, Mine_CreateFileW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreatePipe, Mine_CreatePipe), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateFileMappingW, Mine_CreateFileMappingW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CloseHandle, Mine_CloseHandle), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::DuplicateHandle, Mine_DuplicateHandle), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateProcessW, Mine_CreateProcessW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::CreateProcessA, Mine_CreateProcessA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::DeleteFileW, Mine_DeleteFileW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::DeviceIoControl, Mine_DeviceIoControl), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::GetFileAttributesW, Mine_GetFileAttributesW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::MoveFileA, Mine_MoveFileA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::MoveFileW, Mine_MoveFileW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::MoveFileExA, Mine_MoveFileExA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::MoveFileExW, Mine_MoveFileExW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::MoveFileWithProgressW, Mine_MoveFileWithProgressW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::SetStdHandle, Mine_SetStdHandle), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::LoadLibraryA, Mine_LoadLibraryA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::LoadLibraryW, Mine_LoadLibraryW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::LoadLibraryExA, Mine_LoadLibraryExA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::LoadLibraryExW, Mine_LoadLibraryExW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::SetFilePointer, Mine_SetFilePointer), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::SetFilePointerEx, Mine_SetFilePointerEx), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::ReadFile, Mine_ReadFile), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::ReadFileEx, Mine_ReadFileEx), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::WriteFile, Mine_WriteFile), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::WriteFileEx, Mine_WriteFileEx), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::WriteConsoleA, Mine_WriteConsoleA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::WriteConsoleW, Mine_WriteConsoleW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::ExpandEnvironmentStringsA, Mine_ExpandEnvironmentStringsA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::ExpandEnvironmentStringsW, Mine_ExpandEnvironmentStringsW), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::GetEnvironmentVariableA, Mine_GetEnvironmentVariableA), "AttachDetours DetourAttach Failed");
    ThrowIfFailed(DetourAttach(&(PVOID&)Functions::Original::GetEnvironmentVariableW, Mine_GetEnvironmentVariableW), "AttachDetours DetourAttach Failed");

    ThrowIfFailed(DetourTransactionCommit(), "AttachDetours DetourTransactionCommit Failed");
}

LONG DetachDetours(void)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DetourDetach(&(PVOID&)Functions::Original::EntryPoint, Mine_EntryPoint);
    DetourDetach(&(PVOID&)Functions::Original::ExitProcess, Mine_ExitProcess);
    DetourDetach(&(PVOID&)Functions::Original::CopyFileExA, Mine_CopyFileExA);
    DetourDetach(&(PVOID&)Functions::Original::CopyFileExW, Mine_CopyFileExW);
    DetourDetach(&(PVOID&)Functions::Original::PrivCopyFileExW, Mine_PrivCopyFileExW);
    DetourDetach(&(PVOID&)Functions::Original::CreateHardLinkA, Mine_CreateHardLinkA);
    DetourDetach(&(PVOID&)Functions::Original::CreateHardLinkW, Mine_CreateHardLinkW);
    DetourDetach(&(PVOID&)Functions::Original::CreateDirectoryW, Mine_CreateDirectoryW);
    DetourDetach(&(PVOID&)Functions::Original::CreateDirectoryExW, Mine_CreateDirectoryExW);
    DetourDetach(&(PVOID&)Functions::Original::CreateFileW, Mine_CreateFileW);
    DetourDetach(&(PVOID&)Functions::Original::CreatePipe, Mine_CreatePipe);
    DetourDetach(&(PVOID&)Functions::Original::CreateFileMappingW, Mine_CreateFileMappingW);
    DetourDetach(&(PVOID&)Functions::Original::CloseHandle, Mine_CloseHandle);
    DetourDetach(&(PVOID&)Functions::Original::DuplicateHandle, Mine_DuplicateHandle);
    DetourDetach(&(PVOID&)Functions::Original::CreateProcessW, Mine_CreateProcessW);
    DetourDetach(&(PVOID&)Functions::Original::CreateProcessA, Mine_CreateProcessA);
    DetourDetach(&(PVOID&)Functions::Original::DeleteFileW, Mine_DeleteFileW);
    DetourDetach(&(PVOID&)Functions::Original::DeviceIoControl, Mine_DeviceIoControl);
    DetourDetach(&(PVOID&)Functions::Original::GetFileAttributesW, Mine_GetFileAttributesW);
    DetourDetach(&(PVOID&)Functions::Original::MoveFileA, Mine_MoveFileA);
    DetourDetach(&(PVOID&)Functions::Original::MoveFileW, Mine_MoveFileW);
    DetourDetach(&(PVOID&)Functions::Original::MoveFileExA, Mine_MoveFileExA);
    DetourDetach(&(PVOID&)Functions::Original::MoveFileExW, Mine_MoveFileExW);
    DetourDetach(&(PVOID&)Functions::Original::MoveFileWithProgressW, Mine_MoveFileWithProgressW);
    DetourDetach(&(PVOID&)Functions::Original::SetStdHandle, Mine_SetStdHandle);
    DetourDetach(&(PVOID&)Functions::Original::LoadLibraryA, Mine_LoadLibraryA);
    DetourDetach(&(PVOID&)Functions::Original::LoadLibraryW, Mine_LoadLibraryW);
    DetourDetach(&(PVOID&)Functions::Original::LoadLibraryExA, Mine_LoadLibraryExA);
    DetourDetach(&(PVOID&)Functions::Original::LoadLibraryExW, Mine_LoadLibraryExW);
    DetourDetach(&(PVOID&)Functions::Original::SetFilePointer, Mine_SetFilePointer);
    DetourDetach(&(PVOID&)Functions::Original::SetFilePointerEx, Mine_SetFilePointerEx);
    DetourDetach(&(PVOID&)Functions::Original::ReadFile, Mine_ReadFile);
    DetourDetach(&(PVOID&)Functions::Original::ReadFileEx, Mine_ReadFileEx);
    DetourDetach(&(PVOID&)Functions::Original::WriteFile, Mine_WriteFile);
    DetourDetach(&(PVOID&)Functions::Original::WriteFileEx, Mine_WriteFileEx);
    DetourDetach(&(PVOID&)Functions::Original::WriteConsoleA, Mine_WriteConsoleA);
    DetourDetach(&(PVOID&)Functions::Original::WriteConsoleW, Mine_WriteConsoleW);
    DetourDetach(&(PVOID&)Functions::Original::ExpandEnvironmentStringsA, Mine_ExpandEnvironmentStringsA);
    DetourDetach(&(PVOID&)Functions::Original::ExpandEnvironmentStringsW, Mine_ExpandEnvironmentStringsW);
    DetourDetach(&(PVOID&)Functions::Original::GetEnvironmentVariableA, Mine_GetEnvironmentVariableA);
    DetourDetach(&(PVOID&)Functions::Original::GetEnvironmentVariableW, Mine_GetEnvironmentVariableW);

    if (Functions::Original::getenv) { DetourDetach(&(PVOID&)Functions::Original::getenv, Mine_getenv); }
    if (Functions::Original::getenv_s) { DetourDetach(&(PVOID&)Functions::Original::getenv_s, Mine_getenv_s); }
    if (Functions::Original::wgetenv) { DetourDetach(&(PVOID&)Functions::Original::wgetenv, Mine_wgetenv); }
    if (Functions::Original::wgetenv_s) { DetourDetach(&(PVOID&)Functions::Original::wgetenv, Mine_wgetenv_s); }
    if (Functions::Original::dupenv_s) { DetourDetach(&(PVOID&)Functions::Original::dupenv_s, Mine_dupenv_s); }
    if (Functions::Original::wdupenv_s) { DetourDetach(&(PVOID&)Functions::Original::wdupenv_s, Mine_wdupenv_s); }

    return DetourTransactionCommit();
}
//
//////////////////////////////////////////////////////////////////////////////

void NoteRead(PCSTR psz)
{
    FileInfo *pInfo = FileNames::FindPartial(psz);
    pInfo->m_fRead = true;
}

void NoteRead(PCWSTR pwz)
{
    FileInfo *pInfo = FileNames::FindPartial(pwz);
    pInfo->m_fRead = true;
}

void NoteWrite(PCSTR psz)
{
    FileInfo *pInfo = FileNames::FindPartial(psz);
    pInfo->m_fWrite = true;
    if (!pInfo->m_fRead) {
        pInfo->m_fCantRead = true;
    }
}

void NoteWrite(PCWSTR pwz)
{
    FileInfo *pInfo = FileNames::FindPartial(pwz);
    pInfo->m_fWrite = true;
    if (!pInfo->m_fRead) {
        pInfo->m_fCantRead = true;
    }
}

void NoteDelete(PCSTR psz)
{
    FileInfo *pInfo = FileNames::FindPartial(psz);
    if (pInfo->m_fWrite || pInfo->m_fRead) {
        pInfo->m_fCleanup = true;
    }
    else {
        pInfo->m_fDelete = true;
    }
    if (!pInfo->m_fRead) {
        pInfo->m_fCantRead = true;
    }
}

void NoteDelete(PCWSTR pwz)
{
    FileInfo *pInfo = FileNames::FindPartial(pwz);
    if (pInfo->m_fWrite || pInfo->m_fRead) {
        pInfo->m_fCleanup = true;
    }
    else {
        pInfo->m_fDelete = true;
    }
    if (!pInfo->m_fRead) {
        pInfo->m_fCantRead = true;
    }
}

void NoteCleanup(PCSTR psz)
{
    FileInfo *pInfo = FileNames::FindPartial(psz);
    pInfo->m_fCleanup = true;
}

void NoteCleanup(PCWSTR pwz)
{
    FileInfo *pInfo = FileNames::FindPartial(pwz);
    pInfo->m_fCleanup = true;
}

////////////////////////////////////////////////////////////// Logging System.
//
static bool s_bLog = 1;
static LONG s_nTlsIndent = -1;
static LONG s_nTlsThread = -1;
static LONG s_nThreadCnt = 0;

LONG EnterFunc()
{
	DWORD dwErr = GetLastError();

	LONG nIndent = 0;
	LONG nThread = 0;
	if (s_nTlsIndent >= 0)
	{
		nIndent = (LONG)(LONG_PTR)TlsGetValue(s_nTlsIndent);
		TlsSetValue(s_nTlsIndent, (PVOID)(LONG_PTR)(nIndent + 1));
	}

	if (s_nTlsThread >= 0)
	{
		nThread = (LONG)(LONG_PTR)TlsGetValue(s_nTlsThread);
	}

	SetLastError(dwErr);

	return nIndent;
}

void ExitFunc()
{
	DWORD dwErr = GetLastError();

	LONG nIndent = 0;
	LONG nThread = 0;
	if (s_nTlsIndent >= 0)
	{
		nIndent = (LONG)(LONG_PTR)TlsGetValue(s_nTlsIndent) - 1;
		// TODO: ASSERT(nIndent >= 0);
		TlsSetValue(s_nTlsIndent, (PVOID)(LONG_PTR)nIndent);
	}

	if (s_nTlsThread >= 0)
	{
		nThread = (LONG)(LONG_PTR)TlsGetValue(s_nTlsThread);
	}

	SetLastError(dwErr);
}

void Print(const CHAR *psz, ...)
{
    DWORD dwErr = GetLastError();

    if (s_bLog && psz) {
        va_list  args;
        va_start(args, psz);

        TblogV(psz, args);

        va_end(args);
    }

    SetLastError(dwErr);
}

void AssertFailed(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine)
{
    Tblog("ASSERT(%hs) failed in %s, line %d.\n", pszMsg, pszFile, nLine);
}

//////////////////////////////////////////////////////////////////////////////
//
// DLL module information
//
bool ThreadAttach(HMODULE hDll)
{
    (void)hDll;

    if (s_nTlsIndent >= 0) {
        TlsSetValue(s_nTlsIndent, (PVOID)0);
    }
    if (s_nTlsThread >= 0) {
        LONG nThread = InterlockedIncrement(&s_nThreadCnt);
        TlsSetValue(s_nTlsThread, (PVOID)(LONG_PTR)nThread);
    }
    return true;
}

bool ThreadDetach(HMODULE hDll)
{
    (void)hDll;

    if (s_nTlsIndent >= 0) {
        TlsSetValue(s_nTlsIndent, (PVOID)0);
    }
    if (s_nTlsThread >= 0) {
        TlsSetValue(s_nTlsThread, (PVOID)0);
    }
    return true;
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

    PBYTE xCreate = (PBYTE)DetourCodeFromPointer((PVOID)Functions::Original::CreateProcessW, nullptr);
    TBLOG_PAYLOAD* pPayload = nullptr;

    for (HMODULE hMod = nullptr; (hMod = DetourEnumerateModules(hMod)) != nullptr;) {
        ULONG cbData;
        PVOID pvData = DetourFindPayload(hMod, s_guidTrace, &cbData);

        if (pvData != nullptr && pPayload == nullptr) {
            pPayload = (TBLOG_PAYLOAD*)pvData;
        }

        ULONG cbMod = DetourGetModuleSize(hMod);

        if (((PBYTE)hMod) < xCreate && ((PBYTE)hMod + cbMod) > xCreate) {
            s_hKernel32 = hMod;
        }
    }

    ZeroMemory(&s_Payload, sizeof(s_Payload));

    if (pPayload == nullptr) {
        return false;
    }

    CopyMemory(&s_Payload, pPayload, sizeof(s_Payload));

    LoadStdHandleName(STD_INPUT_HANDLE, s_Payload.wzStdin, false);
    LoadStdHandleName(STD_OUTPUT_HANDLE, s_Payload.wzStdout, s_Payload.fStdoutAppend);
    LoadStdHandleName(STD_ERROR_HANDLE, s_Payload.wzStderr, s_Payload.fStderrAppend);
    s_nTraceProcessId = s_Payload.nTraceProcessId;

    GetModuleFileNameA(s_hInst, s_szDllPath, ARRAYSIZE(s_szDllPath));

    // Find hidden functions.
    Functions::Original::PrivCopyFileExW =
        (BOOL (WINAPI *)(LPCWSTR, LPCWSTR, LPPROGRESS_ROUTINE, LPVOID, LPBOOL, DWORD))
        GetProcAddress(s_hKernel32, "PrivCopyFileExW");
    if (Functions::Original::PrivCopyFileExW == nullptr) {
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
    if (error != NO_ERROR) {
        Tblog("<!-- Error detaching detours: %d -->\n", error);
    }

    TblogClose();

    if (s_nTlsIndent >= 0) {
        TlsFree(s_nTlsIndent);
    }
    if (s_nTlsThread >= 0) {
        TlsFree(s_nTlsThread);
    }
    return true;
}

inline void UpdateIfRoom(PWCHAR& pwzDst, PWCHAR pwzDstEnd, WCHAR c)
{
    if (pwzDst < pwzDstEnd) {
        *pwzDst++ = c;  // Write character if room in buffer.
    }
    else {
        pwzDst++;       // If no room, just advance pointer (to alloc calculation)
    }
}

static PCHAR RemoveReturns(PCHAR pszBuffer)
{
    PCHAR pszIn = pszBuffer;
    PCHAR pszOut = pszBuffer;

    while (*pszIn) {
        if (*pszIn == '\r') {
            pszIn++;
            continue;
        }
        *pszOut++ = *pszIn++;
    }
    *pszOut = '\0';

    return pszBuffer;
}

static PWCHAR RemoveReturns(PWCHAR pwzBuffer)
{
    PWCHAR pwzIn = pwzBuffer;
    PWCHAR pwzOut = pwzBuffer;

    while (*pwzIn) {
        if (*pwzIn == '\r') {
            pwzIn++;
            continue;
        }
        *pwzOut++ = *pwzIn++;
    }
    *pwzOut = '\0';

    return pwzBuffer;
}

PBYTE LoadFile(HANDLE hFile, DWORD cbFile)
{
    PBYTE pbFile = (PBYTE)GlobalAlloc(GPTR, cbFile + 3);
    if (pbFile == nullptr) {
        return nullptr;
    }

    DWORD cbRead = 0;
    Functions::Original::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
    Functions::Original::ReadFile(hFile, pbFile, cbFile, &cbRead, nullptr);

    // Make sure the file is zero terminated.
    pbFile[cbRead + 0] = 0;
    pbFile[cbRead + 1] = 0;
    pbFile[cbRead + 2] = 0;

    return pbFile;
}

PWCHAR More(PCWSTR pwzPath, PWCHAR pwzDst, PWCHAR pwzDstEnd)
{
    HANDLE hFile = Functions::Original::CreateFileW(pwzPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    FileInfo *pInfo = FileNames::FindPartial(pwzPath);
    pInfo->m_fAbsorbed = true;

    DWORD cbFile = Functions::Original::SetFilePointer(hFile, 0, nullptr, FILE_END);
    DWORD cbRead = 0;

    PCHAR pszFile = (PCHAR)GlobalAlloc(GPTR, cbFile + 2);   // 2 bytes null for Unicode or Ascii.
    if (pszFile != nullptr) {
        Functions::Original::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
        Functions::Original::ReadFile(hFile, pszFile, cbFile, &cbRead, nullptr);

        if (((PUCHAR)pszFile)[0] == 0xff && ((PUCHAR)pszFile)[1] == 0xfe) {
            // Unicode
            PWCHAR pwzFile = ((PWCHAR)pszFile) + 1;
            PCWSTR pwzIn = pwzFile;
            while (*pwzIn) {
                if (*pwzIn == ' ' || *pwzIn == '\t' || *pwzIn == '\r' || *pwzIn == '\n') {
                    UpdateIfRoom(pwzDst, pwzDstEnd, ' ');
                    while (*pwzIn == ' ' || *pwzIn == '\t' || *pwzIn == '\r' || *pwzIn == '\n') {
                        pwzIn++;
                    }
                }
                else {
                    UpdateIfRoom(pwzDst, pwzDstEnd, *pwzIn++);
                }
            }
        }
        else {
            PCSTR pszIn = pszFile;
            while (*pszIn) {
                if (*pszIn == ' ' || *pszIn == '\t' || *pszIn == '\r' || *pszIn == '\n') {
                    UpdateIfRoom(pwzDst, pwzDstEnd, ' ');
                    while (*pszIn == ' ' || *pszIn == '\t' || *pszIn == '\r' || *pszIn == '\n') {
                        pszIn++;
                    }
                }
                else {
                    UpdateIfRoom(pwzDst, pwzDstEnd, *pszIn++);
                }
            }
        }

        GlobalFree(pszFile);
    }

    Functions::Original::CloseHandle(hFile);

    return pwzDst;
}

// This function is called twice.  On the first call, pwzDstEnd <= pwzDst and
// no data is copied, but pwzDst is advanced so we can see how big of a
// buffer is needed to hold the command line.
//
// On the second call, the command line is actually populated.
PWCHAR LoadCommandLine(PCWSTR pwz, PWCHAR pwzDst, PWCHAR pwzDstEnd)
{
    while (*pwz) {
        PCWSTR pwzArgBeg = nullptr;
        PCWSTR pwzArgEnd = nullptr;
        WCHAR cQuote = '\0';
        bool fMore = false;

        if (*pwz == '@') {
            fMore = true;
            pwz++;
        }

        if (*pwz == '\"' || *pwz == '\'') {
            cQuote = *pwz++;

            pwzArgBeg = pwz;
            while (*pwz != '\0' && *pwz != cQuote) {
                pwz++;
            }
            pwzArgEnd = pwz;

            if (*pwz == cQuote) {
                pwz++;
            }
        }
        else {
            pwzArgBeg = pwz;
            while (*pwz != '\0' && *pwz != ' ' && *pwz != '\t' && *pwz != '\n' && *pwz != '\r') {
                pwz++;
            }
            pwzArgEnd = pwz;
        }

        if (fMore) {
            // More arguments!
            WCHAR wzPath[MAX_PATH];
            PWCHAR pwzPath = wzPath;
            PCWSTR pwzTmp = pwzArgBeg + 1;
            while (pwzTmp < pwzArgEnd && pwzPath < wzPath + ARRAYSIZE(wzPath)-2) {
                *pwzPath++ = *pwzTmp++;
            }
            *pwzPath = '\0';

            PWCHAR pwzOut = More(wzPath, pwzDst, pwzDstEnd);
            if (pwzOut != nullptr) {
                pwzDst = pwzOut;

                cQuote = 0;
                pwzArgBeg = pwzArgEnd;
            }
        }

        if (cQuote) {
            UpdateIfRoom(pwzDst, pwzDstEnd, cQuote);
        }
        for (; pwzArgBeg < pwzArgEnd; pwzArgBeg++) {
            UpdateIfRoom(pwzDst, pwzDstEnd, *pwzArgBeg);
        }
        if (cQuote) {
            UpdateIfRoom(pwzDst, pwzDstEnd, cQuote);
        }

        if (*pwz) {
            UpdateIfRoom(pwzDst, pwzDstEnd, ' ');
        }

        // skip over separating spaces.
        while (*pwz == ' ' || *pwz == '\t' || *pwz == '\n' || *pwz == '\r') {
            pwz++;
        }
    }
    return pwzDst;
}

void TestHandle(PCSTR pszName, HANDLE h)
{
    FileInfo *pInfo = OpenFiles::RecallFile(h);

    if (pInfo != nullptr) {
#if 1 // Ignore PIPEs.
        if (FileNames::PrefixMatch(pInfo->m_pwzPath, L"\\\\.\\PIPE\\")) {
            // Ignore;
        }
        else
#endif
            if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\conout$")) {
            // Ignore;
        }
        else if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\conin$")) {
            // Ignore;
        }
        else if (FileNames::SuffixMatch(pInfo->m_pwzPath, L"\\nul")) {
            // Ignore;
        }
        else {
            Tblog("<%hs%hs>%le</%hs>\n",
                  pszName, pInfo->m_fAppend ? " append=\"true\"" : "", pInfo->m_pwzPath, pszName);
        }
    }
    else {
        Tblog("<!-- hand: %hs (%x) ***Unknown*** -->\n", pszName, h);
    }
}

LONG WINAPI DetourAttachIf(PVOID *ppPointer, PVOID pDetour)
{
    if (*ppPointer == nullptr) {
        Tblog("<!-- DetourAttachIf failed: %p -->\n", pDetour);
        return NO_ERROR;
    }

    PDETOUR_TRAMPOLINE pRealTrampoline;
    PVOID pRealTarget;
    PVOID pRealDetour;

    LONG err = DetourAttachEx(ppPointer, pDetour, &pRealTrampoline, &pRealTarget, &pRealDetour);
    if (err == NO_ERROR) {
        // Tblog("<!-- DetourAttachIf %p at %p -->\n", pDetour, pRealTarget);
        return NO_ERROR;
    }
    return err;
}

int WINAPI Mine_EntryPoint(void)
{
	// This function is invoked instead of the process EntryPoint (Functions::Original::EntryPoint).

	TblogOpen();

	SaveEnvironment();

	{
		CHAR szExeName[MAX_PATH];
		CHAR szId[128];
		CHAR szParent[128];
		WCHAR wzPath[MAX_PATH];
		PCHAR pszExeName = szExeName;

		// Get the base command line (skipping over the executable name)
		PCWSTR pwzLine = GetCommandLineW();
		if (*pwzLine == '\"')
		{
			pwzLine++;
			while (*pwzLine && *pwzLine != '\"')
			{
				pwzLine++;
			}

			if (*pwzLine == '\"')
			{
				pwzLine++;
			}
		}
		else
		{
			while (*pwzLine && *pwzLine != ' ' && *pwzLine != '\t')
			{
				pwzLine++;
			}
		}

		while (*pwzLine && (*pwzLine == ' ' || *pwzLine == '\t'))
		{
			pwzLine++;
		}

		// Get the root executable name.
		if (GetModuleFileNameA(0, szExeName, ARRAYSIZE(szExeName)))
		{
			PCHAR psz = szExeName;

			while (*psz)
			{
				psz++;
			}

			while (psz > szExeName && psz[-1] != ':' && psz[-1] != '\\' && psz[-1] != '/')
			{
				psz--;
			}

			pszExeName = psz;
			while (*psz && *psz != '.')
			{
				psz++;
			}

			*psz = '\0';
		}
		else
		{
			szExeName[0] = '\0';
		}

		// Start the XML process node.
		Tblog("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		{
			PCHAR pszId = szId;
			PCHAR pszParent = szParent;
			for (DWORD i = 0; i < s_Payload.nGeneology; i++)
			{
				pszId = SafePrintf(pszId, 16, "%d.", s_Payload.rGeneology[i]);
				if (i < s_Payload.nGeneology - 1)
				{
					pszParent = SafePrintf(pszParent, 16, "%d.", s_Payload.rGeneology[i]);
				}
			}

			*pszId = '\0';
			*pszParent = '\0';

			if (szParent[0] == '\0')
			{
				Tblog("<t:Process id=\"::%hs::\"", szId);
			}
			else
			{
				Tblog("<t:Process id=\"::%hs::\" parentId=\"::%hs::\"", szId, szParent);
			}

			Tblog(" par=\"%ls\" exe=\"%hs\"", s_Payload.wzParents, pszExeName);

			bool drop = false;
			PCWSTR pwzz = s_Payload.wzzDrop;
			while (*pwzz)
			{
				if (Compare(pwzz, pszExeName) == 0)
				{
					// match
					drop = true;
					break;
				}

				pwzz += Size(pwzz) + 1;
			}

			if (drop)
			{
				Tblog(" drop=\"true\"");
			}
		}

		{
			PWCHAR pwz = s_Payload.wzParents;
			while (*pwz)
			{
				pwz++;
			}

			*pwz++ = '/';
			PCSTR psz = pszExeName;
			while (*psz)
			{
				*pwz++ = *psz++;
			}

			*pwz = '\0';
		}


		if (HasChar(pwzLine, '|'))
		{
			Tblog(" pipes=\"true\"");
		}

		if (HasChar(pwzLine, '>'))
		{
			Tblog(" redirects=\"true\"");
		}

		Tblog(" xmlns:t=\"http://schemas.microsoft.com/research/tracebld/2008\">\n");

		// Get the directory.
		DWORD dwSize = GetCurrentDirectoryA(ARRAYSIZE(szExeName), szExeName);
		if (dwSize > 0 && dwSize < ARRAYSIZE(szExeName))
		{
			Tblog("<t:Directory>%hs</t:Directory>\n", szExeName);
		}

		// Get the real executable name.
		wzPath[0] = '\0';
		if (GetModuleFileNameA(0, szExeName, ARRAYSIZE(szExeName)))
		{
			FileInfo *pInfo = FileNames::FindPartial(szExeName);
			Tblog("<t:Executable>%ls</t:Executable>\n",
					FileNames::ParameterizeName(wzPath, ARRAYSIZE(wzPath), pInfo));
		}

		// Construct the processed command line.
		PWCHAR pwzDstEnd = (PWCHAR)pwzLine;
		PWCHAR pwzDst = pwzDstEnd;
		pwzDst = LoadCommandLine(pwzLine, pwzDst, pwzDstEnd);
		DWORD wcNew = (DWORD)((pwzDst - pwzDstEnd) + 1);
		PWCHAR pwzFin = (PWCHAR)GlobalAlloc(GPTR, wcNew * sizeof(WCHAR));
		pwzDst = pwzFin;
		pwzDstEnd = pwzFin + wcNew;
		pwzDst = LoadCommandLine(pwzLine, pwzDst, pwzDstEnd);
		*pwzDst = '\0';

		FileNames::ParameterizeLine(pwzFin, pwzFin + wcNew);
		if (HasSpace(wzPath))
		{
			Tblog("<t:Line>&quot;%le&quot; %le</t:Line>\n", wzPath, pwzFin);
		}
		else
		{
			Tblog("<t:Line>%le %le</t:Line>\n", wzPath, pwzFin);
		}

		TestHandle("t:StdIn", GetStdHandle(STD_INPUT_HANDLE));
		TestHandle("t:StdOut", GetStdHandle(STD_OUTPUT_HANDLE));
		TestHandle("t:StdErr", GetStdHandle(STD_ERROR_HANDLE));
	}

	if (FindMsvcr())
	{
		FindProc(&(PVOID&)Functions::Original::getenv, "getenv");
		FindProc(&(PVOID&)Functions::Original::wgetenv, "_wgetenv");
		FindProc(&(PVOID&)Functions::Original::getenv_s, "getenv_s");
		FindProc(&(PVOID&)Functions::Original::wgetenv_s, "_wgetenv_s");
		FindProc(&(PVOID&)Functions::Original::dupenv_s, "_dupenv_s");
		FindProc(&(PVOID&)Functions::Original::wdupenv_s, "_wdupenv_s");

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DetourAttachIf(&(PVOID&)Functions::Original::getenv, Mine_getenv);
		DetourAttachIf(&(PVOID&)Functions::Original::getenv_s, Mine_getenv_s);
		DetourAttachIf(&(PVOID&)Functions::Original::wgetenv, Mine_wgetenv);
		DetourAttachIf(&(PVOID&)Functions::Original::wgetenv, Mine_wgetenv_s);
		DetourAttachIf(&(PVOID&)Functions::Original::dupenv_s, Mine_dupenv_s);
		DetourAttachIf(&(PVOID&)Functions::Original::wdupenv_s, Mine_wdupenv_s);

		DetourTransactionCommit();
	}

	return Functions::Original::EntryPoint();
}

void WINAPI Mine_ExitProcess(UINT a0)
{
	if (a0 & 0x80000000)
	{
		Tblog("<t:Return>%d</t:Return>\n", -(int)a0);
	}
	else {
		Tblog("<t:Return>%d</t:Return>\n", a0);
	}

	FileNames::Dump();
	EnvVars::Dump();

	TblogClose();

	Functions::Original::ExitProcess(a0);
}

bool APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

	if (DetourIsHelperProcess())
	{
		return true;
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();
		Functions::Original::EntryPoint = (int (WINAPI *)(void))DetourGetEntryPoint(nullptr);
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
