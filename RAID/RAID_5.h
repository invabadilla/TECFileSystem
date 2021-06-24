//
// Created by usuario on 6/12/21.
//

#ifndef TECFILESYSTEM_RAID_5_H
#define TECFILESYSTEM_RAID_5_H
#include "DiskNode.h"


using namespace MemPool;
using namespace std;

/**
 * Class to build the RAID
 */
class RAID_5 {
public:
    RAID_5();

    DiskNode *getFirstDisk() const;
    void setFirstDisk(DiskNode *firstDisk);
    void Update_Memory();


private:
    DiskNode *firstDisk = nullptr;


};


#endif //TECFILESYSTEM_RAID_5_H
