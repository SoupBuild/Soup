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
		void OnOpen(std::string_view path) override final
		{
			_callback1->OnOpen(path);
			_callback2->OnOpen(path);
		}

		void OnFOpen(std::string_view pathname, std::string_view mode) override final
		{
			_callback1->OnFOpen(pathname, mode);
			_callback2->OnFOpen(pathname, mode);
		}

		void OnFDOpen(int32_t fd, std::string_view mode) override final
		{
			_callback1->OnFDOpen(fd, mode);
			_callback2->OnFDOpen(fd, mode);
		}

		void OnFReopen(std::string_view pathname, std::string_view mode) override final
		{
			_callback1->OnFReopen(pathname, mode);
			_callback2->OnFReopen(pathname, mode);
		}

		void OnMkdir(std::string_view path, std::string_view mode) override final
		{
			_callback1->OnMkdir(path, mode);
			_callback2->OnMkdir(path, mode);
		}

	private:
		std::shared_ptr<IDetourCallback> _callback1;
		std::shared_ptr<IDetourCallback> _callback2;
	};
}
