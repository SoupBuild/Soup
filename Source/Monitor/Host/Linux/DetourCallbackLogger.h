#pragma once 
#include "IDetourCallback.h"

namespace Monitor::Linux
{
	class DetourCallbackLogger : public IDetourCallback
	{
	public:
		DetourCallbackLogger(std::ostream& stream) :
			m_stream(stream)
		{
		}

		virtual void OnInitialize() override final
		{
			m_stream << "Initialize: " << std::endl;
		}

		virtual void OnShutdown() override final
		{
			m_stream << "Shutdown: " << std::endl;
		}

		virtual void OnError(std::string_view message) override final
		{
			m_stream << "Error: " << message << std::endl;
		}

		// FileApi
		virtual void OnOpen() override final
		{
			m_stream << "open: " << std::endl;
		}

	private:
		std::ostream& m_stream;
	};
}
