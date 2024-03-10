#pragma once

namespace Monitor::Linux
{
	#ifdef SOUP_BUILD
	export
	#endif
	enum class DetourEventType : uint32_t
	{
		// FileApi
		open,
		creat,
		openat,
		fopen,
		fdopen,
		freopen,
		mkdir,

		// ProcessApi
		system,
		fork,
		execl,
		execlp,
		execle,
		execv,
		execvp,
		execvpe,
	};
}