#include "map_server.h"
#include <iostream>

const static std::string GET = "GET";
const static std::string SET = "SET";

static void StringSplit(const std::string& str, char delimiter, std::vector<std::string>& words) {
  size_t pos1 = 0;
  size_t pos2 = 0;
  while((pos2 = str.find(delimiter, pos1)) != std::string::npos) {
    words.push_back(str.substr(pos1, pos2 - pos1));
    pos1 = pos2 + 1; 
  } 
  if (!str.empty()) {
    words.push_back(str.substr(pos1, pos2));
  }
}

static bool StringStartWith(const std::string& str, const std::string& word) {
  size_t i = 0;
  for(; i < word.size(); ++i) {
    if (i >= str.size() || str[i] != word[i]) {
      break;
    }
  }
  return i == word.size();
}

Session::Session(boost::asio::io_service& io_service): sock(io_service) {
}

MapServer::MapServer(int port): port_(port),
                                acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_)) {
}

void MapServer::Start() {
  Accept();
  io_service_.run();
}

bool MapServer::GetValueFromKVMap(session_ptr session) {
  std::vector<std::string> words;
  StringSplit(session->str, '*', words);
  if (words.size() != 2) {
    return false;
  }
  const std::string& key = words[1];
  std::string result;
  const auto& it = kv_map_.find(key);
  if (it != kv_map_.end()) {
    result = it->second;
  }
  else {
    result = "NULL";
  }
  session->size = result.size();
  for (size_t i = 0; i < session->size; ++i) {
    session->data[i] = result[i];
  }
  return true;
}

bool MapServer::SetValueIntoKVMap(session_ptr session) {
  std::vector<std::string> words;
  StringSplit(session->str, '*', words);
  if (words.size() != 3) {
    return false;
  }
  kv_map_[words[1]] = words[2];
  std::string result = "OK";
  session->size = result.size();
  for (size_t i = 0; i < session->size; ++i) {
    session->data[i] = result[i];
  }
  return true;
}

void MapServer::Accept() {
  session_ptr session(new Session(io_service_));
  acceptor_.async_accept(session->sock,
                         boost::bind(&MapServer::HandleAccept,
                                     this,
                                     session,  
                                     boost::asio::placeholders::error));
} 

void MapServer::HandleAccept(session_ptr session, const boost::system::error_code& error) {
  if (!error) {
    session->period = kREADSIZE;
    session->sock.async_read_some(boost::asio::buffer(&session->size, sizeof(size_t)),
                                  boost::bind(&MapServer::HandleCallback,
                                              this,
                                              session,  
                                              boost::asio::placeholders::error));
  } else {
    std::cout << boost::system::system_error(error).what() << std::endl;
  }
  Accept();
}

void MapServer::HandleCallback(session_ptr session, const boost::system::error_code& error) {
  if (!error) {
    switch (session->period) {
      case kREADSIZE:
        session->period = kREADDATA;
        session->size = boost::asio::detail::socket_ops::network_to_host_long(session->size);
        session->sock.async_read_some(boost::asio::buffer(session->data, session->size),
                                      boost::bind(&MapServer::HandleCallback,
                                                  this,
                                                  session,  
                                                  boost::asio::placeholders::error));
       break;
      case kREADDATA:
        session->str = std::string(session->data, session->data + session->size);
        if (StringStartWith(session->str, ::GET) && GetValueFromKVMap(session)) {
        } else if (StringStartWith(session->str, ::SET) && SetValueIntoKVMap(session)) {
        } else {
          // error
          return;
        }
        session->period = kWRITESIZE; 
        session->size = boost::asio::detail::socket_ops::host_to_network_long(session->size);
        session->sock.async_write_some(boost::asio::buffer(&session->size, sizeof(size_t)),
                                      boost::bind(&MapServer::HandleCallback,
                                                  this,
                                                  session,  
                                                  boost::asio::placeholders::error));
        break;
      case kWRITESIZE:
        session->period = kWRITEDATA;
        session->sock.async_write_some(boost::asio::buffer(session->data, session->size),
                                      boost::bind(&MapServer::HandleCallback,
                                                  this,
                                                  session,  
                                                  boost::asio::placeholders::error));
        break;
      case kWRITEDATA:
        break;
      default:
        // never go there
        return;
    }
  } else {
    std::cout << boost::system::system_error(error).what() << std::endl;
  }
}

void MapServer::ProcessReadSize(session_ptr session) {
}


int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Usage: map_server <port>\n";
    return 1;
  }
  MapServer mapServer(atoi(argv[1]));
  mapServer.Start();
  return 0;
}
