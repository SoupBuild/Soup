#pragma once

namespace Soup::Core
{
	class SystemAccessTracker : public Monitor::IMonitorCallback
	{
	private:
		int _activeProcessCount;
		std::set<std::string> _input;
		std::set<std::string> _inputMissing;
		std::set<std::string> _output;
		std::set<std::string> _deleteOnClose;

	public:
		SystemAccessTracker() :
			_activeProcessCount(0),
			_input(),
			_output(),
			_deleteOnClose()
		{
		}

		void VerifyResult()
		{
			if (_activeProcessCount != 0)
			{
				Log::Warning("A child process is still running in the background");
			}

			// Cleanup on close delete files
			for (auto& file : _deleteOnClose)
			{
				auto extractOutputResult = _output.extract(file);
				auto extractInputResult = _input.extract(file);
			}
		}

		std::set<std::string> GetInput()
		{
			return _input;
		}

		std::set<std::string> GetOutput()
		{
			return _output;
		}

		virtual void OnCreateProcess(std::string_view applicationName, bool wasDetoured) override final
		{
			if (wasDetoured)
				Log::Diag("SystemAccessTracker::OnCreateDetouredProcess - " + std::string(applicationName));
			else
				Log::Diag("SystemAccessTracker::OnCreateProcess - " + std::string(applicationName));
		}

		virtual void TouchFileRead(Path filePath, bool exists, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileReadBlocked: " + filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();
				ToUpper(value);

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileRead " + value);
				#endif

				if (exists)
				{
					_input.insert(value);
				}
				else
				{
					_inputMissing.insert(value);
				}
			}
		}

		virtual void TouchFileWrite(Path filePath, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileWriteBlocked: " + filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();
				ToUpper(value);

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileWrite " + value);
				#endif

				_output.insert(value);
			}
		}

		virtual void TouchFileDelete(Path filePath, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileDeleteBlocked: " + filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();
				ToUpper(value);

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileDelete " + value);
				#endif

				// If this was an output file extract it as it was a transient file
				// TODO: May want to track if we created the file
				auto extractOutputResult = _output.extract(value);
				auto extractInputResult = _input.extract(value);
			}
		}

		virtual void TouchFileDeleteOnClose(Path filePath) override final
		{
			auto value = filePath.ToString();
			ToUpper(value);

			#ifdef TRACE_SYSTEM_ACCESS
			Log::Diag("TouchFileDeleteOnClose " + value);
			#endif

			_deleteOnClose.insert(std::move(value));
		}

		virtual void SearchPath(std::string_view path, std::string_view filename) override final
		{
			Log::Warning("Search Path encountered: " + std::string(path) + " - " + std::string(filename));
		}

	private:
		void ToUpper(std::string& value)
		{
			std::transform(
				value.begin(),
				value.end(),
				value.begin(),
				[](unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); });
		}
	};
}
