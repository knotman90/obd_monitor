#ifndef __LOGGER__
#define __LOGGER__

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


namespace Log {

class Logger {
 public:
  Logger(){};
  virtual ~Logger(){};
  Logger(const Logger&) = delete;             // non construction-copyable
  Logger& operator=(const Logger&) = delete;  // non copyable
  virtual void log(const std::string& s) = 0;
};

class FileLogger : public Logger {
 public:
  FileLogger(const std::string& filepath) {
    try {
      logfile.open(filepath+get_timestamp("%F_%T"));
    } catch (const std::ifstream::failure& e) {
      std::cerr << "Exception opening/reading file";
      std::cerr << e.what() << std::endl;
    }
  }

  virtual void log(const std::string& s) override{
    try {
      const std::string line = get_timestamp() + "\t" + s;
      logfile << line << std::endl;
    } catch (const std::ifstream::failure& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  virtual ~FileLogger(){
  	logfile.close();
  }

 private:
  std::ofstream logfile;

  std::string get_timestamp(const std::string& format = "%F %T") {
    const std::chrono::system_clock::time_point now =
        std::chrono::system_clock::now();
    const std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream transTime;
    transTime << std::put_time(std::localtime(&now_c), format.c_str());
    return transTime.str();
  }
};

class FileCerrLogger : public FileLogger{
public:
	FileCerrLogger(const std::string& filepath) : FileLogger(filepath){};

	void log(const std::string& s) override {
		std::cerr<<s<<std::endl;
		FileLogger::log(s);
	}
};

}  // namespace Log

#endif  //__LOGGER__