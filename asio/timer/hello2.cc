#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

void sayHello(const boost::system::error_code& error,
              boost::asio::deadline_timer* timer,
              int* count)
{
  std::cout << "#:" << *count <<" Hello, world!" << std::endl;
  if (--(*count) > 0)
  {
    timer->expires_at(timer->expires_at() + boost::posix_time::seconds(1));
    timer->async_wait(boost::bind(sayHello, _1, timer, count));
  }
}

int main()
{
  boost::asio::io_service io_service;
  boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(1));
  int count = 3;
  timer.async_wait(boost::bind(sayHello, _1, &timer, &count));
  int n = io_service.run();
  std::cout << "io_service return : " << n << std::endl;
  return 0;
}
