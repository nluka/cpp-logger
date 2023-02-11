# cpp-logger

Simple, threadsafe logger for C++.

```cpp
int main()
{
  logger::set_out_pathname("mylogs.log");
  logger::set_delim("\n");
  logger::set_autoflush(false);

  // no flush, so this event gets buffered
  logger::log(EventType::INF, "program started successfully");

  try {
    // do something useful
  } catch (char const *const err) {
    logger::log(EventType::FTL, "uh oh... %s", err);
    logger::flush();
    return 1;
  }

  logger::log(EventType::INF, "program finished successfully");
  logger::flush();

  return 0;
}
```
