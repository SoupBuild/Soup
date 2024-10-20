#pragma once 

namespace Monitor::Linux
{
	class DetourForkCallback : public IDetourCallback
	{
	public:
		DetourForkCallback(std::shared_ptr<IDetourCallback> callback1, std::shared_ptr<IDetourCallback> callback2) :
			_callback1(std::move(callback1)),
			_callback2(std::move(callback2))
		{
		}

		void OnInitialize() override final
		{
			_callback1->OnInitialize();
			_callback2->OnInitialize();
		}

		void OnShutdown(bool hadError) override final
		{
			_callback1->OnShutdown(hadError);
			_callback2->OnShutdown(hadError);
		}

		void OnError(std::string_view message) override final
		{
			_callback1->OnError(message);
			_callback2->OnError(message);
		}

		// FileApi
		void OnOpen(std::string_view path, int32_t oflag, int32_t result) override final
		{
			_callback1->OnOpen(path, oflag, result);
			_callback2->OnOpen(path, oflag, result);
		}

		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			_callback1->OnCreat(pathname, result);
			_callback2->OnCreat(pathname, result);
		}

		void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			_callback1->OnOpenat(dirfd, pathname, flags, result);
			_callback2->OnOpenat(dirfd, pathname, flags, result);
		}

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_callback1->OnLink(oldpath, newpath, result);
			_callback2->OnLink(oldpath, newpath, result);
		}

		void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
			_callback1->OnLinkat(olddirfd, oldpath, newdirfd, newpath, flags, result);
			_callback2->OnLinkat(olddirfd, oldpath, newdirfd, newpath, flags, result);
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			_callback1->OnRename(oldpath, newpath, result);
			_callback2->OnRename(oldpath, newpath, result);
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			_callback1->OnUnlink(pathname, result);
			_callback2->OnUnlink(pathname, result);
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			_callback1->OnRemove(pathname, result);
			_callback2->OnRemove(pathname, result);
		}

		void OnFOpen(std::string_view pathname, std::string_view mode, uint64_t result) override final
		{
			_callback1->OnFOpen(pathname, mode, result);
			_callback2->OnFOpen(pathname, mode, result);
		}

		void OnFDOpen(int32_t fd, std::string_view mode) override final
		{
			_callback1->OnFDOpen(fd, mode);
			_callback2->OnFDOpen(fd, mode);
		}

		void OnFReopen(std::string_view pathname, std::string_view mode) override final
		{
			_callback1->OnFReopen(pathname, mode);
			_callback2->OnFReopen(pathname, mode);
		}

		void OnMkdir(std::string_view path, std::string_view mode) override final
		{
			_callback1->OnMkdir(path, mode);
			_callback2->OnMkdir(path, mode);
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			_callback1->OnRmdir(pathname, result);
			_callback2->OnRmdir(pathname, result);
		}

		// ProcessApi
		void OnSystem(std::string_view command) override final
		{
			_callback1->OnSystem(command);
			_callback2->OnSystem(command);
		}

		void OnFork() override final
		{
			_callback1->OnFork();
			_callback2->OnFork();
		}

		void OnExecl(std::string_view path, int32_t result) override final
		{
			_callback1->OnExecl(path, result);
			_callback2->OnExecl(path, result);
		}

		void OnExeclp(std::string_view file, int32_t result) override final
		{
			_callback1->OnExeclp(file, result);
			_callback2->OnExeclp(file, result);
		}

		void OnExecle(std::string_view path, int32_t result) override final
		{
			_callback1->OnExecle(path, result);
			_callback2->OnExecle(path, result);
		}

		void OnExecv(std::string_view path, int32_t result) override final
		{
			_callback1->OnExecv(path, result);
			_callback2->OnExecv(path, result);
		}

		void OnExecvp(std::string_view file, int32_t result) override final
		{
			_callback1->OnExecvp(file, result);
			_callback2->OnExecvp(file, result);
		}

		void OnExecvpe(std::string_view file, int32_t result) override final
		{
			_callback1->OnExecvpe(file, result);
			_callback2->OnExecvpe(file, result);
		}

	private:
		std::shared_ptr<IDetourCallback> _callback1;
		std::shared_ptr<IDetourCallback> _callback2;
	};
}
