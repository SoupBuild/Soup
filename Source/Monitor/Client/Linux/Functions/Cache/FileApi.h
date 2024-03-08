#pragma once

extern "C"
{
	typedef int  (*open_ptr) (const char *path, int oflag, ...);
	
	typedef FILE* (*fopen_ptr) (const char * pathname, const char * mode);
	typedef FILE* (*fdopen_ptr) (int fd, const char *mode);
	typedef FILE* (*freopen_ptr) (const char * pathname, const char * mode, FILE* stream);

	typedef int (*mkdir_ptr) (const char *path, mode_t mode);
} 

namespace Monitor::Linux::Functions::Cache::FileApi
{
	open_ptr open;

	fopen_ptr fopen;
	fdopen_ptr fdopen;
	freopen_ptr freopen;

	mkdir_ptr mkdir;
}