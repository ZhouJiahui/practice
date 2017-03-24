#include "map_client.h"
#include <strings.h>
#include <iostream>
#include <vector>

MapClient::MapClient(const char *ip, const char *port): socket_(io_service_){
  boost::asio::ip::tcp::resolver resolver(io_service_);
  boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), ip, port);
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
  boost::asio::connect(socket_, iterator);
}

std::string MapClient::Get(const std::string& key) {
  std::string data = std::string(::GET + ::DELIMITER + key);
  size_t host_size;
  std::string str;
  if (!Write(data.size(), data) || !Read(host_size, str)) {
      return "ERROR";
  }
  return str;
}

std::string MapClient::Set(const std::string& key, const std::string& value) {
  std::string data = std::string(::SET + ::DELIMITER + key + ::DELIMITER + value);
  size_t host_size;
  std::string str;
  if (!Write(data.size(), data) || !Read(host_size, str)) {
      return "ERROR";
  }
  return str;
}

bool MapClient::Write(size_t host_size, const std::string& str) {
  boost::system::error_code error;
  size_t net_size = boost::asio::detail::socket_ops::host_to_network_long(host_size);
  size_t write_n = socket_.write_some(boost::asio::buffer(&net_size, sizeof(net_size)), error);
  if (error) {
    std::cout << boost::system::system_error(error).what() << std::endl;
    return false;
  }
  if (write_n != sizeof(net_size)) {
    std::cout << "write fail. write_n:" << write_n << " host_size:" << host_size << std::endl;
    return false;
  }
  write_n = socket_.write_some(boost::asio::buffer(str.c_str(), host_size), error);
  if (error) {
    std::cout << boost::system::system_error(error).what() << std::endl;
    return false;
  }
  if (write_n != host_size) {
    std::cout << "write fail. write_n:" << write_n << " host_size:" << host_size << std::endl;
    return false;
  }
  return true;
}

bool MapClient::Read(size_t& host_size, std::string& str) {
  boost::system::error_code error;
  size_t net_size;
  size_t read_n = socket_.read_some(boost::asio::buffer(&net_size, sizeof(net_size)), error);
  if (error) {
    std::cout << boost::system::system_error(error).what() << std::endl;
    return false;
  }
  if (read_n != sizeof(net_size)) {
    std::cout << "read fail." << std::endl;
    return false;
  }
  host_size = boost::asio::detail::socket_ops::network_to_host_long(net_size);
  char buf[MAX_LENGTH] = {0};
  read_n = socket_.read_some(boost::asio::buffer(buf, host_size), error);
  if (error) {
    std::cout << boost::system::system_error(error).what() << std::endl;
    return false;
  }
  if (read_n != host_size) {
    std::cout << "read fail." << std::endl;
    return false;
  }
  str = std::string(buf, buf + host_size);
  return true;
}

/*
int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: map_client <ip> <port>\n";
    return 1;
  }
  try {
    std::string str;
    while (std::getline(std::cin, str)) {
      MapClient mapClient(argv[1], argv[2]);
      std::vector<std::string> words;
      StringSplit(str, " ", words);
      if (words.size() == 2 && strcasecmp(words[0].c_str(), ::GET.c_str()) == 0) {
        std::cout << mapClient.Get(words[1]) << std::endl;
      } else if (words.size() == 3 && strcasecmp(words[0].c_str(), ::SET.c_str()) == 0) {
        std::cout << mapClient.Set(words[1], words[2]) << std::endl;
      } else {
        std::cout << "unknown input." << std::endl;
      }
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;

}
*/
