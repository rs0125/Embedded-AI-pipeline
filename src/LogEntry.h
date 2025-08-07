#pragma once
#include <Arduino.h>

class LogEntry {
  public:
    String message;
    String type;
    String timestamp;

    LogEntry(String msg, String typ);
    String serialize() const;

  private:
    String getTimestamp() const;
};
