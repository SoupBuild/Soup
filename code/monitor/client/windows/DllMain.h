#pragma once

#include "Helpers.h"

#include "Functions/Overrides/FileApi.h"
#include "Functions/Overrides/LibLoaderApi.h"
#include "Functions/Overrides/ProcessEnv.h"
#include "Functions/Overrides/ProcessThreadsApi.h"
#include "Functions/Overrides/UndocumentedApi.h"
#include "Functions/Overrides/WinBase.h"

#include "AttachDetours.h"

using namespace Monitor::Windows;

bool APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
	(void)hModule;
	(void)lpReserved;

#ifdef ENABLE_MONITOR_DEBUG
	while (!IsDebuggerPresent())
	{
		Sleep(0);
	}
#endif

	if (DetourIsHelperProcess())
	{
		return true;
	}

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DetourRestoreAfterWith();
		Functions::Cache::UndocumentedApi::EntryPoint = (int (WINAPI *)(void))DetourGetEntryPoint(nullptr);
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