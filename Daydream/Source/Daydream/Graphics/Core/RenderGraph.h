#pragma once

namespace Daydream
{
	struct RenderGraphResourceDesc
	{
		// Texture2DDesc 또는 사용자 정의 간소화 구조체
		RenderFormat format;
		UInt32 width;
		UInt32 height;
		// ...
	};

	struct RenderGraphResourceHandle
	{
		UInt32 id = UINT32_MAX;
		bool IsValid() const { return id != UINT32_MAX; }
	};

	struct RenderGraphPassHandle
	{
		UInt32 id = UINT32_MAX;
		bool IsValid() const { return id != UINT32_MAX; }
	};


	class RenderGraph
	{
	public:
		RenderGraph();
		~RenderGraph();

		RenderGraphResourceHandle AddResource(const String& _name, const RenderGraphResourceDesc& _desc);
		RenderGraphPassHandle AddPass(const String& _name, FunctionPtr<void()> _execute);

		void Read(RenderGraphPassHandle _pass, RenderGraphResourceHandle _resource);
		void Write(RenderGraphPassHandle _pass, RenderGraphResourceHandle _resource);

		bool Compile();
		void Execute();
		void Reset();

	private:
		struct ResourceNode
		{
			String name;
		};

		struct PassNode
		{
			String name;
			FunctionPtr<void()> execute;
			Array<UInt32> reads;
			Array<UInt32> writes;
		};

		void BuildDependencyGraph(Array<Array<UInt32>>& _edges, Array<UInt32>& _inDegree) const;

		Array<ResourceNode> resources;
		Array<PassNode> passes;
		Array<UInt32> executionOrder;
	};
}
