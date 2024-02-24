#pragma once

#include "LinuxConnectionManager.h"

extern "C"
{
	typedef int  (*open_ptr) (const char *path, int oflag, ...);
} 

class Detours
{
public:
	open_ptr open;

public:
	Detours()
	{
	}

public:
	void AttachDetours()
	{
		open = (open_ptr)dlsym(RTLD_NEXT, "open");
	}
};

static Detours detours;

int open(const char *path, int oflag, ...)
{
	va_list args;
	va_start(args, oflag);
	va_end(args);
	return detours.open(path, oflag);
}