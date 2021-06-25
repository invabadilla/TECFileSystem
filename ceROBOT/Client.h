//
// Created by Usuario on 22/6/2021.
//

#ifndef CONTROLLERNODE_CLIENT_H
#define CONTROLLERNODE_CLIENT_H
#include <string>

//! Singleton class used by the client to interface with the memory server, all requests to the memory server are handled by this class
class Client {
private:
    //singleton stuff
    Client(int PORT);

    static Client *singleton;

    //private socket stuff

    //! variable to store the current connected socket
    int serverSocket;

    //! buffer array that gets reutilized when connecting to server's socket
    char buffer[8192];

    void listenServer();

    void connectSocket();

public:
    //! port number to which it connects
    int PORT;

    //public singleton stuff
    Client(Client &other) = delete;   //delete copy constructor (non-cloneable)
    void operator=(const Client &) = delete; //delete assignment operator (non-assignable)

    static Client *getInstance(int PORT = 9999);

    // Communications stuff
    void sendRequest(int request);

    std::string sendJson(std::string jsonStr);

    std::string getServerMsg();
};

#endif //CONTROLLERNODE_CLIENT_H
