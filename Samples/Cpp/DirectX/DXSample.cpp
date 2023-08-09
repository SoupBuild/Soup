module;

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include <string>

export module Sample.DirectX:DXSample;
import :DXSampleHelper;

using namespace Microsoft::WRL;

export class DXSample
{
public:
	DXSample(UINT width, UINT height, std::wstring name) :
		m_width(width),
		m_height(height),
		m_title(name),
		m_useWarpDevice(false)
	{
		WCHAR assetsPath[512];
		GetAssetsPath(assetsPath, _countof(assetsPath));
		m_assetsPath = assetsPath;

		m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	}

	virtual ~DXSample()
	{
	}

	virtual void OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;

	// Samples override the event handlers to handle specific messages.
	virtual void OnKeyDown(UINT8 /*key*/) {}
	virtual void OnKeyUp(UINT8 /*key*/) {}

	// Accessors
	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
	LPCWSTR GetTitle() const { return m_title.c_str(); }

	// Helper function for parsing any supplied command line args.
	void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc)
	{
		for (int i = 1; i < argc; ++i)
		{
			if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 || 
				_wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
			{
				m_useWarpDevice = true;
				m_title = m_title + L" (WARP)";
			}
		}
	}

protected:
	// Helper function for resolving the full path of assets.
	std::wstring GetAssetFullPath(LPCWSTR assetName)
	{
		return m_assetsPath + assetName;
	}

	// Helper function for acquiring the first available hardware adapter that supports Direct3D 12.
	// If no such adapter can be found, *ppAdapter will be set to nullptr.
	void GetHardwareAdapter(
		_In_ IDXGIFactory1* pFactory,
		_Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter,
		bool requestHighPerformanceAdapter = false)
	{
		*ppAdapter = nullptr;

		ComPtr<IDXGIAdapter1> adapter;

		ComPtr<IDXGIFactory6> factory6;
		if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
		{
			for (
				UINT adapterIndex = 0;
				SUCCEEDED(factory6->EnumAdapterByGpuPreference(
					adapterIndex,
					requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
					IID_PPV_ARGS(&adapter)));
				++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the Basic Render Driver adapter.
					// If you want a software adapter, pass in "/warp" on the command line.
					continue;
				}

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
				{
					break;
				}
			}
		}

		if(adapter.Get() == nullptr)
		{
			for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1(&desc);

				if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				{
					// Don't select the Basic Render Driver adapter.
					// If you want a software adapter, pass in "/warp" on the command line.
					continue;
				}

				// Check to see whether the adapter supports Direct3D 12, but don't create the
				// actual device yet.
				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
				{
					break;
				}
			}
		}
		
		*ppAdapter = adapter.Detach();
	}


	// Viewport dimensions.
	UINT m_width;
	UINT m_height;
	float m_aspectRatio;

	// Adapter info.
	bool m_useWarpDevice;

private:
	// Root assets path.
	std::wstring m_assetsPath;

	// Window title.
	std::wstring m_title;
};
