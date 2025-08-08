#pragma once
#include <WiFiUdp.h>
#include "LogEntry.h"

class UdpLogger {
  public:
    UdpLogger(const char* remoteIP, int port);
    void begin();
    void send(const LogEntry& entry);

  private:
    const char* remoteIP;
    int port;
    WiFiUDP udp;
};
