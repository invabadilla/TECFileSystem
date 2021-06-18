//
// Created by usuario on 6/12/21.
//

#ifndef TECFILESYSTEM_TECFS_DISK_H
#define TECFILESYSTEM_TECFS_DISK_H

#endif //TECFILESYSTEM_TECFS_DISK_H

#include "DiskNode.h"
#include "tinyxml2.h"
#include <filesystem>

using namespace tinyxml2;
using namespace filesystem;
int iteration =1;

void TECFSDisk(DiskNode *node){

    XMLDocument xml_doc;
    path temp = current_path().parent_path();
    string dn =temp.string()+"/Parameters_TECFS_Disk.xml"; const char * document_name = dn.c_str();
    XMLError eResult = xml_doc.LoadFile(document_name);
    XMLNode* root = xml_doc.FirstChildElement("Parameters");
    XMLElement *ip = root->FirstChildElement("ip");
    XMLElement *port = root->FirstChildElement("port");
    XMLElement *path = root->FirstChildElement("path");
    string ip_ = ip->GetText();
    string port_ = port->GetText();
    string path_ = path->GetText();
    path_ += "/DiskNode"+ to_string(iteration);
    create_directory(path_);
    List<string> path_blocks;
    for (int i = 0; i < 4; i++){
        string path_block_ = path_ + "/Block" + to_string(i);
        create_directory(path_block_);
        path_blocks.insertLast(path_block_);
    }
    node->SetParameters(ip_, stoi(port_), path_, path_blocks);

    iteration +=1;
}