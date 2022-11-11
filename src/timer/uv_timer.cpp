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
