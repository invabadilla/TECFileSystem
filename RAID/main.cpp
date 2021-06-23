#include <iostream>
#include "RAID_5.h"
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "tinyxml2.h"
#include "json.hpp"
#include "Huffman.h"
//#include "Tree_Node.h"

using json = nlohmann::json;
int StartListenign(int port){
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "No se pudo crear el Socket. Cerrando..." << endl;
        return -1;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);

    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
    bind(listening, (sockaddr*)&hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    while(true){
        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        int clientSocket = accept(listening, (sockaddr *) &client, &clientSize);

        char host[NI_MAXHOST];      // Nombre del cliente remoto
        char service[NI_MAXSERV];

        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        if (getnameinfo((sockaddr *) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
            cout << host << " conectado " << endl;
        } else {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            cout << host << " conectado al puerto " << ntohs(client.sin_port) << endl;
        }
        char buf[4096];
        memset(buf, 0, 4096);

        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1) {
            cerr << "Error al recibir mensaje. Cerrando..." << endl;
            break;
        }

        if (bytesReceived == 0) {
            cout << "Cliente desconectado " << endl;
            break;
        }

        string messageR = string(buf, 0, bytesReceived);
        json jmessageR = json::parse(messageR);
        string key = jmessageR.value("key", "oops");
        if (key == "key"){

            string message = jmessageR.value("message", "oops");
            string pre = jmessageR.value("pre", "oops");
            string in = jmessageR.value("in", "oops");
            int asccii = 123;
            cout<<pre<<" "<<pre.size()<<endl;
            for (int i = 0; i < pre.size(); i++) {
                if(isalpha(pre.at(i))){
                    cout<<"es alpha"<<endl;
                    continue;
                }
                else{
                    cout<<char(asccii)<<endl;
                    pre[i]=char(asccii);
                    asccii++;
                    if (asccii >= 254){asccii=123;}
                }
            }
            for (int i = 0; i < in.size(); i++) {
                if(isalpha(in.at(i))){
                    cout<<"es alpha"<<endl;
                    continue;
                }
                else{
                    cout<<char(asccii)<<endl;
                    in[i]=char(asccii);
                    asccii++;
                    if (asccii >= 254){asccii=123;}
                }
            }


            cout<<"message: "<<message<<endl;
            cout<<"pre: "<<pre<<endl;
            cout<<"in: "<<in<<endl;

            Tree_Node *root = buildTree(in, pre, 0, message.size()-1);

            int index = -1;
            string strDecode;
            while (index < (int)message.size() - 2) {

                decode(root, index, message, &strDecode);
            }
            cout<<"message: "<<strDecode<<endl;

            //send(clientSocket, message.c_str(), message.size() + 1, 0);
        }

        if (key == "Genetic")
        {
            string message = "";
            send(clientSocket, message.c_str(), message.size() + 1, 0);
        }
        close(clientSocket);
    }

    return 0;
}
int main() {
    RAID_5 *raid = new RAID_5();
    StartListenign(raid->getFirstDisk()->getPort());
    return 0;
}
