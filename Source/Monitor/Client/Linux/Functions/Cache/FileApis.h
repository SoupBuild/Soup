#pragma once

extern "C"
{
	typedef int  (*open_ptr) (const char *path, int oflag, ...);
} 

namespace Monitor::Linux::Functions::Cache::FileApi
{
	open_ptr open;
}