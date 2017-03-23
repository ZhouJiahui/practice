#include <map>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

const int MAX_LENGTH = 128;
//typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
//typedef boost::shared_ptr<boost::array<char, max_length> > char_array_ptr;

struct Session;
typedef boost::shared_ptr<Session> session_ptr; 

enum Period {
  kUNKNOWN,
  kREADSIZE,
  kREADDATA,
  kWRITESIZE,
  kWRITEDATA
};

struct Session {
  boost::asio::ip::tcp::socket sock;
  size_t size;
  char data[MAX_LENGTH];
  std::string str;
  //std::string data;
  Period period;
  Session(boost::asio::io_service& io_service);
};

class MapServer {
public:
  MapServer(int port);
  void Start();
  //void Close();
//private:
  bool GetValueFromKVMap(session_ptr session);
  bool SetValueIntoKVMap(session_ptr session);
  void Accept();
  void HandleAccept(session_ptr session, const boost::system::error_code& error);
  void HandleCallback(session_ptr session, const boost::system::error_code& error);

  void ProcessReadSize(session_ptr session);
private:
  std::map<std::string, std::string> kv_map_;
  int port_;
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
};


