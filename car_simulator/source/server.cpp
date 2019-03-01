#include <cxxopts.h>
#include <logger.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include <tuple>

using boost::asio::ip::tcp;
using std::cerr;
using std::cout;
using std::endl;

constexpr size_t MAX_LEN = 1024;
class Server : public std::enable_shared_from_this<Server> {
 public:
  Server(boost::asio::io_service& ios, short port, Log::Logger* const logger)
      : ios(ios),
        acceptor(ios, tcp::endpoint(tcp::v4(), port)),
        socket(ios),
        logger(logger) {
    acceptor.async_accept(socket,
                          boost::bind(&Server::handle_accept, this,
                                      boost::asio::placeholders::error));
  }

  void handle_accept(const boost::system::error_code& err) {
    if (!err) {
      std::string s = socket.remote_endpoint().address().to_string();
      logger->log("Connected to " + s);
      socket.async_read_some(
          boost::asio::buffer(data, MAX_LEN),
          boost::bind(&Server::handle_read, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
    } else {
      std::cerr << "err: " + err.message() << std::endl;
    }
  }

  void handle_read(
      const boost::system::error_code& error,  // Result of operation.
      std::size_t bytes_transferred            // Number of bytes read.
  ) {
    for (unsigned i = 0; i < bytes_transferred; i++) std::cout << data[i];
    std::cout << std::endl;
  }

  tcp::socket& get_socket() { return socket; }

 private:
  boost::asio::io_service& ios;
  tcp::acceptor acceptor;
  tcp::socket socket;
  char data[MAX_LEN];

  const std::shared_ptr<Log::Logger> logger;
};

struct net_config {
  std::string address;
  std::string port;

  std::string to_string() {
    std::ostringstream ss;
    ss << "Net config" << std::endl;
    if(address != "")
      ss << "address: "<< address << std::endl;
    else
      ss<<"Warning: address not specified"<< std::endl;
    if(port != "")
      ss << "port: "<< port << std::endl;
    else
      ss<<"Warning: port not specified"<< std::endl;
    
    return ss.str();
  }
};

using LogFile = std::string;
std::tuple<net_config, LogFile> create_config_from_comm_ln_options(
    int argc, char* argv[]) {
  try {
    cxxopts::Options options(argv[0], " - example command line options");
    options.add_options()("s,server-address", "Address" , cxxopts::value<std::string>())
    ("p,port", "Port", cxxopts::value<std::string>())
    ("f,file-log", "Log File" , cxxopts::value<std::string>());

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
      std::cout << options.help({""}) << std::endl;
      exit(0);
    }

    net_config config;
    std::string logfile;
    if (result.count("s")) {
      config.address = result["s"].as<std::string>();
    }
    if (result.count("p")) {
      config.port = result["p"].as<std::string>();

    }
    if (result.count("f")) {
       logfile = result["f"].as<std::string>();
    }

    return std::make_tuple(config, logfile);

  } catch (const cxxopts::OptionException& e) {
    std::cout << "error parsing options: " << e.what() << std::endl;
    exit(1);
  }
  return {};
}

int main(int argc, char* argv[]) {
  std::tuple<net_config, LogFile> parse_result =
      create_config_from_comm_ln_options(argc, argv);

  std::shared_ptr<Log::Logger> logger(new Log::FileCerrLogger("log.txt"));
  logger->log(std::get<net_config>(parse_result).to_string());
  try {
    boost::asio::io_service ios;
    Server s(ios, 35000, logger.get());
    ios.run();
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}