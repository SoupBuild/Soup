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
		link,
		linkat,
		rename,
		unlink,
		remove,
		fopen,
		fdopen,
		freopen,
		mkdir,
		rmdir,

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