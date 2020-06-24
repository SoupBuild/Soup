#pragma once

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


///////////////////////////////////////

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
    Functions::Cache::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
    Functions::Cache::ReadFile(hFile, pbFile, cbFile, &cbRead, nullptr);

    // Make sure the file is zero terminated.
    pbFile[cbRead + 0] = 0;
    pbFile[cbRead + 1] = 0;
    pbFile[cbRead + 2] = 0;

    return pbFile;
}

PWCHAR More(PCWSTR pwzPath, PWCHAR pwzDst, PWCHAR pwzDstEnd)
{
    HANDLE hFile = Functions::Cache::CreateFileW(pwzPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        return nullptr;
    }

    FileInfo *pInfo = FileNames::FindPartial(pwzPath);
    pInfo->m_fAbsorbed = true;

    DWORD cbFile = Functions::Cache::SetFilePointer(hFile, 0, nullptr, FILE_END);
    DWORD cbRead = 0;

    PCHAR pszFile = (PCHAR)GlobalAlloc(GPTR, cbFile + 2);   // 2 bytes null for Unicode or Ascii.
    if (pszFile != nullptr) {
        Functions::Cache::SetFilePointer(hFile, 0, nullptr, FILE_BEGIN);
        Functions::Cache::ReadFile(hFile, pszFile, cbFile, &cbRead, nullptr);

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

    Functions::Cache::CloseHandle(hFile);

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
