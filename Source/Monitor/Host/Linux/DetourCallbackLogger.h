#pragma once 
#include "IDetourCallback.h"

namespace Monitor::Linux
{
	class DetourCallbackLogger : public IDetourCallback
	{
	public:
		DetourCallbackLogger(std::ostream& stream) :
			m_stream(stream)
		{
		}

		virtual void OnInitialize() override final
		{
			m_stream << "Initialize: " << std::endl;
		}

		virtual void OnShutdown(bool hadError) override final
		{
			m_stream << "Shutdown: " << hadError << std::endl;
		}

		virtual void OnError(std::string_view message) override final
		{
			m_stream << "Error: " << message << std::endl;
		}

		// FileApi
		void OnOpen(std::string_view path, int32_t oflag, int32_t result) override final
		{
			m_stream << "open: " << path << " " << oflag << std::endl;
		}

		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			m_stream << "creat: " << pathname << std::endl;
		}

		void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			m_stream << "openat: " << pathname << " " << flags << std::endl;
		}

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			m_stream << "link: " << oldpath << " " << newpath << std::endl;
		}

		void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
			m_stream << "linkat: " << oldpath << " " << newpath << std::endl;
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			m_stream << "rename: " << oldpath << " " << newpath << std::endl;
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			m_stream << "unlink: " << pathname << std::endl;
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			m_stream << "remove: " << pathname << std::endl;
		}

		void OnFOpen(std::string_view pathname, std::string_view mode, uint64_t result) override final
		{
			m_stream << "fopen: " << pathname << " " << mode << std::endl;
		}

		void OnFDOpen(int32_t fd, std::string_view mode) override final
		{
			m_stream << "fdopen: "  << mode << std::endl;
		}

		void OnFReopen(std::string_view pathname, std::string_view mode) override final
		{
			m_stream << "freopen: " << pathname << " " << mode << std::endl;
		}

		void OnMkdir(std::string_view path, std::string_view mode) override final
		{
			m_stream << "mkdir: " << path << " " << mode << std::endl;
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			m_stream << "rmdir: " << pathname << std::endl;
		}

		// ProcessApi
		void OnSystem(std::string_view command) override final
		{
			m_stream << "system: " << command << std::endl;
		}

		void OnFork() override final
		{
			m_stream << "fork: " << std::endl;
		}

		void OnExecl(std::string_view path, int32_t result) override final
		{
			m_stream << "execl: " << path << std::endl;
		}

		void OnExeclp(std::string_view file, int32_t result) override final
		{
			m_stream << "execlp: " << file << std::endl;
		}

		void OnExecle(std::string_view path, int32_t result) override final
		{
			m_stream << "execle: " << path << std::endl;
		}

		void OnExecv(std::string_view path, int32_t result) override final
		{
			m_stream << "execv: " << path << std::endl;
		}

		void OnExecvp(std::string_view file, int32_t result) override final
		{
			m_stream << "execvp: " << file << std::endl;
		}

		void OnExecvpe(std::string_view file, int32_t result) override final
		{
			m_stream << "execvpe: " << file << std::endl;
		}

		void OnExecve(std::string_view file, int32_t result) override final
		{
			m_stream << "execve: " << file << std::endl;
		}

		void OnExecveat(std::string_view file, int32_t result) override final
		{
			m_stream << "execveat: " << file << std::endl;
		}

		void OnFexecve(int32_t result) override final
		{
			m_stream << "fexecve: " << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
