#include "SteinsPCH.h"
#include "D3D11GraphicsDevice.h"

namespace Steins
{
	namespace
	{
		std::string GetVendor(int _vendorCode)
		{
			switch (_vendorCode)
			{
			case 0x10DE: return "NVIDIA Corporation";
			case 0x1002: return "AMD Inc.";
			case 0x8086: return "Intel";
			case 0x1414: return "Microsoft";
			}
			STEINS_CORE_ERROR("Not a valid VendorID");
			return "";
		}
	} //anonymous namespace

	D3D11GraphicsDevice::D3D11GraphicsDevice(SteinsWindow* _window)
		:GraphicsDevice(_window)
	{ 
		Init();
	}

	void D3D11GraphicsDevice::Init()
	{
		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;
		HRESULT hr = D3D11CreateDevice(
			NULL,
			driverType,
			NULL,
			debugLayerEnabled ? D3D11_CREATE_DEVICE_DEBUG : D3D11_CREATE_DEVICE_SINGLETHREADED,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel,
			commandList.GetAddressOf());

		if (FAILED(hr))
		{
			STEINS_CORE_ERROR("Failed to Create m_D3DDevice!");
		}

		device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter));
		dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

		//Here is For Get DX11 Version
		DXGI_ADAPTER_DESC adapterDescription; // Vendor
		SecureZeroMemory(&adapterDescription, sizeof(DXGI_ADAPTER_DESC));
		char videoCardDescription[128]; // Renderer
		LARGE_INTEGER driverVersion; // Version

		// Vendor
		dxgiAdapter->GetDesc(&adapterDescription);
		dxgiAdapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &driverVersion);

		// Renderer
		wcstombs_s(NULL, videoCardDescription, 128, adapterDescription.Description, 128);

		// Version
		std::string major, minor, release, build;
		major = std::to_string(HIWORD(driverVersion.HighPart));
		minor = std::to_string(LOWORD(driverVersion.HighPart));
		release = std::to_string(HIWORD(driverVersion.LowPart));
		build = std::to_string(LOWORD(driverVersion.LowPart));

		std::string version;
		version = major + "." + minor + "." + release + "." + build;

		//dxgiFactory->Release();
		//dxgiAdapter->Release();
		//dxgiDevice->Release();

		//Set DX11 DebugMode
		if (debugLayerEnabled)
		{
			device->QueryInterface(IID_PPV_ARGS(&debugLayer));
			debugLayer->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
			ID3D11InfoQueue* infoQueue;
			device->QueryInterface(IID_PPV_ARGS(&infoQueue));
			D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
			D3D11_INFO_QUEUE_FILTER filter;
			memset(&filter, 0, sizeof(filter));
			filter.DenyList.NumIDs = 1;
			filter.DenyList.pIDList = hide;
			infoQueue->AddStorageFilterEntries(&filter);
		}

		STEINS_CORE_INFO("DirectX11 Info:");
		STEINS_CORE_INFO("  Vendor: {0}", GetVendor(adapterDescription.VendorId));
		STEINS_CORE_INFO("  Renderer: {0}", videoCardDescription);
		STEINS_CORE_INFO("  Version: {0}", version);

		//DXGI_SWAP_CHAIN_DESC swapDesc = {};
		//swapDesc.BufferCount = 1;
		//swapDesc.BufferDesc.Width = 800;
		//swapDesc.BufferDesc.Height = 600;
		//swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		//swapDesc.OutputWindow = hwnd;
		//swapDesc.SampleDesc.Count = 1;
		//swapDesc.Windowed = TRUE;

		//D3D11CreateDeviceAndSwapChain(
		//	nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		//	nullptr, 0, D3D11_SDK_VERSION,
		//	&swapDesc, &swapChain, &device, nullptr, &context);
	}

	void D3D11GraphicsDevice::Shutdown()
	{
	}

	void D3D11GraphicsDevice::Render()
	{
	}

	void D3D11GraphicsDevice::SwapBuffers()
	{
	}
}