#include <iostream>
#include <boost/asio.hpp>

void sayHello(const boost::system::error_code& error)
{
  std::cout << "Hello, world!" << std::endl;
}

int main()
{
  boost::asio::io_service io_service;
  boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(3));
  timer.async_wait(&sayHello);
  int n = io_service.run();
  std::cout << "io_service return : " << n << std::endl;
  return 0;
}
