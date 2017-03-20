#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using namespace boost::asio;

const int max_length = 1024;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
typedef boost::shared_ptr<boost::array<char, max_length> > char_array_ptr;

void handle_read(socket_ptr sock, char_array_ptr data, size_t bytes_transferred, const boost::system::error_code& error);
void handle_write(socket_ptr sock, char_array_ptr data, const boost::system::error_code& error);
void handle_accept(socket_ptr sock, char_array_ptr data, const boost::system::error_code& error);
void start_server(io_service& io_serv, ip::tcp::acceptor& acceptor);

io_service io_serv;
ip::tcp::acceptor acceptor(io_serv, ip::tcp::endpoint(ip::tcp::v4(), 2017));

void handle_write(socket_ptr sock, char_array_ptr data, const boost::system::error_code& error)
{
  if (!error)
  {
    std::cout << "write complete." << std::endl;
    sock->async_read_some(buffer(*data, max_length), 
                        boost::bind(&handle_read, sock, data, placeholders::bytes_transferred, placeholders::error));
  }
}

void handle_read(socket_ptr sock, char_array_ptr data, size_t bytes_transferred, const boost::system::error_code& error)
{
  if (!error)
  {
    sock->async_write_some(buffer(*data, bytes_transferred), 
                        boost::bind(&handle_write, sock, data, placeholders::error));
  }
}

void handle_accept(socket_ptr sock, 
                  char_array_ptr data, 
                  const boost::system::error_code& error)
{
  if (!error)
  {
    sock->async_read_some(buffer(*data, max_length), 
                        boost::bind(&handle_read, sock, data, placeholders::bytes_transferred, placeholders::error));
  }
  start_server(io_serv, acceptor);
}

void start_server(io_service& io_serv, ip::tcp::acceptor& acceptor)
{
  socket_ptr sock(new ip::tcp::socket(io_serv));
  char_array_ptr data(new boost::array<char, max_length>);
  acceptor.async_accept(*sock, 
                        boost::bind(&handle_accept, sock, data, placeholders::error));
}

int main()
{
  start_server(io_serv, acceptor);
  io_serv.run();
  return 0;
}
