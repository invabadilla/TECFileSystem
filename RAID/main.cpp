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
List<string> StoL (string text){
    string insert;
    List<string> result;
    for (int i = 0; i< text.size(); i++) {
        if (text[i]=='$'){
            result.insertLast(insert);
            insert ="";
        }
        else{
            insert.push_back(text[i]);
        }
    }
    return result;
}
json parseJson (List<string> toSend, string key){
    json mymessage =
            {
                    {"key", key},
                    {"message", toSend.find(0)->getValue()},
                    {"pre", toSend.find(1)->getValue()},
                    {"in", toSend.find(2)->getValue()},

            };
    return mymessage;
}

int StartListenign(int port, RAID_5* raid5){
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
        if (key == "save"){

            string message = jmessageR.value("message", "oops");
            string pre = jmessageR.value("pre", "oops");
            string in = jmessageR.value("in", "oops");

            cout<<"message: "<<message<<endl;
            cout<<"pre: "<<pre<<endl;
            cout<<"in: "<<in<<endl;

            List<string> pre_list = StoL(pre);
            List<string> in_list = StoL(in);
            preIndex = 0;
            Tree_Node *root = buildTree(in_list, pre_list, 0, in_list.getSize()-1);

            int index = -1;
            string strDecode;
            while (index < (int)message.size() - 2) {
                decode(root, index, message, &strDecode);
            }
            cout<<"message: "<<strDecode<<endl;
            string messageS = raid5->getFirstDisk()->FindBlockSuitableToHoldMemory(stoi(strDecode));
            List<string> list = buildHuffmanTree(messageS);
            json js = parseJson(list, "save");
            messageS = js.dump();
            cout<<"message: "<<messageS<<endl;
            send(clientSocket, messageS.c_str(), messageS.size() + 1, 0);
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
    StartListenign(raid->getFirstDisk()->getPort(), raid);
    return 0;
}
