#pragma once
#include "IDetourCallback.h"

namespace Monitor::Linux
{
	/// The monitor callback wrapper that maps Windows Detour events to the shared events
	class DetourMonitorCallback : public IDetourCallback
	{
	private:
		std::shared_ptr<IMonitorCallback> _callback;

	public:
		DetourMonitorCallback(
			std::shared_ptr<IMonitorCallback> callback) :
			_callback(std::move(callback))
		{
		}

		void OnInitialize() override final
		{
			Log::Diag("DetourMonitorCallback::OnInitialize");
		}

		void OnShutdown(bool hadError) override final
		{
			Log::Diag("DetourMonitorCallback::OnShutdown {}", hadError);

			// TODO: Exit with failure
		}

		void OnError(std::string_view message) override final
		{
			Log::Error("DetourMonitorCallback::Error - {}", message);
		}

		// FileApi
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

		void OnCreat(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileWrite(pathname, wasBlocked);
		}

		void OnOpenat(int32_t dirfd, std::string_view pathname, int32_t flags, int32_t result) override final
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

		void OnLink(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
		}

		void OnLinkat(int32_t olddirfd, std::string_view oldpath, int32_t newdirfd, std::string_view newpath, int32_t flags, int32_t result) override final
		{
		}

		void OnRename(std::string_view oldpath, std::string_view newpath, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(oldpath, wasBlocked);
			TouchFileWrite(newpath, wasBlocked);
		}

		void OnUnlink(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		void OnRemove(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		void OnFOpen(std::string_view pathname, std::string_view mode) override final
		{
			// TouchFileWrite(pathName, wasBlocked);
		}

		void OnFDOpen(int32_t fd, std::string_view mode) override final
		{
			// TouchFileWrite(pathName, wasBlocked);
		}

		void OnFReopen(std::string_view pathname, std::string_view mode) override final
		{
			// TouchFileWrite(pathName, wasBlocked);
		}

		void OnMkdir(std::string_view path, std::string_view mode) override final
		{
			bool wasBlocked = false;
			TouchFileWrite(path, wasBlocked);
		}

		void OnRmdir(std::string_view pathname, int32_t result) override final
		{
			bool wasBlocked = false;
			TouchFileDelete(pathname, wasBlocked);
		}

		// ProcessApi
		void OnSystem(std::string_view command) override final
		{
		}

		void OnFork() override final
		{
		}

		void OnExecl(std::string_view path, int32_t result) override final
		{
		}

		void OnExeclp(std::string_view file, int32_t result) override final
		{
		}

		void OnExecle(std::string_view path, int32_t result) override final
		{
		}

		void OnExecv(std::string_view path, int32_t result) override final
		{
		}

		void OnExecvp(std::string_view file, int32_t result) override final
		{
		}

		void OnExecvpe(std::string_view file, int32_t result) override final
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
			_callback->OnCreateProcess(applicationName, wasDetoured);
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
				_callback->TouchFileRead(Path::Parse(fileName), exists, wasBlocked);
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
				_callback->TouchFileWrite(Path::Parse(fileName), wasBlocked);
			}
		}

		void TouchFileDelete(std::wstring_view fileName, bool wasBlocked)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDelete(converter.to_bytes(fileName.data()), wasBlocked);
		}

		void TouchFileDelete(std::string_view fileName, bool wasBlocked)
		{
			_callback->TouchFileDelete(Path::Parse(fileName), wasBlocked);
		}

		void TouchFileDeleteOnClose(std::wstring_view fileName)
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
			TouchFileDeleteOnClose(converter.to_bytes(fileName.data()));
		}

		void TouchFileDeleteOnClose(std::string_view fileName)
		{
			_callback->TouchFileDeleteOnClose(Path::Parse(fileName));
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
			_callback->SearchPath(path, filename);
		}
	};
}
