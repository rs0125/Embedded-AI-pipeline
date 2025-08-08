#include <WiFi.h>
#include "LogEntry.h"
#include "UdpLogger.h"

// --- WiFi Credentials ---
const char* ssid = "MyLinuxHotspot";
const char* password = "mysecurepass";

// --- UDP Logger ---
const char* remoteIP = "10.42.0.255";
const int remotePort = 6969;
UdpLogger logger(remoteIP, remotePort);

// --- Function Prototypes ---
void connectToWiFi();
void setupTime();

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  setupTime();

  logger.begin();

  logger.send(LogEntry("Device Booted", "boot"));
}

void loop() {
  logger.send(LogEntry("Heartbeat", "event"));
  delay(1000);
}

// --- WiFi Helper ---
void connectToWiFi() {
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries++ < 20) {
    delay(500);
  }
  if (WiFi.status() != WL_CONNECTED) {
    logger.send(LogEntry("WiFi failed", "error"));
    ESP.restart();
  }
  Serial.println("WiFi connected: " + WiFi.localIP().toString());
}

// --- Time Setup Helper ---
void setupTime() {
  configTime(19800, 0, "pool.ntp.org");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(500);
  }
}
