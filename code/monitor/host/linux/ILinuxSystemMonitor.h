#pragma once 

namespace Monitor::Linux
{
	class ILinuxSystemMonitor
	{
	public:
		virtual void OnInitialize() = 0;
		virtual void OnShutdown(bool hadError) = 0;
		virtual void OnError(std::string_view message) = 0;

		// FileApi
		virtual void OnCreat(std::string_view pathname, int32_t result) = 0;
		virtual void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) = 0;
		virtual void OnLinkAt(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) = 0;
		virtual void OnMkdir(std::string_view path, uint32_t mode, int32_t result) = 0;
		virtual void OnMkdirAt(int32_t dirfd, std::string_view path, uint32_t mode, int32_t result) = 0;
		virtual void OnOpen(std::string_view path, int32_t oflag, int32_t result) = 0;
		virtual void OnOpenAt(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) = 0;
		virtual void OnOpenAt2(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) = 0;
		virtual void OnRemove(std::string_view pathname, int32_t result) = 0;
		virtual void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) = 0;
		virtual void OnRenameAt(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) = 0;
		virtual void OnRenameAt2(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) = 0;
		virtual void OnRmdir(std::string_view pathname, int32_t result) = 0;
		virtual void OnUnlink(std::string_view pathname, int32_t result) = 0;

		// ProcessApi
		virtual void OnClone(int32_t result) = 0;
		virtual void OnClone3(int32_t result) = 0;
		virtual void OnExecve(std::string_view file, int32_t result) = 0;
		virtual void OnExecveAt(std::string_view file, int32_t result) = 0;
		virtual void OnFork(int32_t result) = 0;
		virtual void OnVFork(int32_t result) = 0;
	};
}
