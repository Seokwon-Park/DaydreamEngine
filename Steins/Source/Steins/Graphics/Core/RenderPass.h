#pragma once

#include "Steins/Enum/RendererEnums.h"
#include "Steins/Graphics/Resources/Framebuffer.h"

namespace Steins
{
	enum class LoadOp {
		LOAD,
		CLEAR,
		DONT_CARE
	};

	enum class StoreOp {
		STORE,
		DONT_CARE
	};

	enum class AttachmentType {
		COLOR,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL
	};

	struct AttachmentDescription
	{
		AttachmentType type;
		RenderFormat format;
		LoadOp loadOp;
		StoreOp storeOp;
		LoadOp stencilLoadOp;   // depth-stencil¿ë
		StoreOp stencilStoreOp; // depth-stencil¿ë
		UInt32 sampleCount;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual void Begin(Shared<Framebuffer> _framebuffer) = 0;
		virtual void End() = 0;

		static Shared<RenderPass> Create(const std::vector<AttachmentDescription>& _attachments);
	private:
	};
}