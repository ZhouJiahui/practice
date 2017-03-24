#ifndef PRACTISE_ASIO_MAPCLIENT_H_
#define PRACTISE_ASIO_MAPCLIENT_H_
#include <boost/asio.hpp>
#include "common.h"

class MapClient {
public:
  MapClient(const char *ip, const char *port);
  std::string Get(const std::string& key);
  std::string Set(const std::string& key, const std::string& value);
private:
  bool Write(size_t host_size, const std::string& str);
  bool Read(size_t& host_size, std::string& str);
private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
};
#endif // PRACTISE_ASIO_MAPCLIENT_H_
