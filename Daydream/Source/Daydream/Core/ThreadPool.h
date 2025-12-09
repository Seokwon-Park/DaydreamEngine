#pragma once

namespace Daydream
{
    class ThreadPool {
    public:
        ThreadPool(UInt64 _numThreads = std::thread::hardware_concurrency() - 1);
        ~ThreadPool() = default;

        template<class F, class... Args>
        static auto Enqueue(F&& _f, Args&&... _args) -> std::future<std::invoke_result_t<F, Args...>>
        {
            return instance->EnqueueInternal(_f, _args);
        }


        //template<class F, class... Args> - 함수(F)와 그 함수의 인자들(Args...)을 모두 받겠다는 뜻(가변 인자 템플릿)
        //std::invoke_result_t<F, Args...> - F라는 함수에 Args를 넣었을 때 리턴되는 타입이 무엇인지 컴파일러가 계산 (예: int를 리턴하는 함수면 int가 됨)
        //auto ... -> ... - 후행리턴
        template<class F, class... Args>
        auto EnqueueInternal(F&& _f, Args&&... _args) -> std::future<std::invoke_result_t<F, Args...>> 
        {
            using ReturnType = std::invoke_result_t<F, Args...>;

            // 1. 작업을 패키징 (비동기 결과 획득을 위해)
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(
                std::bind(std::forward<F>(_f), std::forward<Args>(_args)...)
            );

            std::future<ReturnType> res = task->get_future();

            // 2. 큐에 삽입 (Lock 범위 최소화)
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                tasks.emplace([task]() { (*task)(); });
            }

            // 3. 일꾼 하나 깨우기
            cv.notify_one();
            return res;
        }

    private:
        void WorkerLoop(std::stop_token _stoken) 
        {
            while (!_stoken.stop_requested()) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);

                    // 큐가 비어있으면 대기. (stop 요청이 오면 즉시 깨어남)
                    // cv_any는 stop_token을 지원함!
                    if (!cv.wait(lock, _stoken, [this] { return !tasks.empty(); })) {
                        return; // Stop 요청으로 인해 깨어남 -> 종료
                    }

                    // 작업 꺼내기
                    task = std::move(tasks.front());
                    tasks.pop();
                }

                // Lock 없이 실행
                task();
            }
        }

        static ThreadPool* instance;

        std::vector<std::jthread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable_any cv;
    };
}
