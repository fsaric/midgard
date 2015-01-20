#include "test.h"
#include "valhalla/midgard/logging.h"

#include <thread>
#include <future>
#include <vector>
#include <functional>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <iterator>

using namespace valhalla::midgard;

namespace {

size_t work() {
  std::ostringstream s; s << "hi my name is: " << std::this_thread::get_id();

  for(size_t i  = 0; i < 2; ++i) {
    //std::async is pretty uninteresting unless you make things yield
    LOG_ERROR(s.str()); std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LOG_WARN(s.str()); std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LOG_INFO(s.str()); std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LOG_DEBUG(s.str()); std::this_thread::sleep_for(std::chrono::milliseconds(10));
    LOG_TRACE(s.str()); std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  return 10;
}

void ThreadFileLoggerTest() {
  //get rid of it first so we don't append
  std::remove("test/thread_file_log_test.log");

  //configure logging, if you dont it defaults to standard out logging with colors
  logging::Configure({ {"type", "file"}, {"file_name", "test/thread_file_log_test.log"}, {"reopen_interval", "1"} });

  //start up some threads
  std::vector<std::future<size_t> > results;
  for(size_t i = 0; i < 4; ++i) {
    results.emplace_back(std::async(std::launch::async, work));
  }

  //dont really care about the results but we can pretend
  bool exit_code = 0;
  for(auto& result : results) {
    try {
      size_t count = result.get();
    }
    catch(std::exception& e) {
      std::cout << e.what();
      exit_code++;
    }
  }

  //wait for logger to close and reopen the file
  std::this_thread::sleep_for(std::chrono::milliseconds(1010));
  LOG_TRACE("force log close/reopen");

  //open up the file and make sure it looks right
  std::ifstream file("test/thread_file_log_test.log");

  std::string line;
  size_t error = 0, warn = 0, info = 0, debug = 0, trace = 0;
  while(std::getline(file, line)){
    error += (line.find("ERROR") != std::string::npos);
    warn += (line.find("WARN") != std::string::npos);
    info += (line.find("INFO") != std::string::npos);
    debug += (line.find("DEBUG") != std::string::npos);
    trace += (line.find("TRACE") != std::string::npos);
    line.clear();
  }
  size_t line_count = error + warn + info + debug + trace;
  if(line_count != 41)
    throw std::runtime_error("Log should have exactly 40 lines but had " + std::to_string(line_count));
  if(error != 8 || warn != 8 || info != 8 || debug != 8 || trace != 9)
    throw std::runtime_error("Wrong distribution of log messages");
}

}

int main() {
  test::suite suite("logging");

  //check file logging
  suite.test(TEST_CASE(ThreadFileLoggerTest));

  return suite.tear_down();
}