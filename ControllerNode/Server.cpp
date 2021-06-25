#include "Server.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/*!
 * \brief Construct a new Server object, and configures the socket
 *
 * \param PORT int that indicates de port number
 * \param size int that indicates how much memory the server is going to reserve
 */
Server::Server(int PORT)
{

    //Socket stuff

    struct sockaddr_in address;

    int serverBind = 0;
    int opt = 1;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // configurar el ambiente para el sockaddr structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //assinging the address of local machine
    address.sin_port = htons(PORT);

    // binding to port
    while (true)
    {
        serverBind = bind(serverSocket, (sockaddr *)&address, sizeof(sockaddr));

        if (serverBind < 0)
        {
            printf("Port number: %d\nFailed to bind to local port\n", PORT);
            PORT--;
            address.sin_port = htons(PORT);
        }

        else
        {
            printf("Port number: %d\nSuccessfully bind to local port\n", PORT);
            break;
        }

    }

    // set socket to an especific port
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    //listen
    serverBind = listen(serverSocket, 1);

    if (serverBind < 0)
    {
        printf("Fallo al iniciar la escucha del puerto ");
    }

    else
    {
        printf("Escuchando el puerto local");
    }

    this->request(address, serverSocket);
}

//! Destroy the MServer::MServer object, the destructor is defined to make sure that the memory allocated with the initial malloc is freed
Server::~Server()
{
}

/*!
 * \brief accepts and reads all the incoming messages from the client
 *
 * \param address indicates de IP address
 * \param serverSocket indicates the serverSocket
 */
void Server::request(sockaddr_in address, int serverSocket)
{
    int newSocket = 0;
    char buffer[1024] = {0};
    bool listening = true;
    while (listening)
    {
        readSocket(buffer, address, serverSocket, newSocket);


        if (strcmp(buffer, "OFF") == 0)
        {
            listening = false;
            return;
        }
        if (strcmp(buffer, "save") == 0){
            //readSocket(buffer, address, serverSocket, newSocket);
            std::cout << buffer << std::endl;



            //insertar todo para el save
            //devolver bool de si se realizo exitosamente
            std::string info = "mensaje del server: save";
            send(newSocket, info.c_str(), info.length() + 1, 0);
        } else if (strcmp(buffer, "find") == 0){
            //readSocket(buffer, address, serverSocket, newSocket);
            //insertar todo para el find
            //devolver string de los datos, mostrarlo en la interfaz
            std::cout << buffer << std::endl;
            std::string info = "mensaje del server: find";
            send(newSocket, info.c_str(), info.length() + 1, 0);
        }


        //send(newSocket, hello, strlen(hello), 0);
    }
}

/*!
 * \brief Wait and read a message incoming in the server socket
 *
 * \param buffer a char* buffer to store the contents read
 * \param address indicates de IP address
 * \param serverSocket indicates the serverSocket
 * \param newSocket the socket that will be accepted and read
 */
void Server::readSocket(char *buffer, sockaddr_in address, int serverSocket, int &newSocket)
{
    int addressLen = sizeof(address);
    memset(buffer, 0, 1024); // Clear buffer

    // accept
    newSocket = accept(serverSocket, (sockaddr *)&address, (socklen_t *)&addressLen);

    if (newSocket < 0)
    {
        printf("Fallo al aceptar");
    }
    else
    {
        printf("Aceptado");
    }

    read(newSocket, buffer, 1024);
}

