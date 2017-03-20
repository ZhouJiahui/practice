#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

void echo_server(socket_ptr sock)
{
  boost::system::error_code error;
  char data[1024];
  size_t size = sock->read_some(buffer(data), error);
  if (error)
  {
    std::cout << boost::system::system_error(error).what() << std::endl;
  }
  sock->write_some(buffer(data, size), error);
  if (error)
  {
    std::cout << boost::system::system_error(error).what() << std::endl;
  }
}

int main()
{
  io_service io_service;
  ip::tcp::acceptor acceptor(io_service, ip::tcp::endpoint(ip::tcp::v4(), 2017));
  while (true)
  {
    socket_ptr sock(new ip::tcp::socket(io_service));
    acceptor.accept(*sock);
    boost::thread t(boost::bind(echo_server, sock));     
  }
  return 0;
}
