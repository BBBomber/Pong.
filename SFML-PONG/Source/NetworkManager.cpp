#include "../Include/NetworkManager.h"

void NetworkManager::stopServer()
{
    listening = false;  // Set flag to false, stopping the server from listening

    if (serverThread.joinable())
    {
        serverThread.join();  // Wait for the server thread to finish
    }

    listener.close();  // Close the listener socket
}

bool NetworkManager::startServer(unsigned short port)
{
    if (listener.listen(port) != sf::Socket::Done)
    {
        std::cerr << "Error starting server on port " << port << std::endl;
        return false;
    }
    std::cout << "Server is listening on port " << port << std::endl;

    listener.setBlocking(false);

    // Start the server thread, using the class member for better lifecycle management
    serverThread = std::thread([this]()
        {
            while (listening)  // Keep listening while the flag is true
            {
                if (listener.accept(client) == sf::Socket::Done)
                {
                    std::cout << "Client connected!" << std::endl;
                    clientConnected = true;  // Set flag indicating a client is connected
                    client.setBlocking(false);
                    break;  // Exit the loop after a successful connection
                }
            }
        });

    return true;
}

bool NetworkManager::connectToServer(const std::string& ipAddress, unsigned short port)
{
    if (client.connect(ipAddress, port) != sf::Socket::Done)
    {
        std::cerr << "Failed to connect to server at " << ipAddress << std::endl;
        return false;
    }
    std::cout << "Connected to server!" << std::endl;
    clientConnected = true;

    client.setBlocking(false);
    return true;
}

bool NetworkManager::isClientConnected()
{
    return clientConnected;
}

void NetworkManager::sendDataToClient(const std::string& data)
{
    std::size_t totalSent = 0;
    while (totalSent < data.size())
    {
        std::size_t sent;
        if (client.send(data.c_str() + totalSent, data.size() - totalSent, sent) != sf::Socket::Done)
        {
            std::cerr << "Error sending data to client" << std::endl;
            break;
        }
        totalSent += sent;
    }
}

std::string NetworkManager::receiveDataFromClient()
{
    char data[100];
    std::size_t received;
    sf::Socket::Status status = client.receive(data, sizeof(data), received);
    if (status == sf::Socket::Done)
    {
        return std::string(data, received);
    }
    else if (status == sf::Socket::NotReady)
    {
        // No data available, return an empty string
        return "";
    }
    else if (status == sf::Socket::Disconnected)
    {
        std::cerr << "Client disconnected" << std::endl;
        clientConnected = false;
        return "";
    }
    return "";
}

void NetworkManager::sendDataToServer(const std::string& data)
{
    if (client.send(data.c_str(), data.size()) != sf::Socket::Done)
    {
        std::cerr << "Error sending data to server" << std::endl;
    }
}

std::string NetworkManager::receiveDataFromServer()
{
    static std::string buffer;  // Buffer to accumulate data
    char data[100];  // Temporary buffer for each receive call
    std::size_t received;
    sf::Socket::Status status = client.receive(data, sizeof(data), received);

    if (status == sf::Socket::Done)
    {
        buffer.append(data, received);  // Accumulate the data in the buffer

        // Debug: Print the buffer to see what's accumulated
        std::cout << "Buffer content: " << buffer << std::endl;

        // Check if a full message has been received (use ';' as a message delimiter)
        size_t pos = buffer.find(';');
        if (pos != std::string::npos)
        {
            std::string message = buffer.substr(0, pos);  // Extract the complete message
            buffer.erase(0, pos + 1);  // Remove the processed message from the buffer
            return message;  // Return the full message
        }
        // If we don't have a full message yet, return an empty string
        return "";
    }
    else if (status == sf::Socket::NotReady)
    {
        // No data available, return an empty string
        return "";
    }
    else if (status == sf::Socket::Disconnected)
    {
        std::cerr << "Disconnected from server" << std::endl;
        clientConnected = false;
        return "";
    }

    std::cerr << "Error receiving data from server" << std::endl;
    return "";
}