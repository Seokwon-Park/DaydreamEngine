#include "DaydreamPCH.h"
#include "D3D11RenderDevice.h"
#include "D3D11RenderContext.h"
#include "D3D11Buffer.h"
#include "D3D11Swapchain.h"
#include "D3D11Shader.h"
#include "D3D11VertexShader.h"
#include "D3D11Framebuffer.h"
#include "D3D11PipelineState.h"
#include "D3D11ImGuiRenderer.h"
#include "D3D11Texture.h"
#include "D3D11TextureCube.h"
#include "D3D11Sampler.h"
#include "D3D11Material.h"
#include "D3D11RenderPass.h"

#include "Daydream/Graphics/Utility/GraphicsUtility.h"

namespace Daydream
{
	D3D11RenderDevice::D3D11RenderDevice()
	{
		API = RendererAPIType::DirectX11;
	}

	D3D11RenderDevice::~D3D11RenderDevice()
	{
		//#if defined(DEBUG) || defined(_DEBUG)
		//		Microsoft::WRL::ComPtr<ID3D11Debug> DXGIDebug;
		//
		//		if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&DXGIDebug))))
		//		{
		//			DXGIDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
		//			DXGIDebug = nullptr;
		//		}
		//#endif
				/*dxgiAdapter->Release();
				dxgiDevice->Release();
				dxgiFactory->Release();*/
				//device.Reset();
	}

	void D3D11RenderDevice::Init()
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
			DAYDREAM_CORE_ERROR("Failed to Create m_D3DDevice!");
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

		DAYDREAM_CORE_INFO("DirectX11 Info:");
		DAYDREAM_CORE_INFO("  Vendor: {0}", GraphicsUtility::GetVendor(adapterDescription.VendorId));
		DAYDREAM_CORE_INFO("  Renderer: {0}", videoCardDescription);
		DAYDREAM_CORE_INFO("  Version: {0}", version);
	}

	void D3D11RenderDevice::Shutdown()
	{
	}

	void D3D11RenderDevice::Render()
	{
	}


	Shared<RenderContext> D3D11RenderDevice::CreateContext()
	{
		return MakeShared<D3D11GraphicsContext>(this);
	}

	Shared<VertexBuffer> D3D11RenderDevice::CreateDynamicVertexBuffer(UInt32 _size, UInt32 _stride, UInt32 _initialDataSize, const void* _initialData)
	{
		Shared<D3D11VertexBuffer> buffer = MakeShared<D3D11VertexBuffer>(this, BufferUsage::Dynamic, _size, _stride, nullptr);
		if (_initialData != nullptr)
		{
			buffer->SetData(_initialData, _initialDataSize);
		}
		return buffer;
	}

	Shared<VertexBuffer> D3D11RenderDevice::CreateStaticVertexBuffer(UInt32 _size, UInt32 _stride, const void* _initialData)
	{
		//return MakeShared<D3D11VertexBuffer>();
		return MakeShared<D3D11VertexBuffer>(this, BufferUsage::Static, _size, _stride, _initialData);
	}

	Shared<IndexBuffer> Daydream::D3D11RenderDevice::CreateIndexBuffer(const UInt32* _indices, UInt32 _count)
	{
		return MakeShared<D3D11IndexBuffer>(this, _indices, _count);
	}

	Shared<RenderPass> D3D11RenderDevice::CreateRenderPass(const RenderPassDesc& _desc)
	{
		return MakeShared<D3D11RenderPass>(this, _desc);
	}

	Shared<Framebuffer> D3D11RenderDevice::CreateFramebuffer(Shared<RenderPass> _renderPass, const FramebufferDesc& _desc)
	{
		return _renderPass->CreateFramebuffer(_desc);
	}

	Shared<PipelineState> D3D11RenderDevice::CreatePipelineState(const PipelineStateDesc& _desc)
	{
		return MakeShared<D3D11PipelineState>(this, _desc);
	}

	Shared<Shader> D3D11RenderDevice::CreateShader(const std::string& _src, const ShaderType& _type, ShaderLoadMode _mode)
	{
		switch (_type)
		{
		case ShaderType::None:  return nullptr;
		case ShaderType::Vertex: return MakeShared<D3D11VertexShader>(this, _src, _mode);
		case ShaderType::Hull: return nullptr;
		case ShaderType::Domain: return nullptr;
		case ShaderType::Geometry: return nullptr;
		case ShaderType::Pixel: return MakeShared<D3D11PixelShader>(this, _src, _mode);
		case ShaderType::Compute: return nullptr;
		default:
			break;
		}
		return Shared<Shader>();
	}

	Shared<Swapchain> D3D11RenderDevice::CreateSwapchain(DaydreamWindow* _window, const SwapchainDesc& _desc)
	{
		return MakeShared<D3D11Swapchain>(this, _window, _desc);
	}

	Shared<Texture2D> D3D11RenderDevice::CreateTexture2D(const void* _imageData, const TextureDesc& _desc)
	{
		//return MakeShared<D3D11Texture2D>(this, _path, _desc);
		return MakeShared<D3D11Texture2D>(this, _desc, _imageData);
	}

	Shared<TextureCube> D3D11RenderDevice::CreateTextureCube(Array<const void*>& _imageDatas, const TextureDesc& _desc)
	{
		return MakeShared<D3D11TextureCube>(this, _desc, _imageDatas);
	}

	Shared<Sampler> D3D11RenderDevice::CreateSampler(const SamplerDesc& _desc)
	{
		return MakeShared<D3D11Sampler>(this, _desc);
	}

	Unique<ImGuiRenderer> D3D11RenderDevice::CreateImGuiRenderer()
	{
		return MakeUnique<D3D11ImGuiRenderer>(this);
	}

	Shared<ConstantBuffer> D3D11RenderDevice::CreateConstantBuffer(UInt32 _size)
	{
		return MakeShared<D3D11ConstantBuffer>(this, _size);
	}

	Shared<Material> D3D11RenderDevice::CreateMaterial(Shared<PipelineState> _pipeline)
	{
		return _pipeline->CreateMaterial();
	}
}