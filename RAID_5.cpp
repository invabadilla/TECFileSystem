//
// Created by usuario on 6/12/21.
//

#include "RAID_5.h"
#include "tinyxml2.h"
#include "TECFS-Disk.h"

RAID_5::RAID_5() {
    for (int i = 0; i < 4; i++){
        if (firstDisk == nullptr){
            DiskNode *node = new DiskNode(4096, 1024, 1, true);
            TECFSDisk(node);
            setFirstDisk(node);
            cout << node->GetParameters() <<endl;
        }
        else{
            DiskNode *node = new DiskNode(4096, 1024, 1, true);
            TECFSDisk(node);
            node->setNext(firstDisk);
            setFirstDisk(node);
        }
    }
}

DiskNode *RAID_5::getFirstDisk() const {
    return firstDisk;
}

void RAID_5::setFirstDisk(DiskNode *firstDisk) {
    RAID_5::firstDisk = firstDisk;
}

