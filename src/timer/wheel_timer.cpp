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
int main(int argc, char **argv)
{
    //    skynet_timer_init();
    //    std::vector<std::unique_ptr<std::thread>> vecThread;
    //    vecThread.emplace_back(std::make_unique<std::thread>(thread_timer, nullptr));
    //    for (auto i = 0; i < 2; i++)
    //    {
    //        vecThread.emplace_back(std::make_unique<std::thread>(worker_thread));
    //    }
    //    for (auto &it : vecThread)
    //    {
    //        it->join();
    //    }
    return 0;
}
