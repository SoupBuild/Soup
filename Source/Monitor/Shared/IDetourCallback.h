#pragma once 

namespace Monitor
{
	export class IDetourCallback
	{
	public:
		virtual void OnExit() = 0;
		virtual void OnError() = 0;
		virtual void OnCopyFile() = 0;
		virtual void OnCreateDirectory(const std::string_view directory) = 0;
		virtual void OnCreateFile(const std::string_view filename) = 0;
		virtual void OnCreateHardLink() = 0;
		virtual void OnCreateProcess() = 0;
		virtual void OnDeleteFile(const std::string_view filename) = 0;
		virtual void OnGetEnvironmentVariable() = 0;
		virtual void OnGetFileAttributes(const std::string_view filename) = 0;
		virtual void OnLoadLibrary() = 0;
		virtual void OnMoveFile() = 0;
		virtual void OnOpenFile(const std::string_view filename) = 0;
	};
}
