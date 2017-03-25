#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

class Server {
public:
  Server(int port): acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
                    socket_(io_service_){
}

  void Start() {
    Accept();
    io_service_.run();
  }

  void Accept() {
    acceptor_.async_accept(socket_,
                           boost::bind(&Server::HandleRead,
                                       this,
                                       boost::asio::placeholders::error));
  }

  void HandleRead(const boost::system::error_code& error) {
    if (!error) {
      boost::asio::async_read_until(socket_, data_, "*",
                                boost::bind(&Server::HandleReadLine,
                                this,
                                boost::asio::placeholders::error));
    } else {
      std::cout << boost::system::system_error(error).what() << std::endl;
    }
  }

  void HandleReadLine(const boost::system::error_code& error) {
    std::string str;
    std::istream is(&data_);
    std::cout << "begin==\n";
    size_t i = 0;
    const char *buf = boost::asio::buffer_cast<const char*>(data_.data());
    const char *it = std::find(buf, buf + data_.size(), '*');
    if (it != (buf + data_.size())) {
      std::string str(buf, it);
      std::cout << str;
      size_t n = it - buf + 1;
      data_.consume(n);
    }
    std::cout << "===end\n";
    boost::asio::async_read_until(socket_, data_, "*",
                                  boost::bind(&Server::HandleReadLine,
                                  this,
                                  boost::asio::placeholders::error));
   }
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf data_;
};

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Usage: read_util_server <port>\n";
    return 1;
  }
  try {
    Server server(atoi(argv[1]));
    server.Start();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
