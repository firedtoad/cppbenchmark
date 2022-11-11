// Copyright 2020 The Division Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// Author dietoad@gmail.com && firedtoad@gmail.com

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
int main(int argc, char **argv)
{

    //    std::vector<std::unique_ptr<std::thread>> vecThread;
    //    for (auto i = 0; i < 4; i++)
    //    {
    //        vecThread.emplace_back(std::make_unique<std::thread>(worker_thread));
    //    }
    //    for (auto &it : vecThread)
    //    {
    //        it->join();
    //    }
    return 0;
}
