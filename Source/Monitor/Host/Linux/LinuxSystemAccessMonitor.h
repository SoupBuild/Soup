#pragma once
#include "ILinuxSystemMonitor.h"

namespace Monitor::Linux
{
	/// The monitor wrapper that maps Linux events to the shared events
	class LinuxSystemAccessMonitor : public ILinuxSystemMonitor
	{
	private:
		std::shared_ptr<ISystemAccessMonitor> _monitor;

	public:
		LinuxSystemAccessMonitor(
			std::shared_ptr<ISystemAccessMonitor> monitor) :
			_monitor(std::move(monitor))
		{
		}

		void OnInitialize() override final
		{
			Log::Diag("LinuxSystemAccessMonitor::OnInitialize");
		}

		void OnShutdown(bool hadError) override final
		{
			Log::Diag("LinuxSystemAccessMonitor::OnShutdown {}", hadError);

			// TODO: Exit with failure
		}

		void OnError(std::string_view message) override final
		{
			Log::Error("LinuxSystemAccessMonitor::Error - {}", message);
		}

		// FileApi
		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileWrite(pathname, wasBlocked);
		}

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
		}

		void OnLinkAt(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
		}

		void OnMkdir(std::string_view path, uint32_t mode, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileWrite(path, wasBlocked);
		}

		void OnMkdirAt(int32_t dirfd, std::string_view path, uint32_t mode, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileWrite(path, wasBlocked);
		}

		void OnOpen(std::string_view path, int32_t oflag, int32_t result) override final
		{
			bool isWrite = (oflag & O_WRONLY) != 0 || (oflag & O_RDWR) != 0;
			bool wasBlocked = false;
			if (isWrite)
			{
				TouchFileWrite(path, wasBlocked);
			}
			else
			{
				bool exists = result != -1;
				TouchFileRead(path, exists, wasBlocked);
			}
		}

		void OnOpenAt(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			bool isWrite = (flags & O_WRONLY) != 0 || (flags & O_RDWR) != 0;
			bool wasBlocked = false;
			if (isWrite)
			{
				TouchFileWrite(pathname, wasBlocked);
			}
			else
			{
				bool exists = result != -1;
				TouchFileRead(pathname, exists, wasBlocked);
			}
		}
		
		void OnOpenAt2(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
		{
			bool isWrite = (flags & O_WRONLY) != 0 || (flags & O_RDWR) != 0;
			bool wasBlocked = false;
			if (isWrite)
			{
				TouchFileWrite(pathname, wasBlocked);
			}
			else
			{
				bool exists = result != -1;
				TouchFileRead(pathname, exists, wasBlocked);
			}
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(oldpath, wasBlocked);
			TouchFileWrite(newpath, wasBlocked);
		}

		void OnRenameAt(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(oldpath, wasBlocked);
			TouchFileWrite(newpath, wasBlocked);
		}

		void OnRenameAt2(int32_t oldfd, std::string_view oldpath, int32_t newfd, std::string_view newpath, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(oldpath, wasBlocked);
			TouchFileWrite(newpath, wasBlocked);
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		// ProcessApi
		void OnClone(int32_t result) override final
		{
		}

		void OnClone3(int32_t result) override final
		{
		}


		void OnExecve(std::string_view file, int32_t result) override final
		{
		}

		void OnExecveAt(std::string_view file, int32_t result) override final
		{
		}

		void OnFork(int32_t result) override final
		{
		}

		void OnVFork(int32_t result) override final
		{
		}

	private:
		void OnCreateProcess(bool wasDetoured, std::wstring_view applicationName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			OnCreateProcess(wasDetoured, converter.to_bytes(applicationName.data()));
		}

		void OnCreateProcess(bool wasDetoured, std::string_view applicationName)
		{
			_monitor->OnCreateProcess(applicationName, wasDetoured);
		}

		void TouchFileRead(std::wstring_view fileName, bool exists, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileRead(converter.to_bytes(fileName.data()), exists, wasBlocked);
		}

		void TouchFileRead(std::string_view fileName, bool exists, bool wasBlocked)
		{
			// Verify not a special file
			if (!IsSpecialFile(fileName))
			{
				_monitor->TouchFileRead(Path::Parse(fileName), exists, wasBlocked);
			}
		}

		void TouchFileWrite(std::wstring_view fileName, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileWrite(converter.to_bytes(fileName.data()), wasBlocked);
		}

		void TouchFileWrite(std::string_view fileName, bool wasBlocked)
		{
			// Verify not a special file
			if (!IsSpecialFile(fileName))
			{
				_monitor->TouchFileWrite(Path::Parse(fileName), wasBlocked);
			}
		}

		void TouchFileDelete(std::wstring_view fileName, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDelete(converter.to_bytes(fileName.data()), wasBlocked);
		}

		void TouchFileDelete(std::string_view fileName, bool wasBlocked)
		{
			_monitor->TouchFileDelete(Path::Parse(fileName), wasBlocked);
		}

		void TouchFileDeleteOnClose(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDeleteOnClose(converter.to_bytes(fileName.data()));
		}

		void TouchFileDeleteOnClose(std::string_view fileName)
		{
			_monitor->TouchFileDeleteOnClose(Path::Parse(fileName));
		}

		bool IsSpecialFile(std::string_view fileName)
		{
			// Check if the file name is a pipe or the standard input/output streams
			return false;
		}

		void SearchPath(std::wstring_view path, std::wstring_view filename)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			auto pathString = converter.to_bytes(path.data());
			auto filenameString = converter.to_bytes(filename.data());
			SearchPath(pathString, filenameString);
		}

		void SearchPath(std::string_view path, std::string_view filename)
		{
			_monitor->SearchPath(path, filename);
		}
	};
}