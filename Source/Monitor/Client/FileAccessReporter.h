
typedef HANDLE (WINAPI *CreateFileW_t)(
    __in     LPCWSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
    );

typedef HANDLE (WINAPI *CreateFileA_t)(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
    );

CreateFileW_t Real_CreateFileW;
CreateFileA_t Real_CreateFileA;

HANDLE WINAPI Detoured_CreateFileW(
    _In_     LPCWSTR               lpFileName,
    _In_     DWORD                 dwDesiredAccess,
    _In_     DWORD                 dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_     DWORD                 dwCreationDisposition,
    _In_     DWORD                 dwFlagsAndAttributes,
    _In_opt_ HANDLE                hTemplateFile)
{
    std::cout << "CreateFileW" << std::endl;
    return Real_CreateFileW(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}

HANDLE WINAPI Detoured_CreateFileA(
    _In_     LPCSTR                lpFileName,
    _In_     DWORD                 dwDesiredAccess,
    _In_     DWORD                 dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_     DWORD                 dwCreationDisposition,
    _In_     DWORD                 dwFlagsAndAttributes,
    _In_opt_ HANDLE                hTemplateFile)
{
    std::cout << "CreateFileA" << std::endl;
    return Real_CreateFileA(
        lpFileName,
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile);
}

DllExport bool AttachDetours()
{
    LONG hr;

    if (!DetourRestoreAfterWith())
        return false;

    hr = DetourTransactionBegin();
    if (hr != NO_ERROR)
        return false;

    hr = DetourUpdateThread(GetCurrentThread());
    if (hr != NO_ERROR)
        return false;

    Real_CreateFileA = ::CreateFileA;
    hr = DetourAttach((PVOID*)&Real_CreateFileA, Detoured_CreateFileA);
    if (hr != ERROR_SUCCESS)
        return false;

    Real_CreateFileW = ::CreateFileW;
    hr = DetourAttach((PVOID*)&Real_CreateFileW, Detoured_CreateFileW);
    if (hr != ERROR_SUCCESS)
        return false;

    hr = DetourTransactionCommit();
    if (hr != NO_ERROR)
        return false;

    return true;
}

DllExport bool DetachDetours()
{
    LONG hr;

    hr = DetourTransactionBegin();
    if (hr != NO_ERROR)
        return false;

    hr = DetourUpdateThread(GetCurrentThread());
    if (hr != NO_ERROR)
        return false;

    hr = DetourDetach(&(PVOID&)Real_CreateFileA, Detoured_CreateFileA);
    if (hr != NO_ERROR)
        return false;

    hr = DetourDetach(&(PVOID&)Real_CreateFileW, Detoured_CreateFileW);
    if (hr != NO_ERROR)
        return false;

    hr = DetourTransactionCommit();
    if (hr != NO_ERROR)
        return false;

    return true;
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    (void)hinst;
    (void)reserved;

    if (DetourIsHelperProcess())
    {
        return true;
    }

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        if (!AttachDetours())
            return false;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (!DetachDetours())
            return false;
    }

    return true;
}

