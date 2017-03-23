#include <boost/asio.hpp>
#include "common.h"

class MapClient {
public:
  MapClient(char *ip, char *port);
  std::string Get(const std::string& key);
  std::string Set(const std::string& key, const std::string& value);
private:
  bool Write(size_t host_size, const std::string& str);
  bool Read(size_t& host_size, std::string& str);
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
};
