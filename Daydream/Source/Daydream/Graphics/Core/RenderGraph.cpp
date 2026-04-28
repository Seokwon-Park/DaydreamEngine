#include "DaydreamPCH.h"
#include "RenderGraph.h"

namespace Daydream
{
	RenderGraph::RenderGraph()
	{

	}

	RenderGraph::~RenderGraph()
	{
	}
	RenderGraphResourceHandle RenderGraph::AddResource(const String& _name)
	{
		ResourceNode node{};
		node.name = _name;
		resources.push_back(node);
		return { static_cast<UInt32>(resources.size() - 1) };
	}
	RenderGraphPassHandle RenderGraph::AddPass(const String& _name, FunctionPtr<void()> _execute)
	{
		PassNode node{};
		node.name = _name;
		node.execute = std::move(_execute);
		passes.push_back(std::move(node));
		return { static_cast<UInt32>(passes.size() - 1) };
	}
	void RenderGraph::Read(RenderGraphPassHandle _pass, RenderGraphResourceHandle _resource)
	{
		if (!_pass.IsValid() || !_resource.IsValid())return;
		if (_pass.id >= passes.size() || _resource.id >= resources.size() )return;
		passes[_pass.id].reads.push_back(_resource.id);

	}
	void RenderGraph::Write(RenderGraphPassHandle _pass, RenderGraphResourceHandle _resource)
	{
		if (!_pass.IsValid() || !_resource.IsValid())return;
		if (_pass.id >= passes.size() || _resource.id >= resources.size())return;
		passes[_pass.id].writes.push_back(_resource.id);
	}

	bool RenderGraph::Compile()
	{
		executionOrder.clear();

		Array<Array<UInt32>> edges;
		Array<UInt32> inDegree;
		BuildDependencyGraph(edges, inDegree);

		//렌더링 순서 위상정렬
		Queue<UInt32> readyPasses;
		for (UInt32 i = 0; i < inDegree.size(); i++)
		{
			//선행 작업이 없으면 추가
			if (inDegree[i] == 0)
			{
				readyPasses.push(i);
			}
		}

		while (!readyPasses.empty())
		{
			UInt32 passIndex = readyPasses.front();
			readyPasses.pop();
			executionOrder.push_back(passIndex);

			for (UInt32 next : edges[passIndex])
			{
				if (--inDegree[next] == 0)
				{
					readyPasses.push(next);
				}
			}
		}

		return executionOrder.size() == passes.size();
	}

	void RenderGraph::Execute()
	{
		for (UInt32 passIndex : executionOrder)
		{
			if (passIndex >= passes.size()) continue;
			if (passes[passIndex].execute)
			{
				passes[passIndex].execute();
			}
		}
	}

	void RenderGraph::Reset()
	{
		resources.clear();
		passes.clear();
		executionOrder.clear();
	}

	void RenderGraph::BuildDependencyGraph(Array<Array<UInt32>>& _edges, Array<UInt32>& _inDegree) const
	{
		_edges.clear();
		_inDegree.clear();

		_edges.resize(passes.size());
		_inDegree.resize(passes.size(), 0);

		//위상정렬을 위한 그래프를 구성한다.
		for (UInt32 writer = 0; writer < passes.size(); writer++)
		{
			for (UInt32 reader = 0; reader < passes.size(); reader++)
			{
				//쓰는pass랑 읽는pass가 같은 경우는 continue
				if (writer == reader)
				{
					continue;
				}

				//writer가 작성해야되는 리소스들 중에서
				bool dependent = false;
				for (UInt32 writtenRes : passes[writer].writes)
				{
					auto it = std::find(passes[reader].reads.begin(), passes[reader].reads.end(), writtenRes);

					//reader가 읽어야 하는 리소스가 있으면
					if (it != passes[reader].reads.end())
					{
						dependent = true;
						break;
					}
				}
				
				//reader는 writer 작업이 선행되어야 하므로 reader의 indegree를 추가해주고 단방향 간선을 생성한다.
				if (dependent)
				{
					_edges[writer].push_back(reader);
					_inDegree[reader]++;
				}
			}
		}
	}


}

