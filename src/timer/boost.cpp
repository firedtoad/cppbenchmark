#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <thread>

void worker_thread()
{
    boost::asio::io_service io_service;

    std::vector<boost::asio::deadline_timer> vi;
    vi.reserve(100000);
    for (auto i = 1; i < 100000; i++)
    {
        auto &timer = vi.emplace_back(io_service);
        timer.expires_from_now(boost::posix_time::millisec(i * 1));
        timer.async_wait(
            [](boost::system::error_code)
            {
                //            std::cout << __PRETTY_FUNCTION__ << '\n';
            });
    }

    io_service.run();
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
