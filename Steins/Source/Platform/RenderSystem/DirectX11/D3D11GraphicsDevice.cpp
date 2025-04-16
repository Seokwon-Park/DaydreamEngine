#include "SteinsPCH.h"
#include "D3D11GraphicsDevice.h"
#include "Platform/RenderSystem/DXHelper.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"


namespace Steins
{
	D3D11GraphicsDevice::D3D11GraphicsDevice()
	{
	}

	D3D11GraphicsDevice::~D3D11GraphicsDevice()
	{
#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D11Debug> DXGIDebug;

		if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&DXGIDebug))))
		{
			DXGIDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
			DXGIDebug = nullptr;
		}
#endif
		/*dxgiAdapter->Release();
		dxgiDevice->Release();
		dxgiFactory->Release();*/
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

		////Set DX11 DebugMode
		//if (debugLayerEnabled)
		//{
		//	device->QueryInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
		//	debugLayer->ReportLiveDeviceObjects(D3D11_RLDO_IGNORE_INTERNAL);
		//	ID3D11InfoQueue* infoQueue;
		//	device->QueryInterface(IID_PPV_ARGS(&infoQueue));
		//	D3D11_MESSAGE_ID hide[] = { D3D11_MESSAGE_ID_DEVICE_DRAW_SAMPLER_NOT_SET };
		//	D3D11_INFO_QUEUE_FILTER filter;
		//	memset(&filter, 0, sizeof(filter));
		//	filter.DenyList.NumIDs = 1;
		//	filter.DenyList.pIDList = hide;
		//	infoQueue->AddStorageFilterEntries(&filter);
		//}

		STEINS_CORE_INFO("DirectX11 Info:");
		STEINS_CORE_INFO("  Vendor: {0}", DXHelper::GetVendor(adapterDescription.VendorId));
		STEINS_CORE_INFO("  Renderer: {0}", videoCardDescription);
		STEINS_CORE_INFO("  Version: {0}", version);


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

	void D3D11GraphicsDevice::ClearRenderTargetViews(Color _clearColor)
	{
		//deviceContext->ClearRenderTargetView(rtv.Get(), _clearColor.color);
		//BindRenderTargets();
	}
	void D3D11GraphicsDevice::BindRenderTargets()
	{
		deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);
	}
}