#pragma once 

namespace Monitor::Linux
{
	class IDetourCallback
	{
	public:
		virtual void OnInitialize() = 0;
		virtual void OnShutdown(bool hadError) = 0;
		virtual void OnError(std::string_view message) = 0;

		// FileApi
		virtual void OnOpen(std::string_view path, int32_t oflag, int32_t result) = 0;
		virtual void OnCreat(std::string_view pathname, int32_t result) = 0;
		virtual void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) = 0;
		virtual void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) = 0;
		virtual void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) = 0;
		virtual void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) = 0;
		virtual void OnUnlink(std::string_view pathname, int32_t result) = 0;
		virtual void OnRemove(std::string_view pathname, int32_t result) = 0;
		virtual void OnFOpen(std::string_view pathname, std::string_view mode, uint64_t result) = 0;
		virtual void OnFDOpen(int32_t fd, std::string_view mode) = 0;
		virtual void OnFReopen(std::string_view pathname, std::string_view mode) = 0;
		virtual void OnMkdir(std::string_view path, std::string_view mode) = 0;
		virtual void OnRmdir(std::string_view pathname, int32_t result) = 0;

		// ProcessApi
		virtual void OnSystem(std::string_view command) = 0;
		virtual void OnFork() = 0;
		virtual void OnExecl(std::string_view path, int32_t result) = 0;
		virtual void OnExeclp(std::string_view file, int32_t result) = 0;
		virtual void OnExecle(std::string_view path, int32_t result) = 0;
		virtual void OnExecv(std::string_view path, int32_t result) = 0;
		virtual void OnExecvp(std::string_view file, int32_t result) = 0;
		virtual void OnExecvpe(std::string_view file, int32_t result) = 0;
		virtual void OnExecve(std::string_view file, int32_t result) = 0;
		virtual void OnExecveat(std::string_view file, int32_t result) = 0;
		virtual void OnFexecve(int32_t result) = 0;
	};
}
