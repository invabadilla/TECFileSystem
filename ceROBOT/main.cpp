#include "mainwindow.h"

#include <QApplication>


#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include "json.hpp"
#include "ServerManager.h"

using namespace std;


int main(int argc, char *argv[])
{

    ServerManager *manager = ServerManager::getInstance(9999);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
