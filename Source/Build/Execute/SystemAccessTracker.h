#pragma once 

namespace Soup::Build
{
	class SystemAccessTracker : public Monitor::IDetourCallback
	{
	public:
		SystemAccessTracker(std::ostream& stream) :
			m_stream(stream)
		{
		}

		void OnExit() override final
		{
			// m_stream << "Exit: " << std::endl;
		}

		void OnError() override final
		{
			// m_stream << "Error: " << std::endl;
		}

		void OnCopyFile() override final
		{
			// m_stream << "CopyFile: " << std::endl;
		}

		void OnCreateDirectory(const std::string_view directory) override final
		{
			// m_stream << "CreateDirectory: " << directory << std::endl;
		}

		void OnCreateFile(const std::string_view filename) override final
		{
			// m_stream << "CreateFile: " << filename << std::endl;
		}

		void OnCreateHardLink() override final
		{
			// m_stream << "CreateHardLink: " << std::endl;
		}

		void OnCreateProcess() override final
		{
			// m_stream << "CreateProcess: " << std::endl;
		}

		void OnDeleteFile(const std::string_view filename) override final
		{
			// m_stream << "DeleteFile: " << filename << std::endl;
		}

		void OnGetEnvironmentVariable() override final
		{
			// m_stream << "GetEnvironmentVariable: " << std::endl;
		}

		void OnGetFileAttributes(const std::string_view filename) override final
		{
			// m_stream << "GetFileAttributes: " << filename << std::endl;
		}

		void OnLoadLibrary() override final
		{
			// m_stream << "LoadLibrary: " << std::endl;
		}

		void OnMoveFile() override final
		{
			// m_stream << "MoveFile: " << std::endl;
		}

		void OnOpenFile(const std::string_view filename) override final
		{
			// m_stream << "OpenFile: " << filename << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
