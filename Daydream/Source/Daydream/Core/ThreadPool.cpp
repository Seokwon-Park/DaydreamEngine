#include "DaydreamPCH.h"
#include "ThreadPool.h"

namespace Daydream
{
	ThreadPool::ThreadPool(UInt64 _numThreads)
	{
		workers.reserve(_numThreads);
		for (size_t i = 0; i < _numThreads; ++i) {
			workers.emplace_back(
				[this](std::stop_token _stoken) 
				{
				WorkerLoop(_stoken);
				});
		}
	}


}

