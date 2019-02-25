#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>


using boost::asio::ip::tcp;
using std::cout;
using std::cerr;
using std::endl;

constexpr size_t MAX_LEN = 1024;
class Server : public std::enable_shared_from_this<Server>{
public:
  Server(boost::asio::io_service& ios,
         short port) : ios(ios), acceptor(ios, tcp::endpoint(tcp::v4(), port)), socket(ios)
  {
    acceptor.async_accept(socket,
                          boost::bind(&Server::handle_accept, this,
                                      boost::asio::placeholders::error));
  }

  void handle_accept(const boost::system::error_code& err)
  {
    if (!err) {
    std::string s = socket.remote_endpoint().address().to_string();
    std::cout<<"Connected to "<<s<<std::endl;
	socket.async_read_some(
	          boost::asio::buffer(data, MAX_LEN),
	          boost::bind(&Server::handle_read, this,
	                      boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)
	          );
    }
    else {
      std::cerr << "err: " + err.message() << std::endl;
    }
  }

	void handle_read(
	  const boost::system::error_code& error, // Result of operation.
	  std::size_t bytes_transferred           // Number of bytes read.
	){
		for(int i = 0 ; i < bytes_transferred ; i++)
			std::cout<<data[i];
		std::cout<<std::endl;

	}


  tcp::socket& get_socket()
  {
    return socket;
  }

private:
  tcp::socket socket;
  boost::asio::io_service& ios;
  tcp::acceptor acceptor;
  
  char data[MAX_LEN];
};

int main(int argc, char *argv[])
{
  try {
    boost::asio::io_service ios;
    Server s(ios, 35000);
    ios.run();
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}