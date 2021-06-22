#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include <string>

//! Singleton class used by the client to interface with the memory server, all requests to the memory server are handled by this class
class ServerManager
{

private:
    //singleton stuff
    ServerManager(int PORT);

    static ServerManager *singleton;

    //aquí método para deserealizar json?

    //private socket stuff

    //! variable to store the current connected socket
    int serverSocket;

    //! buffer array that gets reutilized when connecting to server's socket
    char buffer[1024];

    void listenServer();

    void connectSocket();

public:
    //! port number to which it connects
    int PORT;

    //public singleton stuff
    ServerManager(ServerManager &other) = delete;   //delete copy constructor (non-cloneable)
    void operator=(const ServerManager &) = delete; //delete assignment operator (non-assignable)

    static ServerManager *getInstance(int PORT = 9999);

    // Communications stuff
    void sendRequest(int request);

    void sendJson(std::string jsonStr);

    std::string getServerMsg();
};

#endif