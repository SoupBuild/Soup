#pragma once 

namespace Soup::Build
{
	class SystemAccessTracker : public Monitor::IDetourCallback
	{
	public:
		SystemAccessTracker() :
			m_input(),
			m_output()
		{
		}

		void OnExit() override final
		{
		}

		void OnError() override final
		{
			Log::Error("SystemAccessTracker Error");
		}

		void OnCopyFile(
			const std::string_view source,
			const std::string_view destination) override final
		{
			auto normalizeSource = Path(source).ToString();
			auto normalizeDestination = Path(destination).ToString();
			Log::Diag("OnCopyFile: " + normalizeSource + " -> " + normalizeDestination);
			m_input.insert(normalizeSource);
			m_output.insert(normalizeDestination);
		}

		void OnCreateDirectory(
			const std::string_view directory) override final
		{
			auto normalizeDirectory = Path(directory).ToString();
			Log::Diag("OnCreateDirectory: " + normalizeDirectory);
			m_output.insert(normalizeDirectory);
		}

		void OnCreateFile(const std::string_view filename) override final
		{
			// throw std::runtime_error("Not implemented");
		}

		void OnCreateHardLink() override final
		{
			// throw std::runtime_error("Not implemented");
		}

		void OnCreateProcess() override final
		{
			// throw std::runtime_error("Not implemented");
		}

		void OnDeleteFile(const std::string_view filename) override final
		{
			// throw std::runtime_error("Not implemented");
		}

		void OnGetEnvironmentVariable() override final
		{
		}

		void OnGetFileAttributes(const std::string_view filename) override final
		{
			auto normalizeFilename = Path(filename).ToString();
			Log::Diag("OnGetFileAttributes: " + normalizeFilename);
			m_input.insert(normalizeFilename);
		}

		void OnLoadLibrary() override final
		{
			// throw std::runtime_error("Not implemented");
		}

		void OnMoveFile(
			const std::string_view source,
			const std::string_view destination) override final
		{
			auto normalizeSource = Path(source).ToString();
			auto normalizeDestination = Path(destination).ToString();
			Log::Diag("OnMoveFile: " + normalizeSource + " -> " + normalizeDestination);
			m_input.insert(normalizeSource);
			m_output.insert(normalizeDestination);
		}

		void OnOpenFile(const std::string_view filename) override final
		{
			// throw std::runtime_error("Not implemented");
		}

	private:
		std::set<std::string> m_input;
		std::set<std::string> m_output;
	};
}
