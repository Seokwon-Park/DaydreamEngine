#include "SteinsPCH.h"
#include "D3D11GraphicsDevice.h"
#include "Steins/Core/Window.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"


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
	}

	D3D11GraphicsDevice::~D3D11GraphicsDevice()
	{
		//dxgiAdapter->Release();
		//dxgiAdapter = nullptr;
		//dxgiDevice->Release();
		//dxgiDevice = nullptr;
		//dxgiFactory->Release();
		//dxgiFactory = nullptr;
		//device.Reset();
	}

	void D3D11GraphicsDevice::Init()
	{
		UInt32 creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
		// If the project is in a debug build, enable debugging via SDK Layers.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};


		D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			driverType,
			nullptr,
			creationFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel,
			deviceContext.GetAddressOf());

		if (FAILED(hr))
		{
			STEINS_CORE_ERROR("Failed to Create m_D3DDevice!");
		}

		device->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()));
		dxgiDevice->GetParent(IID_PPV_ARGS(dxgiAdapter.GetAddressOf()));
		dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

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
			device->QueryInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
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

		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferCount = 2;
		swapDesc.BufferDesc.Width = windowHandle->GetWidth();
		swapDesc.BufferDesc.Height = windowHandle->GetHeight();
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.OutputWindow = glfwGetWin32Window((GLFWwindow*)windowHandle->GetNativeWindow());
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapDesc.Windowed = TRUE;

		dxgiFactory->CreateSwapChain(device.Get(), &swapDesc, swapChain.GetAddressOf());

		ComPtr<ID3D11Texture2D> backBuffer;
		swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));

		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtv.GetAddressOf());
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
		swapChain->Present(1, 0);
	}

	void D3D11GraphicsDevice::Clear()
	{
		//deviceContext->ClearRenderTargetView();
	}

	void D3D11GraphicsDevice::DrawIndexed(UInt32 _indexCount, UInt32 _startIndex, UInt32 _baseVertex)
	{
		deviceContext->DrawIndexed(_indexCount, _startIndex, _baseVertex);
	}
	void D3D11GraphicsDevice::ClearRenderTargetViews(Color _clearColor)
	{
		deviceContext->ClearRenderTargetView(rtv.Get(), _clearColor.color);
		BindRenderTargets();
	}
	void D3D11GraphicsDevice::BindRenderTargets()
	{
		deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	}
}