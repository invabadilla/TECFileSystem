//
// Created by usuario on 4/10/21.
//

#ifndef SERVER_IMEMORYBLOCK_H
#define SERVER_IMEMORYBLOCK_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <iostream>
#include <fstream>

namespace MemPool{
    typedef unsigned char TByte; //Plantilla para guardar memoria

    /**
     * Clase padre para la creacion del Pool de Memoria
     */
    class IMemoryBlock{
    public:
        virtual ~IMemoryBlock(){};
        virtual void *GetMemory(const size_t &sMemorySize) = 0;
        virtual void FreeMemory(void *ptrMemoryBlock)= 0;
    };
}

#endif //SERVER_IMEMORYBLOCK_H
