#pragma once


namespace Daydream
{
	class PipelineState;

	class PipelineStateManager
	{
	public:
		PipelineStateManager();

		Shared<PipelineState> Get(const String& _path);

		void CreateEssentialPipelineStates();
	private:
		HashMap<String, Shared<PipelineState>> pipelineCache;
	};
}