// <copyright file="ProcessManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::System
{
	/// <summary>
	/// A platform specific process executable using system
	/// </summary>
	export class ProcessManager
	{
	public:
		typedef void OutputCallBackFunction(void* context, const char*, int);

		static void GetProcessFileName(char* buffer, int length)
		{
			// Pass in null handle to get current process information
			auto handle = nullptr;
			auto sizeRead = GetModuleFileNameA(
				handle,
				buffer,
				length);
			if (sizeRead == 0)
			{
				throw GetLastError();
			}
			else if (sizeRead == length)
			{
				// Note: This may have perfectly fit, but we are being lazy
				throw "Ran out of room in the buffer";
			}
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		static int Execute(
			const char* application,
			char* arguments,
			const char* workingDirectory,
			OutputCallBackFunction stdOutputCallback,
			void* stdOutputContext,
			OutputCallBackFunction stdErrorCallback,
			void* stdErrorContext)
		{
			// Setup the input/output streams

			// TODO: We need to read from the buffer to ensure it doesn't deadlock on the wait forever
			int pipeBufferSize = 5 * 1024 * 1024;

			// Set the bInheritHandle flag so pipe handles are inherited.
			SECURITY_ATTRIBUTES securityAttributes; 
			securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
			securityAttributes.bInheritHandle = true; 
			securityAttributes.lpSecurityDescriptor = nullptr; 

			// Create a pipe for the child process's STDOUT.
			SmartHandle childStdOutRead;
			SmartHandle childStdOutWrite;
			if (!CreatePipe(&childStdOutRead._handle, &childStdOutWrite._handle, &securityAttributes, pipeBufferSize))
				return -1;

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!SetHandleInformation(childStdOutRead._handle, HANDLE_FLAG_INHERIT, 0))
				return -1;

			// Create a pipe for the child process's STDERR.
			SmartHandle childStdErrRead;
			SmartHandle childStdErrWrite;
			if (!CreatePipe(&childStdErrRead._handle, &childStdErrWrite._handle, &securityAttributes, pipeBufferSize))
				return -1;

			// Ensure the read handle to the pipe for STDERR is not inherited.
			if (!SetHandleInformation(childStdErrRead._handle, HANDLE_FLAG_INHERIT, 0))
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
			startupInfo.hStdError = childStdErrWrite._handle;
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

			// Wait until child process exits.
			WaitForSingleObject(processHandle._handle, INFINITE);

			// Get the exit code
			DWORD exitCode;
			if (!GetExitCodeProcess(processHandle._handle, &exitCode))
				return -1;

			// Close the child write handle to ensure we stop reading
			childStdOutWrite.Close();
			childStdErrWrite.Close();

			// Read all and write to stdout
			// TODO: May want to switch over to a background thread with peak to read in order
			DWORD dwRead = -1;
			const int BufferSize = 256;
			char buffer[BufferSize + 1];

			// Read on output
			while (true)
			{
				if(!ReadFile(childStdOutRead._handle, buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				buffer[dwRead] = '\0';
				stdOutputCallback(stdOutputContext, buffer, dwRead);
			}

			// Read all errors
			while (true)
			{
				if(!ReadFile(childStdErrRead._handle, buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				buffer[dwRead] = '\0';
				stdErrorCallback(stdErrorContext, buffer, dwRead);
			}

			return exitCode;
		}
	};
}
