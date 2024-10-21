#pragma once

#include "Functions/Cache/FileApi.h"
#include "Functions/Cache/ProcessApi.h"

namespace Monitor::Linux
{
	void AttachDetours()
	{
		// FileApi
		Functions::Cache::FileApi::open = (open_ptr)dlsym(RTLD_NEXT, "open");
		Functions::Cache::FileApi::creat = (creat_ptr)dlsym(RTLD_NEXT, "creat");
		Functions::Cache::FileApi::openat = (openat_ptr)dlsym(RTLD_NEXT, "openat");
		Functions::Cache::FileApi::link = (link_ptr)dlsym(RTLD_NEXT, "link");
		Functions::Cache::FileApi::linkat = (linkat_ptr)dlsym(RTLD_NEXT, "linkat");
		Functions::Cache::FileApi::rename = (rename_ptr)dlsym(RTLD_NEXT, "rename");
		Functions::Cache::FileApi::unlink = (unlink_ptr)dlsym(RTLD_NEXT, "unlink");
		Functions::Cache::FileApi::remove = (remove_ptr)dlsym(RTLD_NEXT, "remove");
		Functions::Cache::FileApi::fopen = (fopen_ptr)dlsym(RTLD_NEXT, "fopen");
		Functions::Cache::FileApi::fdopen = (fdopen_ptr)dlsym(RTLD_NEXT, "fdopen");
		Functions::Cache::FileApi::freopen = (freopen_ptr)dlsym(RTLD_NEXT, "freopen");
		Functions::Cache::FileApi::mkdir = (mkdir_ptr)dlsym(RTLD_NEXT, "mkdir");

		// ProcessApi
		Functions::Cache::ProcessApi::system = (system_ptr)dlsym(RTLD_NEXT, "system");
		Functions::Cache::ProcessApi::fork = (fork_ptr)dlsym(RTLD_NEXT, "fork");
		Functions::Cache::ProcessApi::execl = (execl_ptr)dlsym(RTLD_NEXT, "execl");
		Functions::Cache::ProcessApi::execlp = (execlp_ptr)dlsym(RTLD_NEXT, "execlp");
		// Functions::Cache::ProcessApi::execle = (execle_ptr)dlsym(RTLD_NEXT, "execle");
		Functions::Cache::ProcessApi::execv = (execv_ptr)dlsym(RTLD_NEXT, "execv");
		Functions::Cache::ProcessApi::execvp = (execvp_ptr)dlsym(RTLD_NEXT, "execvp");
		Functions::Cache::ProcessApi::execvpe = (execvpe_ptr)dlsym(RTLD_NEXT, "execvpe");
		
		Functions::Cache::ProcessApi::execve = (execve_ptr)dlsym(RTLD_NEXT, "execve");
		Functions::Cache::ProcessApi::execveat = (execveat_ptr)dlsym(RTLD_NEXT, "execveat");
		Functions::Cache::ProcessApi::fexecve = (fexecve_ptr)dlsym(RTLD_NEXT, "fexecve");
	}
}
