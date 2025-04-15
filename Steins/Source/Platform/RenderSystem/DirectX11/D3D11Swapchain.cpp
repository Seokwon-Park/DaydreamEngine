#include "SteinsPCH.h"
#include "D3D11Swapchain.h"

#include "Steins/Core/Window.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace Steins
{
	DXGI_FORMAT RenderFormatToDXGIFormat(RenderFormat _format)
	{
		switch (_format)
		{
		case Steins::RenderFormat::UNKNOWN:                             return DXGI_FORMAT_UNKNOWN;
		case Steins::RenderFormat::R32G32B32A32_TYPELESS:               return DXGI_FORMAT_R32G32B32A32_TYPELESS;
		case Steins::RenderFormat::R32G32B32A32_FLOAT:                  return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case Steins::RenderFormat::R32G32B32A32_UINT:                   return DXGI_FORMAT_R32G32B32A32_UINT;
		case Steins::RenderFormat::R32G32B32A32_SINT:                   return DXGI_FORMAT_R32G32B32A32_SINT;
		case Steins::RenderFormat::R32G32B32_TYPELESS:                  return DXGI_FORMAT_R32G32B32_TYPELESS;
		case Steins::RenderFormat::R32G32B32_FLOAT:                     return DXGI_FORMAT_R32G32B32_FLOAT;
		case Steins::RenderFormat::R32G32B32_UINT:                      return DXGI_FORMAT_R32G32B32_UINT;
		case Steins::RenderFormat::R32G32B32_SINT:                      return DXGI_FORMAT_R32G32B32_SINT;
		case Steins::RenderFormat::R16G16B16A16_TYPELESS:               return DXGI_FORMAT_R16G16B16A16_TYPELESS;
		case Steins::RenderFormat::R16G16B16A16_FLOAT:                  return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case Steins::RenderFormat::R16G16B16A16_UNORM:                  return DXGI_FORMAT_R16G16B16A16_UNORM;
		case Steins::RenderFormat::R16G16B16A16_UINT:                   return DXGI_FORMAT_R16G16B16A16_UINT;
		case Steins::RenderFormat::R16G16B16A16_SNORM:                  return DXGI_FORMAT_R16G16B16A16_SNORM;
		case Steins::RenderFormat::R16G16B16A16_SINT:                   return DXGI_FORMAT_R16G16B16A16_SINT;
		case Steins::RenderFormat::R32G32_TYPELESS:                     return DXGI_FORMAT_R32G32_TYPELESS;
		case Steins::RenderFormat::R32G32_FLOAT:                        return DXGI_FORMAT_R32G32_FLOAT;
		case Steins::RenderFormat::R32G32_UINT:                         return DXGI_FORMAT_R32G32_UINT;
		case Steins::RenderFormat::R32G32_SINT:                         return DXGI_FORMAT_R32G32_SINT;
		case Steins::RenderFormat::R32G8X24_TYPELESS:                   return DXGI_FORMAT_R32G8X24_TYPELESS;
		case Steins::RenderFormat::D32_FLOAT_S8X24_UINT:                return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case Steins::RenderFormat::R32_FLOAT_X8X24_TYPELESS:            return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
		case Steins::RenderFormat::X32_TYPELESS_G8X24_UINT:             return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;
		case Steins::RenderFormat::R10G10B10A2_TYPELESS:                return DXGI_FORMAT_R10G10B10A2_TYPELESS;
		case Steins::RenderFormat::R10G10B10A2_UNORM:                   return DXGI_FORMAT_R10G10B10A2_UNORM;
		case Steins::RenderFormat::R10G10B10A2_UINT:                    return DXGI_FORMAT_R10G10B10A2_UINT;
		case Steins::RenderFormat::R11G11B10_FLOAT:                     return DXGI_FORMAT_R11G11B10_FLOAT;
		case Steins::RenderFormat::R8G8B8A8_TYPELESS:                   return DXGI_FORMAT_R8G8B8A8_TYPELESS;
		case Steins::RenderFormat::R8G8B8A8_UNORM:                      return DXGI_FORMAT_R8G8B8A8_UNORM;
		case Steins::RenderFormat::R8G8B8A8_UNORM_SRGB:                 return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case Steins::RenderFormat::R8G8B8A8_UINT:                       return DXGI_FORMAT_R8G8B8A8_UINT;
		case Steins::RenderFormat::R8G8B8A8_SNORM:                      return DXGI_FORMAT_R8G8B8A8_SNORM;
		case Steins::RenderFormat::R8G8B8A8_SINT:                       return DXGI_FORMAT_R8G8B8A8_SINT;
		case Steins::RenderFormat::R16G16_TYPELESS:                     return DXGI_FORMAT_R16G16_TYPELESS;
		case Steins::RenderFormat::R16G16_FLOAT:                        return DXGI_FORMAT_R16G16_FLOAT;
		case Steins::RenderFormat::R16G16_UNORM:                        return DXGI_FORMAT_R16G16_UNORM;
		case Steins::RenderFormat::R16G16_UINT:                         return DXGI_FORMAT_R16G16_UINT;
		case Steins::RenderFormat::R16G16_SNORM:                        return DXGI_FORMAT_R16G16_SNORM;
		case Steins::RenderFormat::R16G16_SINT:                         return DXGI_FORMAT_R16G16_SINT;
		case Steins::RenderFormat::R32_TYPELESS:                        return DXGI_FORMAT_R32_TYPELESS;
		case Steins::RenderFormat::D32_FLOAT:                           return DXGI_FORMAT_D32_FLOAT;
		case Steins::RenderFormat::R32_FLOAT:                           return DXGI_FORMAT_R32_FLOAT;
		case Steins::RenderFormat::R32_UINT:                            return DXGI_FORMAT_R32_UINT;
		case Steins::RenderFormat::R32_SINT:                            return DXGI_FORMAT_R32_SINT;
		case Steins::RenderFormat::R24G8_TYPELESS:                      return DXGI_FORMAT_R24G8_TYPELESS;
		case Steins::RenderFormat::D24_UNORM_S8_UINT:                   return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case Steins::RenderFormat::R24_UNORM_X8_TYPELESS:               return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		case Steins::RenderFormat::X24_TYPELESS_G8_UINT:                return DXGI_FORMAT_X24_TYPELESS_G8_UINT;
		case Steins::RenderFormat::R8G8_TYPELESS:                       return DXGI_FORMAT_R8G8_TYPELESS;
		case Steins::RenderFormat::R8G8_UNORM:                          return DXGI_FORMAT_R8G8_UNORM;
		case Steins::RenderFormat::R8G8_UINT:                           return DXGI_FORMAT_R8G8_UINT;
		case Steins::RenderFormat::R8G8_SNORM:                          return DXGI_FORMAT_R8G8_SNORM;
		case Steins::RenderFormat::R8G8_SINT:                           return DXGI_FORMAT_R8G8_SINT;
		case Steins::RenderFormat::R16_TYPELESS:                        return DXGI_FORMAT_R16_TYPELESS;
		case Steins::RenderFormat::R16_FLOAT:                           return DXGI_FORMAT_R16_FLOAT;
		case Steins::RenderFormat::D16_UNORM:                           return DXGI_FORMAT_D16_UNORM;
		case Steins::RenderFormat::R16_UNORM:                           return DXGI_FORMAT_R16_UNORM;
		case Steins::RenderFormat::R16_UINT:                            return DXGI_FORMAT_R16_UINT;
		case Steins::RenderFormat::R16_SNORM:                           return DXGI_FORMAT_R16_SNORM;
		case Steins::RenderFormat::R16_SINT:                            return DXGI_FORMAT_R16_SINT;
		case Steins::RenderFormat::R8_TYPELESS:                         return DXGI_FORMAT_R8_TYPELESS;
		case Steins::RenderFormat::R8_UNORM:                            return DXGI_FORMAT_R8_UNORM;
		case Steins::RenderFormat::R8_UINT:                             return DXGI_FORMAT_R8_UINT;
		case Steins::RenderFormat::R8_SNORM:                            return DXGI_FORMAT_R8_SNORM;
		case Steins::RenderFormat::R8_SINT:                             return DXGI_FORMAT_R8_SINT;
		case Steins::RenderFormat::A8_UNORM:                            return DXGI_FORMAT_A8_UNORM;
		case Steins::RenderFormat::R1_UNORM:                            return DXGI_FORMAT_R1_UNORM;
		case Steins::RenderFormat::R9G9B9E5_SHAREDEXP:                  return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		case Steins::RenderFormat::R8G8_B8G8_UNORM:                     return DXGI_FORMAT_R8G8_B8G8_UNORM;
		case Steins::RenderFormat::G8R8_G8B8_UNORM:                     return DXGI_FORMAT_G8R8_G8B8_UNORM;
		case Steins::RenderFormat::BC1_TYPELESS:                        return DXGI_FORMAT_BC1_TYPELESS;
		case Steins::RenderFormat::BC1_UNORM:                           return DXGI_FORMAT_BC1_UNORM;
		case Steins::RenderFormat::BC1_UNORM_SRGB:                      return DXGI_FORMAT_BC1_UNORM_SRGB;
		case Steins::RenderFormat::BC2_TYPELESS:                        return DXGI_FORMAT_BC2_TYPELESS;
		case Steins::RenderFormat::BC2_UNORM:                           return DXGI_FORMAT_BC2_UNORM;
		case Steins::RenderFormat::BC2_UNORM_SRGB:                      return DXGI_FORMAT_BC2_UNORM_SRGB;
		case Steins::RenderFormat::BC3_TYPELESS:                        return DXGI_FORMAT_BC3_TYPELESS;
		case Steins::RenderFormat::BC3_UNORM:                           return DXGI_FORMAT_BC3_UNORM;
		case Steins::RenderFormat::BC3_UNORM_SRGB:                      return DXGI_FORMAT_BC3_UNORM_SRGB;
		case Steins::RenderFormat::BC4_TYPELESS:                        return DXGI_FORMAT_BC4_TYPELESS;
		case Steins::RenderFormat::BC4_UNORM:                           return DXGI_FORMAT_BC4_UNORM;
		case Steins::RenderFormat::BC4_SNORM:                           return DXGI_FORMAT_BC4_SNORM;
		case Steins::RenderFormat::BC5_TYPELESS:                        return DXGI_FORMAT_BC5_TYPELESS;
		case Steins::RenderFormat::BC5_UNORM:                           return DXGI_FORMAT_BC5_UNORM;
		case Steins::RenderFormat::BC5_SNORM:                           return DXGI_FORMAT_BC5_SNORM;
		case Steins::RenderFormat::B5G6R5_UNORM:                        return DXGI_FORMAT_B5G6R5_UNORM;
		case Steins::RenderFormat::B5G5R5A1_UNORM:                      return DXGI_FORMAT_B5G5R5A1_UNORM;
		case Steins::RenderFormat::B8G8R8A8_UNORM:                      return DXGI_FORMAT_B8G8R8A8_UNORM;
		case Steins::RenderFormat::B8G8R8X8_UNORM:                      return DXGI_FORMAT_B8G8R8X8_UNORM;
		case Steins::RenderFormat::R10G10B10_XR_BIAS_A2_UNORM:          return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
		case Steins::RenderFormat::B8G8R8A8_TYPELESS:                   return DXGI_FORMAT_B8G8R8A8_TYPELESS;
		case Steins::RenderFormat::B8G8R8A8_UNORM_SRGB:                 return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case Steins::RenderFormat::B8G8R8X8_TYPELESS:                   return DXGI_FORMAT_B8G8R8X8_TYPELESS;
		case Steins::RenderFormat::B8G8R8X8_UNORM_SRGB:                 return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
		case Steins::RenderFormat::BC6H_TYPELESS:                       return DXGI_FORMAT_BC6H_TYPELESS;
		case Steins::RenderFormat::BC6H_UF16:                           return DXGI_FORMAT_BC6H_UF16;
		case Steins::RenderFormat::BC6H_SF16:                           return DXGI_FORMAT_BC6H_SF16;
		case Steins::RenderFormat::BC7_TYPELESS:                        return DXGI_FORMAT_BC7_TYPELESS;
		case Steins::RenderFormat::BC7_UNORM:                           return DXGI_FORMAT_BC7_UNORM;
		case Steins::RenderFormat::BC7_UNORM_SRGB:                      return DXGI_FORMAT_BC7_UNORM_SRGB;
		case Steins::RenderFormat::AYUV:                                return DXGI_FORMAT_AYUV;
		case Steins::RenderFormat::Y410:                                return DXGI_FORMAT_Y410;
		case Steins::RenderFormat::Y416:                                return DXGI_FORMAT_Y416;
		case Steins::RenderFormat::NV12:                                return DXGI_FORMAT_NV12;
		case Steins::RenderFormat::P010:                                return DXGI_FORMAT_P010;
		case Steins::RenderFormat::P016:                                return DXGI_FORMAT_P016;
		case Steins::RenderFormat::YUY2:                                return DXGI_FORMAT_YUY2;
		case Steins::RenderFormat::Y210:                                return DXGI_FORMAT_Y210;
		case Steins::RenderFormat::Y216:                                return DXGI_FORMAT_Y216;
		case Steins::RenderFormat::NV11:                                return DXGI_FORMAT_NV11;
		case Steins::RenderFormat::AI44:                                return DXGI_FORMAT_AI44;
		case Steins::RenderFormat::IA44:                                return DXGI_FORMAT_IA44;
		case Steins::RenderFormat::P8:                                  return DXGI_FORMAT_P8;
		case Steins::RenderFormat::A8P8:                                return DXGI_FORMAT_A8P8;
		case Steins::RenderFormat::B4G4R4A4_UNORM:                      return DXGI_FORMAT_B4G4R4A4_UNORM;
		case Steins::RenderFormat::P208:                                return DXGI_FORMAT_P208;
		case Steins::RenderFormat::V208:                                return DXGI_FORMAT_V208;
		case Steins::RenderFormat::V408:                                return DXGI_FORMAT_V408;
		case Steins::RenderFormat::SAMPLER_FEEDBACK_MIN_MIP_OPAQUE:    return DXGI_FORMAT_SAMPLER_FEEDBACK_MIN_MIP_OPAQUE;
		case Steins::RenderFormat::SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE:
			return DXGI_FORMAT_SAMPLER_FEEDBACK_MIP_REGION_USED_OPAQUE;
		case Steins::RenderFormat::A4B4G4R4_UNORM:                      return DXGI_FORMAT_A4B4G4R4_UNORM;
		default:                                                        return DXGI_FORMAT_UNKNOWN;
		}
	}

	D3D11Swapchain::D3D11Swapchain(GraphicsDevice* _device, SwapchainDesc* _desc, SteinsWindow* _window)
	{
		device = Cast<D3D11GraphicsDevice>(_device);
		desc = *_desc;
		STEINS_CORE_ASSERT(device, "Device is not D3D11GraphicsDevice!");

		DXGI_SAMPLE_DESC sampleDesc = {};
		sampleDesc.Count = 1;    // 샘플 수 (1이면 MSAA 비활성화)
		sampleDesc.Quality = 0;  // 품질 레벨 (0이면 기본값)

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = _desc->width;
		bufferDesc.Height = _desc->height;
		//bufferDesc.RefreshRate 
		bufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 프로그레시브로 설정
		bufferDesc.Format = RenderFormatToDXGIFormat(_desc->format);

		DXGI_SWAP_CHAIN_DESC desc;
		desc.BufferDesc = bufferDesc;
		desc.SampleDesc = sampleDesc;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = _desc->bufferCount;
		desc.OutputWindow = glfwGetWin32Window((GLFWwindow*)_window->GetNativeWindow());
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = 0;

		HRESULT hr =  device->GetFactory()->CreateSwapChain(device->GetDevice(), &desc, swapchain.GetAddressOf());
		STEINS_CORE_ASSERT(SUCCEEDED(hr), "Failed To Create Swapchain!");

	}
	void D3D11Swapchain::SetVSync(bool _enabled)
	{
	}
	void D3D11Swapchain::SwapBuffers()
	{
		swapchain->Present(desc.isVSync, 0);
	}
}