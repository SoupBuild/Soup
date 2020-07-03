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

		void OnCreateDirectory(const std::string_view directory) override final
		{
			std::cout<< "CreateDirectory: " << directory << std::endl;
		}

		void OnCreateFile(const std::string_view filename) override final
		{
			std::cout<< "CreateFile: " << filename << std::endl;
		}

		void OnCreateHardLink() override final
		{
			std::cout<< "CreateHardLink: " << std::endl;
		}

		void OnCreateProcess() override final
		{
			std::cout<< "CreateProcess: " << std::endl;
		}

		void OnDeleteFile(const std::string_view filename) override final
		{
			std::cout<< "DeleteFile: " << filename << std::endl;
		}

		void OnGetEnvironmentVariable() override final
		{
			std::cout<< "GetEnvironmentVariable: " << std::endl;
		}

		void OnGetFileAttributes(const std::string_view filename) override final
		{
			std::cout<< "GetFileAttributes: " << filename << std::endl;
		}

		void OnLoadLibrary() override final
		{
			std::cout<< "LoadLibrary: " << std::endl;
		}

		void OnMoveFile() override final
		{
			std::cout<< "MoveFile: " << std::endl;
		}

		void OnOpenFile(const std::string_view filename) override final
		{
			std::cout<< "OpenFile: " << filename << std::endl;
		}
	};
}
