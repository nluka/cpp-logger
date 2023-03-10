#ifndef NLUKA_LOGGER_HPP
#define NLUKA_LOGGER_HPP

#include <string>

#define LOGGER_THREADSAFE 1 // 0 = no thread safety for `logger::write` and `logger::flush`

// Simple, threadsafe logging.
namespace logger {

// Sets the pathname of the file to write logs to.
void set_out_pathname(char const *);
// Sets the pathname of the file to write logs to.
void set_out_pathname(std::string const &);

// Sets the character sequence used to separate events. The default is "\n".
void set_delim(char const *);

// When enabled, events will be flushed after each `logger::log`. Off by default.
void set_autoflush(bool);

enum class EventType : uint8_t {
  // Info
  INF = 0,
  // Warning
  WRN,
  // Error
  ERR,
  // Fatal
  FTL,
  // Number of event types
  COUNT,
};

// Logs an event with formatted message. If `LOGGER_THREADSAFE` is non-zero, this operation is threadsafe.
void log(EventType, char const *fmt, ...);

// Flushes all buffered logs. If `LOGGER_THREADSAFE` is non-zero, this operation is threadsafe.
void flush();

} // namespace logger

#endif // NLUKA_LOGGER_HPP
