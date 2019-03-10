#include <cxxopts.h>
#include <logger.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <memory>
#include <tuple>
#include <server.h>
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

  inline void reply_and_log(const std::string & response, const std::string & log_message, boost::system::error_code & error_write_back)
  {
    socket.write_some(boost::asio::buffer(response.c_str(), response.size()), error_write_back);
    logger->log(log_message);
  }

  void prepare_message_for_parsing(const std::size_t bytes_transferred)
  {
    data[strlen(data)-1] = '\0';
  }

  void handle_read(
      const boost::system::error_code& error,  // Result of operation.
      std::size_t bytes_transferred            // Number of bytes read.
      ) 

  {
    std::string response;
    boost::system::error_code error_write_back;
    
    if(bytes_transferred > 1)
    {
      prepare_message_for_parsing(bytes_transferred);

      if(strcmp(data, STR_BAT_VOLT) == 0)
      {
        response = "4V";
        reply_and_log(response, "Request for Voltage received", error_write_back);
      }

      else if(strcmp(data, STR_PROT_NAME) == 0)
      {
        response = "TCP/IP";
        reply_and_log(response, "Request for Protocol Name received", error_write_back);
      }

      else if(strcmp(data, STR_VIN_NUM) == 0)
      {
        response = "123456";
        reply_and_log(response, "Request for Vehicle Number received", error_write_back);
      }

      else if(strcmp(data, STR_ECU_NAME) == 0)
      {
        response = "ECU_NAME";
        reply_and_log(response, "Request for ECU Name received", error_write_back);
      }

      else if(strcmp(data, STR_DTC_CNT) == 0)
      {
        response = "DTC_COUNT";
        reply_and_log(response, "Request for DTC received", error_write_back);
      }

      else if(strcmp(data, STR_SUPP_PID_1) == 0)
      {
        response = "STR_SUPP_PID_1";
        reply_and_log(response, "Request for supported PIDs 1 - 32 for MODE 1", error_write_back);
      }

      else if(strcmp(data, STR_SUPP_PID_9) == 0)
      {
        response = "STR_SUPP_PID_9";
        reply_and_log(response, "Request forsupported PIDs 1 - 32 for MODE 9", error_write_back);
      }

      else if(strcmp(data, STR_DTC) == 0)
      {
        response = "DTC_COUNT";
        reply_and_log(response, "Request for DTCs that are set.", error_write_back);
      }

      else if(strcmp(data, STR_ENGINE_RPM) == 0)
      {
        response = "10000";
        reply_and_log(response, "Request for engine rpm received", error_write_back);
      }

      else if(strcmp(data, STR_VEHICLE_SPEED) == 0)
      {
        response = "100 km/hr";
        reply_and_log(response, "Request for vehicle received", error_write_back);
      }

      else if(strcmp(data, STR_FUEL_PRESS) == 0)
      {
        response = "52psi";
        reply_and_log(response, "Request for fuel pressure received)", error_write_back);
      }

      else if(strcmp(data, STR_MAP_PRESS) == 0)
      {
        response = "10psi";
        reply_and_log(response, "Request for map pressure received", error_write_back);
      }

      else if(strcmp(data, STR_FUEL_FLOW_RATE) == 0)
      {
        response = "2.25";
        reply_and_log(response, "Request for fuel flow rate received", error_write_back);
      }

      else if(strcmp(data, STR_COOLANT_TEMP) == 0)
      {
        response = "10 degree celsius";
        reply_and_log(response, "Request for coolant temperature received", error_write_back);
      }

      else if(strcmp(data, STR_FUEL_TANK_LEV) == 0)
      {
        response = "50%";
        reply_and_log(response, "Request for fuel tank level received", error_write_back);
      }

      else if(strcmp(data, STR_INTAKE_AIR_TMP) == 0)
      {
        response = "23 degree celsius";
        reply_and_log(response, "Request for air intake temperature received", error_write_back);
      }

      else if(strcmp(data, STR_OIL_TMP) == 0)
      {
        response = "23 degree celsius";
        reply_and_log(response, "Request for oil temperature received", error_write_back);
      }

      else if(strcmp(data, STR_FREEZE_FRAME_DTC) == 0)
      {
        response = "FREEZE_FRAME_DTC";
        reply_and_log(response, "Request for Freeze Frame DTC received", error_write_back);
      }


    }

    socket.async_read_some(
      boost::asio::buffer(data, MAX_LEN),
      boost::bind(&Server::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));

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