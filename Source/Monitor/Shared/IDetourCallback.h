#pragma once 

namespace Monitor
{
	export class IDetourCallback
	{
	public:
		virtual void OnExit() = 0;
		virtual void OnError() = 0;
		virtual void OnCopyFile() = 0;
		virtual void OnCreateDirectory() = 0;
		virtual void OnCreateFile() = 0;
		virtual void OnCreateHardLink() = 0;
		virtual void OnCreateProcess() = 0;
		virtual void OnDeleteFile() = 0;
		virtual void OnGetEnvironmentVariable() = 0;
		virtual void OnGetFileAttributes() = 0;
		virtual void OnLoadLibrary() = 0;
		virtual void OnMoveFile() = 0;
		virtual void OnOpenFile() = 0;
	};
}
