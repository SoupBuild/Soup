#pragma once 
#include "ILinuxSystemMonitor.h"

namespace Monitor::Linux
{
	class LinuxSystemLoggerMonitor : public ILinuxSystemMonitor
	{
	private:
		std::ostream& m_stream;

	public:
		LinuxSystemLoggerMonitor(std::ostream& stream) :
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
			m_stream << "open(" << path << ", " << oflag << ") = " << result << std::endl;
		}

		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			m_stream << "creat(" << pathname << ") = " << result << std::endl;
		}

		void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			m_stream << "openat(" << dirfd << ", " << pathname << ", " << flags << ") = " << result << std::endl;
		}

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			m_stream << "link(" << oldpath << ", " << newpath << ") = " << result << std::endl;
		}

		void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
			m_stream << "linkat(" << olddirfd << ", "<< oldpath << ", " << newdirfd << ", " << newpath << ", " << flags << ") = " << result << std::endl;
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			m_stream << "rename(" << oldpath << ", " << newpath << ") = " << result << std::endl;
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			m_stream << "unlink(" << pathname << ") = " << result << std::endl;
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			m_stream << "remove(" << pathname << ") = " << result << std::endl;
		}

		void OnFOpen(std::string_view pathname, std::string_view mode, uint64_t result) override final
		{
			m_stream << "fopen(" << pathname << " " << mode << ") = " << result << std::endl;
		}

		void OnFDOpen(int32_t fd, std::string_view mode, int32_t result) override final
		{
			m_stream << "fdopen(" << fd << ", " << mode << ") = " << result << std::endl;
		}

		void OnFReopen(std::string_view pathname, std::string_view mode, int32_t result) override final
		{
			m_stream << "freopen(" << pathname << ", " << mode << ") = " << result << std::endl;
		}

		void OnMkdir(std::string_view path, uint32_t mode, int32_t result) override final
		{
			m_stream << "mkdir(" << path << ", " << mode << ") = " << result << std::endl;
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			m_stream << "rmdir(" << pathname << ") = " << result << std::endl;
		}

		// ProcessApi
		void OnSystem(std::string_view command, int32_t result) override final
		{
			m_stream << "system(" << command << ") = " << result << std::endl;
		}

		void OnFork(int32_t result) override final
		{
			m_stream << "fork() = " << result << std::endl;
		}

		void OnVFork(int32_t result) override final
		{
			m_stream << "vfork() = " << result << std::endl;
		}

		void OnClone(int32_t result) override final
		{
			m_stream << "clone() = " << result << std::endl;
		}

		void OnClone2(int32_t result) override final
		{
			m_stream << "__clone2() = " << result << std::endl;
		}

		void OnClone3(int32_t result) override final
		{
			m_stream << "clone3() = " << result << std::endl;
		}

		void OnExecl(std::string_view path, int32_t result) override final
		{
			m_stream << "execl(" << path << ") = " << result << std::endl;
		}

		void OnExeclp(std::string_view file, int32_t result) override final
		{
			m_stream << "execlp(" << file << ") = " << result << std::endl;
		}

		void OnExecle(std::string_view path, int32_t result) override final
		{
			m_stream << "execle(" << path << ") = " << result << std::endl;
		}

		void OnExecv(std::string_view path, int32_t result) override final
		{
			m_stream << "execv(" << path << ") = " << result << std::endl;
		}

		void OnExecvp(std::string_view file, int32_t result) override final
		{
			m_stream << "execvp(" << file << ") = " << result << std::endl;
		}

		void OnExecvpe(std::string_view file, int32_t result) override final
		{
			m_stream << "execvpe(" << file << ") = " << result << std::endl;
		}

		void OnExecve(std::string_view file, int32_t result) override final
		{
			m_stream << "execve(" << file << ") = " << result << std::endl;
		}

		void OnExecveat(std::string_view file, int32_t result) override final
		{
			m_stream << "execveat(" << file << ") = " << result << std::endl;
		}

		void OnFexecve(int32_t result) override final
		{
			m_stream << "fexecve() = " << result << std::endl;
		}
	};
}