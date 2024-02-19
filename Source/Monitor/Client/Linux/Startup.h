#pragma once

extern "C"
{
	typedef int  (*open_ptr) (const char *path, int oflag, ...);
} 

class Startup
{
public:
	open_ptr open;

public:
	Startup() :
		open((open_ptr)dlsym(RTLD_NEXT, "open"))
	{
	}
};

static Startup startup;

int open(const char *path, int oflag, ...)
{
	va_list args;
	va_start(args, oflag);
	va_end(args);
	return startup.open(path, oflag);
}