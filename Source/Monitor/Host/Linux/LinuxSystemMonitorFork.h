#pragma once 

namespace Monitor::Linux
{
	class LinuxSystemMonitorFork : public ILinuxSystemMonitor
	{
	private:
		std::shared_ptr<ILinuxSystemMonitor> _monitor1;
		std::shared_ptr<ILinuxSystemMonitor> _monitor2;

	public:
		LinuxSystemMonitorFork(
			std::shared_ptr<ILinuxSystemMonitor> monitor1,
			std::shared_ptr<ILinuxSystemMonitor> monitor2) :
			_monitor1(std::move(monitor1)),
			_monitor2(std::move(monitor2))
		{
		}

		void OnInitialize() override final
		{
			_monitor1->OnInitialize();
			_monitor2->OnInitialize();
		}

		void OnShutdown(bool hadError) override final
		{
			_monitor1->OnShutdown(hadError);
			_monitor2->OnShutdown(hadError);
		}

		void OnError(std::string_view message) override final
		{
			_monitor1->OnError(message);
			_monitor2->OnError(message);
		}

		// FileApi
		void OnOpen(std::string_view path, int32_t oflag, int32_t result) override final
		{
			_monitor1->OnOpen(path, oflag, result);
			_monitor2->OnOpen(path, oflag, result);
		}

		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnCreat(pathname, result);
			_monitor2->OnCreat(pathname, result);
		}

		void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			_monitor1->OnOpenat(dirfd, pathname, flags, result);
			_monitor2->OnOpenat(dirfd, pathname, flags, result);
		}

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnLink(oldpath, newpath, result);
			_monitor2->OnLink(oldpath, newpath, result);
		}

		void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
			_monitor1->OnLinkat(olddirfd, oldpath, newdirfd, newpath, flags, result);
			_monitor2->OnLinkat(olddirfd, oldpath, newdirfd, newpath, flags, result);
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnRename(oldpath, newpath, result);
			_monitor2->OnRename(oldpath, newpath, result);
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnUnlink(pathname, result);
			_monitor2->OnUnlink(pathname, result);
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnRemove(pathname, result);
			_monitor2->OnRemove(pathname, result);
		}

		void OnFOpen(std::string_view pathname, std::string_view mode, uint64_t result) override final
		{
			_monitor1->OnFOpen(pathname, mode, result);
			_monitor2->OnFOpen(pathname, mode, result);
		}

		void OnFDOpen(int32_t fd, std::string_view mode) override final
		{
			_monitor1->OnFDOpen(fd, mode);
			_monitor2->OnFDOpen(fd, mode);
		}

		void OnFReopen(std::string_view pathname, std::string_view mode) override final
		{
			_monitor1->OnFReopen(pathname, mode);
			_monitor2->OnFReopen(pathname, mode);
		}

		void OnMkdir(std::string_view path, std::string_view mode) override final
		{
			_monitor1->OnMkdir(path, mode);
			_monitor2->OnMkdir(path, mode);
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnRmdir(pathname, result);
			_monitor2->OnRmdir(pathname, result);
		}

		// ProcessApi
		void OnSystem(std::string_view command) override final
		{
			_monitor1->OnSystem(command);
			_monitor2->OnSystem(command);
		}

		void OnFork() override final
		{
			_monitor1->OnFork();
			_monitor2->OnFork();
		}

		void OnVFork() override final
		{
			_monitor1->OnVFork();
			_monitor2->OnVFork();
		}

		void OnClone() override final
		{
			_monitor1->OnClone();
			_monitor2->OnClone();
		}

		void OnClone2() override final
		{
			_monitor1->OnClone2();
			_monitor2->OnClone2();
		}

		void OnClone3() override final
		{
			_monitor1->OnClone3();
			_monitor2->OnClone3();
		}

		void OnExecl(std::string_view path, int32_t result) override final
		{
			_monitor1->OnExecl(path, result);
			_monitor2->OnExecl(path, result);
		}

		void OnExeclp(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExeclp(file, result);
			_monitor2->OnExeclp(file, result);
		}

		void OnExecle(std::string_view path, int32_t result) override final
		{
			_monitor1->OnExecle(path, result);
			_monitor2->OnExecle(path, result);
		}

		void OnExecv(std::string_view path, int32_t result) override final
		{
			_monitor1->OnExecv(path, result);
			_monitor2->OnExecv(path, result);
		}

		void OnExecvp(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecvp(file, result);
			_monitor2->OnExecvp(file, result);
		}

		void OnExecvpe(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecvpe(file, result);
			_monitor2->OnExecvpe(file, result);
		}

		void OnExecve(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecve(file, result);
			_monitor2->OnExecve(file, result);
		}

		void OnExecveat(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecveat(file, result);
			_monitor2->OnExecveat(file, result);
		}

		void OnFexecve(int32_t result) override final
		{
			_monitor1->OnFexecve(result);
			_monitor2->OnFexecve(result);
		}
	};
}
