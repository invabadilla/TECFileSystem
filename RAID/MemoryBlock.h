//
// Created by usuario on 4/10/21.
//

#ifndef SERVER_SMEMORYCHUNK_H
#define SERVER_SMEMORYCHUNK_H

#include "IMemoryBlock.h"
#include <vector>
#include "List.h"
#include <iostream>

using namespace std;
namespace MemPool {

    /**
     * Struct for the creation of each Block of the Disk Node
     */
    typedef struct MemoryBlock {
        string path_block;
        List <string> path_meta;
        TByte *Data;
        size_t DataSize;
        size_t UsedSize;
        bool IsAllocationChunk;
        MemoryBlock *Next;

    } MemoryBlock;
}
#endif //SERVER_SMEMORYCHUNK_H
