//
// Created by usuario on 6/12/21.
//

#include "RAID_5.h"
#include "tinyxml2.h"
#include "TECFS-Disk.h"

RAID_5::RAID_5() {
    DiskNode *node0 = new DiskNode(40960, 10240, 1, true);
    DiskNode *node1 = new DiskNode(40960, 10240, 1, true);
    DiskNode *node2 = new DiskNode(40960, 10240, 1, true);
    DiskNode *node3 = new DiskNode(40960, 10240, 1, true);

    TECFSDisk(node0);
    setFirstDisk(node0);

    TECFSDisk(node1);
    node0->setNext(node1);

    TECFSDisk(node2);
    node1->setNext(node2);

    TECFSDisk(node3);
    node2->setNext(node3);

    DiskNode *node = getFirstDisk();
    for (int i = 0; i < 4; i++){
        cout << node->GetParameters() <<endl;
        node = node->getNext();
    }
}

DiskNode *RAID_5::getFirstDisk() const {
    return firstDisk;
}

void RAID_5::setFirstDisk(DiskNode *firstDisk) {
    RAID_5::firstDisk = firstDisk;
}

