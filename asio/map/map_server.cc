#include "map_server.h"
#include <iostream>

Session::Session(boost::asio::io_service& io_service): sock(io_service) {
}

MapServer::MapServer(int port): acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
}

void MapServer::Start() {
  Accept();
  io_service_.run();
}

bool MapServer::GetValueFromKVMap(session_ptr session) {
  std::vector<std::string> words;
  StringSplit(session->str, ::DELIMITER, words);
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
  StringSplit(session->str, ::DELIMITER, words);
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
    ++total_count_;
    std::cout << "#:" << total_count_ << " accept sucessfully." << std::endl;
    session->period = kREADSIZE;
    boost::asio::async_read(session->sock, boost::asio::buffer(&session->size, sizeof(session->size)),
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
        std::cout << "kReadSize:" << session->size << std::endl;
        boost::asio::async_read(session->sock, boost::asio::buffer(session->data, session->size),
                                boost::bind(&MapServer::HandleCallback,
                                            this,
                                            session,  
                                            boost::asio::placeholders::error));
       break;
      case kREADDATA:
        session->str = std::string(session->data, session->data + session->size);
        std::cout << "kREADDATA:" << session->str << std::endl;
        if (StringStartWith(session->str, ::GET) && GetValueFromKVMap(session)) {
        } else if (StringStartWith(session->str, ::SET) && SetValueIntoKVMap(session)) {
        } else {
          // error
          return;
        }
        session->period = kWRITESIZE; 
        session->size = boost::asio::detail::socket_ops::host_to_network_long(session->size);
        boost::asio::async_write(session->sock, boost::asio::buffer(&session->size, sizeof(session->size)),
                                 boost::bind(&MapServer::HandleCallback,
                                             this,
                                             session,  
                                             boost::asio::placeholders::error));
        break;
      case kWRITESIZE:
        session->period = kWRITEDATA;
        boost::asio::async_write(session->sock, boost::asio::buffer(session->data, session->size),
                                 boost::bind(&MapServer::HandleCallback,
                                             this,
                                             session,  
                                             boost::asio::placeholders::error));
        break;
      case kWRITEDATA:
        std::cout << "kWRITEDATA:Send done." << std::endl;
        break;
      default:
        // never go there
        return;
    }
  } else {
    std::cout << boost::system::system_error(error).what() << std::endl;
  }
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cerr << "Usage: map_server <port>\n";
    return 1;
  }
  try {
    MapServer mapServer(atoi(argv[1]));
    mapServer.Start();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
