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

#include <folly/Chrono.h>
#include <folly/Random.h>
//#include <folly/io/async/AsyncSocket.h>
#include <folly/io/async/EventBase.h>
#include <folly/io/async/HHWheelTimer.h>
#include <iostream>
struct TimeOut : public folly::HHWheelTimerHighRes ::Callback
{
    void timeoutExpired() noexcept override
    {
        //      std::cout << "Hello, World!" << std::endl;
    }
    void callbackCanceled() noexcept override
    {
        Callback::callbackCanceled();
    }
};

void worker_thread()
{
    folly::EventBase event_base;

    auto timer = folly::HHWheelTimerHighRes::newTimer(&event_base);
    std::vector<TimeOut> vi(100000);
    for (auto i = 1; i < 100000; i++)
    {
        timer->scheduleTimeout(&vi[i], std::chrono::milliseconds(i * 1));
    }
    event_base.loop();
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
