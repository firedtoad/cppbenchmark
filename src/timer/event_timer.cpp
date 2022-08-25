#include <event2/event.h>
#include <event2/event_struct.h>
#include <iostream>
#include <thread>
#include <vector>

static void time_cb(evutil_socket_t fd, short event, void *arg) {}
void worker_thread()
{
    auto loop = event_base_new();
    std::vector<event> vi;
    std::vector<uint64_t> vx;
    vi.resize(100000);
    vx.resize(100000);
    struct timeval tv;
    for (auto i = 0; i < 100000; i++)
    {
        vx[i] = i;
        evtimer_assign(&vi[i], loop, time_cb, &vx[i]);
        auto d     = i * 1;
        auto s     = d / 1000;
        auto u     = d % 1000;
        tv.tv_sec  = s;
        tv.tv_usec = u * 1000;
        evtimer_add(&vi[i], &tv);
    }
    int n = event_base_dispatch(loop);
    event_base_free(loop);
}
int main()
{

    std::vector<std::unique_ptr<std::thread>> vecThread;
    for (auto i = 0; i < 4; i++)
    {
        vecThread.emplace_back(std::make_unique<std::thread>(worker_thread));
    }
    for (auto &it : vecThread)
    {
        it->join();
    }
    return 0;
}
