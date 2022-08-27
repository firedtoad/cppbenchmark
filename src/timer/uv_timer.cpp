#include <iostream>
#include <thread>
#include <uv.h>
#include <vector>

void timer_cb(uv_timer_t *handle)
{
    //    std::cout<<handle->data<<'\n';
}

void worker_thread()
{
    auto loop = uv_loop_new();
    std::vector<uv_timer_t> vi;
    vi.resize(100000);
    for (auto i = 0; i < 100000; i++)
    {
        uv_timer_init(loop, &vi[i]);
        uv_timer_start(&vi[i], timer_cb, i * 1, 0);
    }
    uv_run(loop, UV_RUN_DEFAULT);
}
int main(int argc, char **argv)
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
