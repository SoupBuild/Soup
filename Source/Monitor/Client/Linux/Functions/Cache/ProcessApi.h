#pragma once

extern "C"
{
	typedef int (*system_ptr) (const char *command);

	typedef pid_t (*fork_ptr) (void);
	
	typedef int (*execl_ptr) (const char *path, const char *arg, ...);
	typedef int (*execlp_ptr) (const char *file, const char *arg, ...);
	// typedef int (*execle_ptr) (const char *path, const char *arg, ..., char * const envp[]);
	typedef int (*execv_ptr) (const char *path, char *const argv[]);
	typedef int (*execvp_ptr) (const char *file, char *const argv[]);
	typedef int (*execvpe_ptr) (const char *file, char *const argv[], char *const envp[]); 
} 

namespace Monitor::Linux::Functions::Cache::ProcessApi
{
	system_ptr system;

	fork_ptr fork;

	execl_ptr execl;
	execlp_ptr execlp;
	// execle_ptr execle;
	execv_ptr execv;
	execvp_ptr execvp;
	execvpe_ptr execvpe;
}