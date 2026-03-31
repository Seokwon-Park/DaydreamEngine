#pragma once

namespace Daydream
{
	class RenderCommandQueue;

	class RenderThread
	{
	public:
		RenderThread();
		~RenderThread();

		void Start();
		void Stop();

		void Submit(RenderCommandQueue* _queue, FunctionPtr<void()> _onComplete);
		void WaitIdle();

		bool IsRunning() const { return isRunning; }
	protected:

	private:
		void WorkerLoop(std::stop_token _stopToken);

		std::jthread workerThread;

		struct SubmittedQueue
		{
			RenderCommandQueue* queue = nullptr;
			FunctionPtr<void()> onComplete = nullptr;
		};

		std::mutex queueMutex;
		std::condition_variable queueCV;
		Queue<SubmittedQueue> submitQueue;

		std::mutex idleMutex;
		std::condition_variable idleCV;
		std::atomic<UInt32> inFlightCount = 0;

		std::atomic<bool> isRunning = false;
	};
}
