/*
 * Go UDP Receiver & Logger
 *
 * This script listens for UDP packets on a specified port, prints the received
 * message to the console, and appends it to a log file named "udp_events.log".
 *
 * Instructions:
 * 1. Make sure you have Go installed on your PC (https://golang.org/doc/install).
 * 2. Save this code as `receiver.go`.
 * 3. Open your terminal or command prompt.
 * 4. Navigate to the directory where you saved the file.
 * 5. Run the script using the command: go run receiver.go
 * 6. The script will start listening. Any messages sent from the ESP32 will
 * appear in the console and be saved to `udp_events.log`.
 */

package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"time"
)

// --- Configuration ---
const (
	listenAddr = "0.0.0.0:6969" // Listen on all network interfaces on port 12345
	logFileName = "udp_events.log"
)

func main() {
	// Resolve the UDP address to listen on.
	// The first parameter is the network type ("udp"), and the second is the address.
	addr, err := net.ResolveUDPAddr("udp", listenAddr)
	if err != nil {
		log.Fatalf("Failed to resolve UDP address: %v", err)
	}

	// Create a UDP listener (connection).
	// This binds the program to the specified port.
	conn, err := net.ListenUDP("udp", addr)
	if err != nil {
		log.Fatalf("Failed to listen on UDP address: %v", err)
	}
	// Ensure the connection is closed when the main function exits.
	defer conn.Close()

	fmt.Printf("UDP server listening on %s\n", listenAddr)
	fmt.Printf("Logging received events to %s\n", logFileName)

	// Create a buffer to hold incoming data.
	// UDP packets have a maximum size, so 1024 bytes is usually sufficient.
	buffer := make([]byte, 1024)

	// Open the log file in append mode.
	// os.O_APPEND: Append new data to the file.
	// os.O_CREATE: Create the file if it doesn't exist.
	// os.O_WRONLY: Open the file for writing only.
	// 0644 is the file permission.
	logFile, err := os.OpenFile(logFileName, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatalf("Failed to open log file: %v", err)
	}
	defer logFile.Close()

	// Infinite loop to continuously read from the UDP connection.
	for {
		// Read data from the UDP connection into the buffer.
		// This is a blocking call; it will wait until a packet is received.
		n, remoteAddr, err := conn.ReadFromUDP(buffer)
		if err != nil {
			// If there's an error, log it and continue to the next iteration.
			log.Printf("Error reading from UDP: %v", err)
			continue
		}

		// Convert the received bytes to a string.
		message := string(buffer[:n])

		// Print the received message to the console.
		fmt.Printf("Received from %s: %s\n", remoteAddr, message)

		// Format the log entry with a timestamp.
		logEntry := fmt.Sprintf("%s - From %s: %s\n", time.Now().Format(time.RFC3339), remoteAddr, message)

		// Write the log entry to the file.
		if _, err := logFile.WriteString(logEntry); err != nil {
			log.Printf("Failed to write to log file: %v", err)
		}
	}
}
