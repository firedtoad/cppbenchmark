#include "timer.h"
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
static volatile int SIG = 0;

static void *thread_timer(void *p)
{
    for (;;)
    {
        skynet_updatetime();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void worker_thread()
{
    for (auto i = 0; i < 100000; i++)
    {
        skynet_timeout(0, i * 1, 0);
    }
}
int main()
{
    skynet_timer_init();
    std::vector<std::unique_ptr<std::thread>> vecThread;
    vecThread.emplace_back(std::make_unique<std::thread>(thread_timer, nullptr));
    for (auto i = 0; i < 2; i++)
    {
        vecThread.emplace_back(std::make_unique<std::thread>(worker_thread));
    }
    for (auto &it : vecThread)
    {
        it->join();
    }
    return 0;
}
