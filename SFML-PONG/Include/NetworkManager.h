#pragma once

#include <SFML/Network.hpp>
#include <thread>
#include <atomic>
#include <iostream>

class NetworkManager{
public:
    // Starts the server
    bool startServer(unsigned short port);

    // Connects to the server as a client
    bool connectToServer(const std::string& ipAddress, unsigned short port);

    // Checks if the client is connected to the server
    bool isClientConnected();

    // Stops the server thread and cleans up resources
    void stopServer();

    // Sends and receives data (implementation depends on game logic)
    void sendDataToClient(const std::string& data);
    std::string receiveDataFromClient();

    // Client side
    void sendDataToServer(const std::string& data);
    std::string receiveDataFromServer();

private:
    sf::TcpListener listener;    // Listener for incoming connections
    sf::TcpSocket client;        // Client-side socket (used by both server and client)
    sf::TcpSocket server;        // Server-side socket
    std::atomic<bool> clientConnected{ false };  // Flag to check if a client is connected
    std::atomic<bool> listening{ true };         // Flag to control the server thread

    std::thread serverThread;    // Thread for handling non-blocking server connections
};