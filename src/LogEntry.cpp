#include "LogEntry.h"

LogEntry::LogEntry(String msg, String typ) {
  message = msg;
  type = typ;
  timestamp = getTimestamp();
}

String LogEntry::serialize() const {
  return "{\"msg\":\"" + message + "\",\"type\":\"" + type + "\",\"time\":\"" + timestamp + "\"}";
}

String LogEntry::getTimestamp() const {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "unknown";
  }
  char buf[30];
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  return String(buf);
}
