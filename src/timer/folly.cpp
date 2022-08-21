#include <iostream>
#include <folly/Random.h>
#include <folly/Chrono.h>
#include <folly/io/async//AsyncSocket.h>
#include <folly/io/async//EventBase.h>
#include <folly/io/async/HHWheelTimer.h>
struct TimeOut : public folly::HHWheelTimerHighRes ::Callback {
    void timeoutExpired() noexcept override {
//      std::cout << "Hello, World!" << std::endl;
    }
    void callbackCanceled() noexcept override {
        Callback::callbackCanceled();
    }
};

void worker_thread() {
    folly::EventBase event_base;

    auto timer = folly::HHWheelTimerHighRes::newTimer(&event_base);
    std::vector<TimeOut> vi(100000);
    for (auto i = 1; i < 100000; i++) {
        timer->scheduleTimeout(&vi[i], std::chrono::milliseconds(i * 1));
    }
    event_base.loop();
}
int main() {

    std::vector<std::unique_ptr<std::thread>> vecThread;
    for (auto i = 0; i < 4; i++) {
        vecThread.emplace_back(std::make_unique<std::thread>(worker_thread));
    }
    for (auto &it : vecThread) {
        it->join();
    }

    return 0;
}
