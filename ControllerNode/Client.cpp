//
// Created by Usuario on 22/6/2021.
//

#include "Client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Client *Client::singleton = nullptr;

/*!
 * \brief Constructor for Client, should only be called once due to being a singleton
 *
 * \param PORT where it'll connect with server
 */
Client::Client(int PORT)
{
    this->PORT = PORT;
}

/*!
 * \brief method to get (or initialize for the first time) the singleton instance
 *
 * \param PORT where it'll connect with server
 * \return Client* singleton instance
 */
Client *Client::getInstance(int PORT /*= 9999*/) //Default port so it doesn't always need one when classes just want to get the singleton and not initialize it
{
    if (singleton == nullptr)
    {
        singleton = new Client(PORT);
    }
    return singleton;
}

/*!
 * \brief Create a new socket connection to server in order to be able to send a new message
 *
 */
void Client::connectSocket()
{
    //Create new socket
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return;
    }

    //Configure environment for address
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(this->PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    // Connect to the server's socket
    if (connect(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("\nConnection Failed \n");
        return;
    }

    // Clear buffer
    memset(this->buffer, 0, 1024);
}

/*!
 * \brief Sends an int as a request to the server so it knows which protocol to follow
 *
 * \param request int as specified by the server/RequestConstants.h file
 */
void Client::sendRequest(int request)
{
    connectSocket();
    //printf("Sending request: %d\n", request);

    std::string msg = std::to_string(request);

    send(this->serverSocket, msg.c_str(), msg.length(), 0);
}

/*!
 * \brief Sends any type of string to the server but it should mostly expect json encoded strings
 *
 * \param jsonStr the message to be sent, possibly encoded in json format
 */
void Client::sendJson(std::string jsonStr)
{
    connectSocket();
    //printf("Sending: %s\n", jsonStr.c_str());

    std::string msg = jsonStr;

    send(this->serverSocket, msg.c_str(), msg.length(), 0);

    std::string messageR = getServerMsg();
}

//! Essentially executes a read of the socket, call only if expecting the server to send a message back after a request or during a protocol
void Client::listenServer()
{
    read(this->serverSocket, this->buffer, 1024);
    //printf("Received: '%s'\n", this->buffer);
}

//! Calls listenServer to get a message from the server and returns it as an std::string, call only if expecting the server to send a message back after a request or during a protocol
std::string Client::getServerMsg()
{
    listenServer();
    std::string msg = std::string(this->buffer);
    return msg;
}