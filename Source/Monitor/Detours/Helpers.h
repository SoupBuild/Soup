#pragma once

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

bool EndsWithIgnoreCase(std::wstring_view lhs, std::wstring_view value)
{
	auto lhsLength = lhs.size();
	auto valueLength = value.size();
	if (lhsLength < valueLength)
		return false;
	
	auto offset = lhsLength - valueLength;
	auto lhsEnd = lhs.substr(offset, valueLength);
	return std::equal(
		lhsEnd.begin(),
		lhsEnd.end(),
		value.begin(),
		value.end(),
		[](wchar_t a, wchar_t b)
		{
			return std::tolower(a) == std::tolower(b);
		});
}

bool EndsWithIgnoreCase(std::string_view lhs, std::string_view value)
{
	auto lhsLength = lhs.size();
	auto valueLength = value.size();
	if (lhsLength < valueLength)
		return false;
	
	auto offset = lhsLength - valueLength;
	auto lhsEnd = lhs.substr(offset, valueLength);
	return std::equal(
		lhsEnd.begin(),
		lhsEnd.end(),
		value.begin(),
		value.end(),
		[](char a, char b)
		{
			return std::tolower(a) == std::tolower(b);
		});
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