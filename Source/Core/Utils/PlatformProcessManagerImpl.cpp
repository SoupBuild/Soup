// <copyright file="PlatformProcessManagerImpl.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#include "PlatformProcessManagerImpl.h"
#include <Windows.h>

using namespace Soup;

struct SmartHandle
{
    SmartHandle(HANDLE handle) :
        _handle(handle)
    {
    }

    SmartHandle() :
        _handle(INVALID_HANDLE_VALUE)
    {
    }

    ~SmartHandle()
    {
        Close();
    }

    void Close()
    {
        if (_handle != INVALID_HANDLE_VALUE)
        {
            if (!CloseHandle(_handle))
                throw "TODO: Better error";
            _handle = INVALID_HANDLE_VALUE;
        }
    }

    HANDLE _handle;
};

int PlatformProcessManagerImpl::Execute(
    const char* application,
    char* arguments,
    const char* workingDirectory)
{
    // Setup the input/output streams
    // Set the bInheritHandle flag so pipe handles are inherited.
    SECURITY_ATTRIBUTES securityAttributes; 
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
    securityAttributes.bInheritHandle = true; 
    securityAttributes.lpSecurityDescriptor = nullptr; 

    // Create a pipe for the child process's STDOUT.
    SmartHandle childStdOutRead;
    SmartHandle childStdOutWrite;
    if (!CreatePipe(&childStdOutRead._handle, &childStdOutWrite._handle, &securityAttributes, 0))
        return -1;

    // Ensure the read handle to the pipe for STDOUT is not inherited.
    if (!SetHandleInformation(childStdOutRead._handle, HANDLE_FLAG_INHERIT, 0))
        return -1;

    // Create a pipe for the child process's STDIN.
    SmartHandle childStdInRead;
    SmartHandle childStdInWrite;
    if (!CreatePipe(&childStdInRead._handle, &childStdInWrite._handle, &securityAttributes, 0))
        return -1;

    // Ensure the write handle to the pipe for STDIN is not inherited.
    if (!SetHandleInformation(childStdInWrite._handle, HANDLE_FLAG_INHERIT, 0))
        return -1;

    // Setup the process creation parameters
    LPSECURITY_ATTRIBUTES processAttributes = nullptr;
    LPSECURITY_ATTRIBUTES threadAttributes = nullptr;
    bool inheritHandles = true;
    DWORD creationFlags = 0;
    void* environment = nullptr;

    STARTUPINFOA startupInfo = {};
    ZeroMemory(&startupInfo, sizeof(STARTUPINFOA));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.hStdError = childStdOutWrite._handle;
    startupInfo.hStdOutput = childStdOutWrite._handle;
    startupInfo.hStdInput = childStdInRead._handle;
    startupInfo.dwFlags |= STARTF_USESTDHANDLES;

    PROCESS_INFORMATION processInfo = {};
    ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

    // Start the process
    if (!CreateProcessA(
        application,
        arguments,
        processAttributes,
        threadAttributes,
        inheritHandles,
        creationFlags,
        environment,
        workingDirectory,
        &startupInfo,
        &processInfo))
    {
        return GetLastError();
    }

    // Throw the process and thread handles into smart pointers to ensure they are cleaned up
    auto processHandle = SmartHandle(processInfo.hProcess);
    auto threadHandle = SmartHandle(processInfo.hThread);

    // Read all and write to stdout
    DWORD dwRead, dwWritten;
    const int BufferSize = 6;
    CHAR chBuf[BufferSize];
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!WriteFile(hParentStdOut, "Hello there", 9, &dwWritten, nullptr))
        return -1;

    // Wait until child process exits.
    WaitForSingleObject(processHandle._handle, INFINITE);

    // Close the child write handle to ensure we stop reading
    childStdOutWrite.Close();

    while (true)
    {
        if(!ReadFile(childStdOutRead._handle, chBuf, BufferSize, &dwRead, nullptr))
            return -2;
        if (dwRead == 0)
            break;

        if (!WriteFile(hParentStdOut, chBuf, dwRead, &dwWritten, nullptr))
            return -3;

        // Stop reading if we ran out of things to read
        if (dwRead != BufferSize)
            break;
    }

    return 0;
}
