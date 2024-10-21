#pragma once

extern "C"
{
	typedef int (*system_ptr) (const char *command);

	typedef pid_t (*fork_ptr) (void);
	typedef pid_t (*vfork_ptr) (void);

	typedef int (*clone_ptr) (int (*fn)(void*), void *stack, int flags, void * arg, ...);
	typedef int (*__clone2_ptr) (int (*fn)(void*), void *stack, int flags, void * arg, ...);
	typedef int (*clone3_ptr) (int (*fn)(void*), void *stack, int flags, void * arg, ...);
	
	typedef int (*execl_ptr) (const char *path, const char *arg, ...);
	typedef int (*execlp_ptr) (const char *file, const char *arg, ...);
	typedef int (*execle_ptr) (const char *path, const char *arg, ...);
	typedef int (*execv_ptr) (const char *path, char *const argv[]);
	typedef int (*execvp_ptr) (const char *file, char *const argv[]);
	typedef int (*execvpe_ptr) (const char *file, char *const argv[], char *const envp[]);

	typedef int (*execve_ptr) (const char *pathname, char *const argv[], char *const envp[]);
	typedef int (*execveat_ptr) (int dirfd, const char *pathname, char *const argv[], char *const envp[], int flags);
	typedef int (*fexecve_ptr) (int fd, char *const argv[], char *const envp[]);
}

namespace Monitor::Linux::Functions::Cache::ProcessApi
{
	system_ptr system;

	fork_ptr fork;
	vfork_ptr vfork;

	clone_ptr clone;
	__clone2_ptr __clone2;
	clone3_ptr clone3;

	execl_ptr execl;
	execlp_ptr execlp;
	execle_ptr execle;
	execv_ptr execv;
	execvp_ptr execvp;
	execvpe_ptr execvpe;

	execve_ptr execve;
	execveat_ptr execveat;
	fexecve_ptr fexecve;
}