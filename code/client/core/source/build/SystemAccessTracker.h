#pragma once

namespace Soup::Core
{
	class SystemAccessTracker : public Monitor::ISystemAccessMonitor
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
				Log::Diag("SystemAccessTracker::OnCreateDetouredProcess - {}", applicationName);
			else
				Log::Diag("SystemAccessTracker::OnCreateProcess - {}", applicationName);
		}

		virtual void TouchFileRead(Path filePath, bool exists, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileReadBlocked: {}", filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileRead {}", value);
				#endif

				if (exists)
				{
					_input.insert(std::move(value));
				}
				else
				{
					_inputMissing.insert(std::move(value));
				}
			}
		}

		virtual void TouchFileWrite(Path filePath, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileWriteBlocked: {}", filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileWrite {}", value);
				#endif

				_output.insert(std::move(value));
			}
		}

		virtual void TouchFileDelete(Path filePath, bool wasBlocked) override final
		{
			if (wasBlocked)
			{
				// TODO: Warning
				Log::Info("FileDeleteBlocked: {}", filePath.ToString());
			}
			else
			{
				auto value = filePath.ToString();

				#ifdef TRACE_SYSTEM_ACCESS
				Log::Diag("TouchFileDelete {}", value);
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

			#ifdef TRACE_SYSTEM_ACCESS
			Log::Diag("TouchFileDeleteOnClose {}", value);
			#endif

			_deleteOnClose.insert(std::move(value));
		}

		virtual void SearchPath(std::string_view path, std::string_view filename) override final
		{
			Log::Warning("Search Path encountered: {} - {}", path, filename);
		}
	};
}
