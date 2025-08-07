#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// --- Configuration ---
const char* ssid = "MyLinuxHotspot";         // Your Wi-Fi network SSID
const char* password = "mysecurepass"; // Your Wi-Fi network password

const char* udpAddress = "10.42.0.255"; // IP address of the PC running the Go receiver
const int udpPort = 6969;                // Port to send UDP packets to

// --- Global Objects ---
WiFiUDP udp;
unsigned int counter = 0;

void connectToWiFi();

void setup() {
  // Start Serial for debugging
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting ESP32 UDP Sender...");

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize UDP
  // The begin() function for WiFiUDP initializes the UDP communication.
  // It's not strictly necessary to call it on the sender side in this simple case,
  // but it's good practice.
  udp.begin(udpPort);
  Serial.println("UDP Sender Initialized.");
}

void loop() {
  // Create the message string
  String message = "Message from ESP32, count: " + String(counter);
  
  // Send the UDP packet
  // beginPacket() sets up the destination IP and port for the packet.
  udp.beginPacket(udpAddress, udpPort);
  
  // write() or print() sends the data.
  udp.print(message);
  
  // endPacket() actually sends the packet over the network.
  if (udp.endPacket()) {
    Serial.print("Sent packet: ");
    Serial.println(message);
    counter++;
  } else {
    Serial.println("Error sending packet.");
  }

  // Wait for a second before sending the next packet
  delay(1000);
}

// --- Helper Functions ---
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    attempts++;
    if (attempts > 20) { // Timeout after 10 seconds
      Serial.println("\nFailed to connect to WiFi. Please check credentials and network.");
      // You might want to add logic here to handle the failure, like rebooting.
      ESP.restart();
    }
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
