//
// Created by usuario on 4/10/21.
//

#ifndef SERVER_SMEMORYCHUNK_H
#define SERVER_SMEMORYCHUNK_H

#include "IMemoryBlock.h"
#include <vector>
#include "List.h"
#include <iostream>

namespace MemPool {

    /**
     * Struct para la creacion de cada Chunk del Pool de memoria
     */
    typedef struct MemoryBlock {
        std::string path_block;
        List <std::string> path_meta;
        TByte *Data;
        size_t DataSize;
        size_t UsedSize;
        bool IsAllocationChunk;
        MemoryBlock *Next;
    } MemoryBlock;
}
#endif //SERVER_SMEMORYCHUNK_H
