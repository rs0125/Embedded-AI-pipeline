#include "UdpLogger.h"

UdpLogger::UdpLogger(const char* ip, int p) : remoteIP(ip), port(p) {}

void UdpLogger::begin() {
  udp.begin(6969); // local port
}

void UdpLogger::send(const LogEntry& entry) {
  String payload = entry.serialize();
  udp.beginPacket(remoteIP, port);
  udp.print(payload);
  udp.endPacket();
}
