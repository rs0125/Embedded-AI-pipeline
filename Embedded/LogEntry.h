#pragma once
#include <Arduino.h>

class LogEntry {
  public:
    String message;
    String type;
    String timestamp;

    LogEntry(String msg, String typ) {
      message = msg;
      type = typ;
      timestamp = getTimestamp();
    }

    String serialize() const {
      // Could be JSON or CSV — here's JSON
      return "{\"msg\":\"" + message + "\",\"type\":\"" + type + "\",\"time\":\"" + timestamp + "\"}";
    }

  private:
    String getTimestamp() const {
      time_t now;
      struct tm timeinfo;
      if (!getLocalTime(&timeinfo)) {
        return "unknown";
      }
      char buf[30];
      strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", &timeinfo);
      return String(buf);
    }
};
