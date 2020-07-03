#pragma once 

namespace Monitor
{
	class DetourCallbackLogger : public IDetourCallback
	{
	public:
		void OnExit() override final
		{
			std::cout<< "Exit: " << std::endl;
		}

		void OnError() override final
		{
			std::cout<< "Error: " << std::endl;
		}

		void OnCopyFile() override final
		{
			std::cout<< "CopyFile: " << std::endl;
		}

		void OnCreateDirectory() override final
		{
			std::cout<< "CreateDirectory: " << std::endl;
		}

		void OnCreateFile() override final
		{
			std::cout<< "CreateFile: " << std::endl;
		}

		void OnCreateHardLink() override final
		{
			std::cout<< "CreateHardLink: " << std::endl;
		}

		void OnCreateProcess() override final
		{
			std::cout<< "CreateProcess: " << std::endl;
		}

		void OnDeleteFile() override final
		{
			std::cout<< "DeleteFile: " << std::endl;
		}

		void OnGetEnvironmentVariable() override final
		{
			std::cout<< "GetEnvironmentVariable: " << std::endl;
		}

		void OnGetFileAttributes() override final
		{
			std::cout<< "GetFileAttributes: " << std::endl;
		}

		void OnLoadLibrary() override final
		{
			std::cout<< "LoadLibrary: " << std::endl;
		}

		void OnMoveFile() override final
		{
			std::cout<< "MoveFile: " << std::endl;
		}

		void OnOpenFile() override final
		{
			std::cout<< "OpenFile: " << std::endl;
		}

	};
}
