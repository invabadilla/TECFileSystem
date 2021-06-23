#ifndef CONTROLLERNODE_SERVER_H
#define CONTROLLERNODE_SERVER_H


#include <netinet/in.h>
#include <string>


//! Class that creates the server socket and listen the client
class Server {

public:
    Server(int PORT);
    ~Server();

    void request(sockaddr_in address, int serverSocket);
    void readSocket(char *bufferOut, sockaddr_in address, int serverSocket, int &newSocket);

};

#endif //CONTROLLERNODE_SERVER_H