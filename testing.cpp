#include <array>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <thread>

#include "logger.hpp"

int main() {
  using logger::EventType;

  std::string const logFilePathname("testing.log");
  size_t const numLogsPerEventType = 100;

  logger::set_out_pathname(logFilePathname);
  logger::set_delim("\n");

  auto const logTask = [](EventType const evType) {
    std::ostringstream oss{};
    oss << std::this_thread::get_id();
    std::string const threadId = oss.str();

    for (size_t i = 1; i <= numLogsPerEventType; ++i) {
      logger::log(
        evType,
        "message %zu from thread %s",
        i, threadId.c_str()
      );
    }
  };

  // populate log file
  #if LOGGER_THREADSAFE
  {
    // enable autoflushing so we can test the locking mechanism
    logger::set_autoflush(true);

    // create a separate thread for each event type
    std::array<std::thread, static_cast<size_t>(EventType::COUNT)> threads{
      std::thread(logTask, EventType::INF),
      std::thread(logTask, EventType::WRN),
      std::thread(logTask, EventType::ERR),
      std::thread(logTask, EventType::FTL)
    };

    // join threads
    for (auto &t : threads) {
      t.join();
    }
  }
  #else // not LOGGER_THREADSAFE
  {
    logTask(EventType::INF);
    logTask(EventType::WRN);
    logTask(EventType::ERR);
    logTask(EventType::FTL);
  }
  #endif // LOGGER_THREADSAFE

  // do this a few times to test flushing an empty event buffer
  logger::flush();
  logger::flush();
  logger::flush();

  size_t lineCount = 0, numBadLines = 0;
  {
    std::ifstream logFile(logFilePathname);
    if (!logFile.is_open())
      std::exit(1);
    if (!logFile.good())
      std::exit(2);

    std::regex const logRegex(
      "\\[((INFO)|(WARN)|(ERR)|(FATAL))\\] " // event type
      "\\([0-9]{4}-[0-9]{1,2}-[0-9]{1,2} "        // date
      "[0-9]{1,2}:[0-9]{2}:[0-9]{2}\\) "          // time
      "message [0-9]+ from thread [0-9]+"         // message
    );

    std::string line{};
    while (std::getline(logFile, line)) {
      ++lineCount;
      if (!std::regex_match(line, logRegex)) {
        ++numBadLines;
      }
    }
  }

  std::cout << "numLogsPerEventType = " << numLogsPerEventType << '\n';
  std::cout << "lineCount = " << lineCount << '\n';
  std::cout << "numBadLines = " << numBadLines << '\n';

  bool const pass =
    lineCount == numLogsPerEventType * static_cast<size_t>(EventType::COUNT)
    && numBadLines == 0
  ;

  std::cout << "result: " << (pass ? "PASS" : "FAIL") << '\n';

  return pass ? 0 : -1;
}
