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
		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnCreat(pathname, result);
			_monitor2->OnCreat(pathname, result);
		}
		
		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnLink(oldpath, newpath, result);
			_monitor2->OnLink(oldpath, newpath, result);
		}

		void OnLinkAt(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
			_monitor1->OnLinkAt(olddirfd, oldpath, newdirfd, newpath, flags, result);
			_monitor2->OnLinkAt(olddirfd, oldpath, newdirfd, newpath, flags, result);
		}

		void OnMkdir(std::string_view path, uint32_t mode, int32_t result) override final
		{
			_monitor1->OnMkdir(path, mode, result);
			_monitor2->OnMkdir(path, mode, result);
		}

		void OnMkdirAt(int32_t dirfd, std::string_view path, uint32_t mode, int32_t result) override final
		{
			_monitor1->OnMkdirAt(dirfd, path, mode, result);
			_monitor2->OnMkdirAt(dirfd, path, mode, result);
		}

		void OnOpen(std::string_view path, int32_t oflag, int32_t result) override final
		{
			_monitor1->OnOpen(path, oflag, result);
			_monitor2->OnOpen(path, oflag, result);
		}

		void OnOpenAt(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			_monitor1->OnOpenAt(dirfd, pathname, flags, result);
			_monitor2->OnOpenAt(dirfd, pathname, flags, result);
		}
		
		void OnOpenAt2(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			_monitor1->OnOpenAt2(dirfd, pathname, flags, result);
			_monitor2->OnOpenAt2(dirfd, pathname, flags, result);
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnRemove(pathname, result);
			_monitor2->OnRemove(pathname, result);
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnRename(oldpath, newpath, result);
			_monitor2->OnRename(oldpath, newpath, result);
		}

		void OnRenameAt(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnRenameAt(oldfd, oldpath, newfd, newpath, result);
			_monitor2->OnRenameAt(oldfd, oldpath, newfd, newpath, result);
		}

		void OnRenameAt2(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) override final
		{
			_monitor1->OnRenameAt2(oldfd, oldpath, newfd, newpath, result);
			_monitor2->OnRenameAt2(oldfd, oldpath, newfd, newpath, result);
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnRmdir(pathname, result);
			_monitor2->OnRmdir(pathname, result);
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			_monitor1->OnUnlink(pathname, result);
			_monitor2->OnUnlink(pathname, result);
		}

		// ProcessApi
		void OnClone(int32_t result) override final
		{
			_monitor1->OnClone(result);
			_monitor2->OnClone(result);
		}

		void OnClone3(int32_t result) override final
		{
			_monitor1->OnClone3(result);
			_monitor2->OnClone3(result);
		}

		void OnExecve(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecve(file, result);
			_monitor2->OnExecve(file, result);
		}

		void OnExecveAt(std::string_view file, int32_t result) override final
		{
			_monitor1->OnExecveAt(file, result);
			_monitor2->OnExecveAt(file, result);
		}

		void OnFork(int32_t result) override final
		{
			_monitor1->OnFork(result);
			_monitor2->OnFork(result);
		}

		void OnVFork(int32_t result) override final
		{
			_monitor1->OnVFork(result);
			_monitor2->OnVFork(result);
		}
	};
}