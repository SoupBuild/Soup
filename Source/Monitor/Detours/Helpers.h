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

		s_hPipe = Functions::Cache::CreateFileW(wzPipe, GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
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
	Functions::Cache::ExitProcess(9990);
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
		if (!Functions::Cache::WriteFile(s_hPipe, &s_rMessage, s_rMessage.nBytes, &cbWritten, nullptr))
		{
			Functions::Cache::ExitProcess(9991);
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

		Functions::Cache::WriteFile(s_hPipe, &s_rMessage, 4, &cbWritten, nullptr);
		FlushFileBuffers(s_hPipe);
		CloseHandle(s_hPipe);
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

bool CreateProcessInternals(
	HANDLE hProcess,
	PCHAR pszId)
{
	EnterCriticalSection(&s_csChildPayload);

	ZeroMemory(&s_ChildPayload, sizeof(s_ChildPayload));
	CopyMemory(&s_ChildPayload, &s_Payload, sizeof(s_ChildPayload));

	s_ChildPayload.nParentProcessId = GetCurrentProcessId();
	s_ChildPayload.rGeneology[s_ChildPayload.nGeneology]
		= (DWORD)InterlockedIncrement(&s_nChildCnt);
	s_ChildPayload.nGeneology++;

	DetourCopyPayloadToProcess(hProcess, s_guidTrace, &s_ChildPayload, sizeof(s_ChildPayload));

	for (DWORD i = 0; i < s_ChildPayload.nGeneology; i++)
	{
		pszId = SafePrintf(pszId, 16, "%d.", s_ChildPayload.rGeneology[i]);
	}

	*pszId = '\0';

	LeaveCriticalSection(&s_csChildPayload);

	return true;
}