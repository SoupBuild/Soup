#pragma once

extern "C"
{
	typedef int (*open_ptr) (const char *path, int oflag, ... /* mode_t mode */ );
	typedef int (*creat_ptr) (const char *pathname, mode_t mode);
	typedef int (*openat_ptr) (int dirfd, const char *pathname, int flags, ... /* mode_t mode */ );
	typedef int (*link_ptr) (const char *oldpath, const char *newpath);
	typedef int (*linkat_ptr) (int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags);
	typedef int (*rename_ptr) (const char *oldpath, const char *newpath); 
	typedef int (*unlink_ptr) (const char *pathname);
	typedef int (*remove_ptr) (const char *pathname);

	typedef FILE* (*fopen_ptr) (const char * pathname, const char * mode);
	typedef FILE* (*fdopen_ptr) (int fd, const char *mode);
	typedef FILE* (*freopen_ptr) (const char * pathname, const char * mode, FILE* stream);

	typedef int (*mkdir_ptr) (const char *path, mode_t mode);
	typedef int (*rmdir_ptr) (const char *pathname); 
} 

namespace Monitor::Linux::Functions::Cache::FileApi
{
	open_ptr open;
	creat_ptr creat;
	openat_ptr openat;
	link_ptr link;
	linkat_ptr linkat;
	rename_ptr rename;
	unlink_ptr unlink;
	remove_ptr remove;

	fopen_ptr fopen;
	fdopen_ptr fdopen;
	freopen_ptr freopen;

	mkdir_ptr mkdir;
	rmdir_ptr rmdir;
}