#pragma once 

namespace Monitor::Linux
{
	class DetourForkCallback : public IDetourCallback
	{
	public:
		DetourForkCallback(std::shared_ptr<IDetourCallback> callback1, std::shared_ptr<IDetourCallback> callback2) :
			_callback1(std::move(callback1)),
			_callback2(std::move(callback2))
		{
		}

		void OnInitialize() override final
		{
			_callback1->OnInitialize();
			_callback2->OnInitialize();
		}

		void OnShutdown() override final
		{
			_callback1->OnShutdown();
			_callback2->OnShutdown();
		}

		void OnError(std::string_view message) override final
		{
			_callback1->OnError(message);
			_callback2->OnError(message);
		}

		// FileApi
		void OnOpen() override final
		{
			_callback1->OnOpen();
			_callback2->OnOpen();
		}

	private:
		std::shared_ptr<IDetourCallback> _callback1;
		std::shared_ptr<IDetourCallback> _callback2;
	};
}
