#pragma once

namespace Daydream
{
    //class Thread {
    //public:
    //    Thread() {}
    //    virtual ~Thread() {
    //        stop();
    //    }

    //    void start() {
    //        if (!thread.joinable()) {
    //            thread = std::jthread(&Thread::run, this);
    //        }
    //    }

    //    void stop() {
    //        if (thread.joinable()) {
    //            thread.request_stop();
    //            cv.notify_all();  // 대기 중인 스레드 깨우기
    //            // jthread는 소멸자에서 자동으로 join됨
    //        }
    //    }

    //protected:
    //    virtual void run(std::stop_token stoken) = 0;
    //    std::condition_variable_any cv;  // stop_token과 함께 사용

    //private:
    //    std::jthread thread;
    //};

    //// 렌더링 전용 스레드
    //class RenderThread : public Thread {
    //public:
    //    RenderThread() {}
    //    ~RenderThread() {}

    //    void SubmitRenderTask(std::function<void()> task) {
    //        std::lock_guard<std::mutex> lock(queueMutex);
    //        tasks.push(task);
    //        cv.notify_one();
    //    }

    //protected:
    //    void run(std::stop_token stoken) override {
    //        while (!stoken.stop_requested()) {
    //            std::function<void()> task;
    //            {
    //                std::unique_lock<std::mutex> lock(queueMutex);
    //                cv.wait(lock, stoken, [this] { return !tasks.empty(); });

    //                if (stoken.stop_requested() && tasks.empty()) break;

    //                if (!tasks.empty()) {
    //                    task = tasks.front();
    //                    tasks.pop();
    //                }
    //            }
    //            if (task) task();
    //        }
    //    }

    //private:
    //    std::queue<std::function<void()>> tasks;
    //    std::mutex queueMutex;
    //};

    //// 오디오 전용 스레드
    //class AudioThread : public Thread {
    //public:
    //    AudioThread() {}
    //    ~AudioThread() {}

    //    void submitAudioTask(std::function<void()> task) {
    //        std::lock_guard<std::mutex> lock(queueMutex);
    //        tasks.push(task);
    //        cv.notify_one();
    //    }

    //protected:
    //    void run(std::stop_token stoken) override {
    //        while (!stoken.stop_requested()) {
    //            std::function<void()> task;
    //            {
    //                std::unique_lock<std::mutex> lock(queueMutex);
    //                cv.wait(lock, stoken, [this] { return !tasks.empty(); });

    //                if (stoken.stop_requested() && tasks.empty()) break;

    //                if (!tasks.empty()) {
    //                    task = tasks.front();
    //                    tasks.pop();
    //                }
    //            }
    //            if (task) task();
    //        }
    //    }

    //private:
    //    std::queue<std::function<void()>> tasks;
    //    std::mutex queueMutex;
    //};
}
