//
// Created by usuario on 4/10/21.
//

#include "DiskNode.h"
#include "MemoryBlock.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "List.h"
#include <filesystem>

using namespace filesystem;
namespace MemPool {
    static const int FREEED_MEMORY_CONTENT = 0xAA;
    static const int NEW_ALLOCATED_MEMORY_CONTENT = 0xFF;

    /**
     * Constructor of the Disk Nodes
     * @param sInitialMemoryPoolSize Total memory of the Disk
     * @param sMemoryChunkSize  Size of each Block
     * @param sMinimalMemorySizeToAllocate Minimum memory space that can be requested
     * @param bSetMemoryData Bool to set a defined space for the memset
     */
    DiskNode::DiskNode(const size_t &sInitialMemoryPoolSize, const size_t &sMemoryChunkSize,
                       const size_t &sMinimalMemorySizeToAllocate, bool bSetMemoryData) {

        m_ptrFirstChunk = NULL;
        m_ptrLastChunk = NULL;
        m_ptrCursorChunk = NULL;

        m_sTotalMemoryPoolSize = 0;
        m_sUsedMemoryPoolSize = 0;
        m_sFreeMemoryPoolSize = 0;

        m_sMemoryChunkSize = sMemoryChunkSize;
        m_uiMemoryChunkCount = 0;
        m_uiObjectCount = 0;

        m_bSetMemoryData = bSetMemoryData;
        AllocateMemory(sInitialMemoryPoolSize);

    }

    /**
     * Destructor of the Disk Node
     */
    DiskNode::~DiskNode() {
        FreeAllAllocatedMemory();
        DeallocateAllChunks();
    }

    /**
     * Function to request memory spaces in the Disk
     * @param sMemorySize Size of memory space requested
     * @return
     */
    void *DiskNode::GetMemory(const std::size_t &sMemorySize) {
        std::size_t sBestMemBlockSize = CalculateBestMemoryBlockSize(sMemorySize);
        MemoryBlock *ptrChunk = NULL;
        while (!ptrChunk) {
            ptrChunk = FindChunkSuitableToHoldMemory(sBestMemBlockSize);
            if (!ptrChunk) {
                break;
            }
        }

        m_sUsedMemoryPoolSize += sBestMemBlockSize;
        m_sFreeMemoryPoolSize -= sBestMemBlockSize;
        m_uiObjectCount++;
        if (ptrChunk != NULL) {
            SetMemoryChunkValues(ptrChunk, sBestMemBlockSize);
            return ((void *) ptrChunk->Data);
        } else { return NULL; }
    }

    /**
     * Function to free up certain memory spaces
     * @param ptrMemoryBlock
     */
    void DiskNode::FreeMemory(void *ptrMemoryBlock) {
        MemoryBlock *ptrChunk = FindChunkHoldingPointerTo(ptrMemoryBlock);
        std::cout << ptrChunk << std::endl;
        if (ptrChunk) {
            FreeChunks(ptrChunk);
        } else {
            assert(false && "ERROR : Requested Pointer not in Memory Pool");
        }
        assert((m_uiObjectCount > 0) && "ERROR : Request to delete more Memory then allocated.");
        m_uiObjectCount--;
    }

    /**
     * Function that performs the allocation of the Memory Pool
     * @param sMemorySize Size of the Disk
     * @return
     */
    bool DiskNode::AllocateMemory(const std::size_t &sMemorySize) {
        unsigned int uiNeededChunks = CalculateNeededChunks(sMemorySize);
        std::size_t sBestMemBlockSize = CalculateBestMemoryBlockSize(sMemorySize);

        TByte *ptrNewMemBlock = (TByte *) malloc(sBestMemBlockSize); // allocate from Operating System
        MemoryBlock *ptrNewChunks = (MemoryBlock *) malloc(
                (uiNeededChunks * sizeof(MemoryBlock))); // allocate Chunk-Array to Manage the Memory
        assert(((ptrNewMemBlock) && (ptrNewChunks)) && "Error : System ran out of Memory");

        m_sTotalMemoryPoolSize += sBestMemBlockSize;
        m_sFreeMemoryPoolSize += sBestMemBlockSize;
        m_uiMemoryChunkCount += uiNeededChunks;

        if (m_bSetMemoryData) {
            memset(((void *) ptrNewMemBlock), NEW_ALLOCATED_MEMORY_CONTENT, sBestMemBlockSize);
        }

        return LinkChunksToData(ptrNewChunks, uiNeededChunks, ptrNewMemBlock);;
    }

    /**
     * Performs the calculation of the number of Blocks for each variable memory space
     * @param sMemorySize Required memory space
     * @return
     */
    unsigned int DiskNode::CalculateNeededChunks(const std::size_t &sMemorySize) {
        float f = (float) (((float) sMemorySize) / ((float) m_sMemoryChunkSize));
        return ((unsigned int) ceil(f));
    }

    /**
     * Calculate the best space of the Chunks to fill the Disk
     * @param sRequestedMemoryBlockSize Size del Pool
     * @return
     */
    std::size_t DiskNode::CalculateBestMemoryBlockSize(const std::size_t &sRequestedMemoryBlockSize) {
        unsigned int uiNeededChunks = CalculateNeededChunks(sRequestedMemoryBlockSize);
        return std::size_t((uiNeededChunks * m_sMemoryChunkSize));
    }

    /**
     * Free the memory of all Blocks
     * @param ptrChunk
     */
    void DiskNode::FreeChunks(MemoryBlock *ptrChunk) {
        MemoryBlock *ptrCurrentChunk = ptrChunk;
        unsigned int uiChunkCount = CalculateNeededChunks(ptrCurrentChunk->UsedSize);
        for (unsigned int i = 0; i < uiChunkCount; i++) {
            if (ptrCurrentChunk) {
                if (m_bSetMemoryData) {
                    memset(((void *) ptrCurrentChunk->Data), FREEED_MEMORY_CONTENT, m_sMemoryChunkSize);
                }
                ptrCurrentChunk->UsedSize = 0;
                m_sUsedMemoryPoolSize -= m_sMemoryChunkSize;
                ptrCurrentChunk = ptrCurrentChunk->Next;
            }
        }
    }


    /**
     * Search Blocks available to store data
     * @param sMemorySize Size required of the variable
     * @return Block available
     */
    MemoryBlock *DiskNode::FindChunkSuitableToHoldMemory(const std::size_t &sMemorySize) {
        unsigned int uiChunksToSkip = 0;
        bool bContinueSearch = true;
        MemoryBlock *ptrChunk = m_ptrCursorChunk;
        for (unsigned int i = 0; i < m_uiMemoryChunkCount; i++) {
            if (ptrChunk) {
                if (ptrChunk == m_ptrLastChunk) {
                    ptrChunk = m_ptrFirstChunk;
                }
                if (ptrChunk->DataSize >= sMemorySize) {
                    if (ptrChunk->UsedSize == 0) {
                        m_ptrCursorChunk = ptrChunk;
                        return ptrChunk;
                    }
                }
                uiChunksToSkip = CalculateNeededChunks(ptrChunk->UsedSize);
                if (uiChunksToSkip == 0) uiChunksToSkip = 1;
                ptrChunk = SkipChunks(ptrChunk, uiChunksToSkip);
            } else {
                bContinueSearch = false;
            }
        }
        return NULL;
    }

    /**
     * Discard the Blocks that are reserved
     * @param ptrStartChunk initial block
     * @param uiChunksToSkip Number of blocks to skip
     * @return Next block Available
     */
    MemoryBlock *DiskNode::SkipChunks(MemoryBlock *ptrStartChunk, unsigned int uiChunksToSkip) {
        MemoryBlock *ptrCurrentChunk = ptrStartChunk;
        for (unsigned int i = 0; i < uiChunksToSkip; i++) {
            if (ptrCurrentChunk) {
                ptrCurrentChunk = ptrCurrentChunk->Next;
            } else {
                assert(false && "Error : Chunk == NULL was not expected.");
                break;
            }
        }
        return ptrCurrentChunk;
    }

    /**
     * Reset used memory
     * @param ptrChunk Block to reset
     * @param sMemBlockSize used space
     */
    void DiskNode::SetMemoryChunkValues(MemoryBlock *ptrChunk, const std::size_t &sMemBlockSize) {
        if ((ptrChunk)) {
            ptrChunk->UsedSize = sMemBlockSize;
        }
        /*else{
            //assert(false && "Error : Invalid NULL-Pointer passed") ;
            continue;
        }*/
    }

    /**
     * Function that links the designated Chunk to each variable and its corresponding space
     * @param ptrNewChunks Pointer to Designated blocks
     * @param uiChunkCount Block amount of memory block
     * @param ptrNewMemBlock Pointer to allocated memory space
     * @return Funcion RecalcChunkMemorySize
     */
    bool DiskNode::LinkChunksToData(MemoryBlock *ptrNewChunks, unsigned int uiChunkCount, TByte *ptrNewMemBlock) {
        MemoryBlock *ptrNewChunk = NULL;
        unsigned int uiMemOffSet = 0;
        bool bAllocationChunkAssigned = false;
        for (unsigned int i = 0; i < uiChunkCount; i++) {
            if (!m_ptrFirstChunk) {
                m_ptrFirstChunk = SetChunkDefaults(&(ptrNewChunks[0]));

                m_ptrLastChunk = m_ptrFirstChunk;
                m_ptrCursorChunk = m_ptrFirstChunk;
            } else {
                ptrNewChunk = SetChunkDefaults(&(ptrNewChunks[i]));
                m_ptrLastChunk->Next = ptrNewChunk;
                m_ptrLastChunk = ptrNewChunk;
            }

            uiMemOffSet = (i * ((unsigned int) m_sMemoryChunkSize));
            m_ptrLastChunk->Data = &(ptrNewMemBlock[uiMemOffSet]);

            if (!bAllocationChunkAssigned) {
                m_ptrLastChunk->IsAllocationChunk = true;
                bAllocationChunkAssigned = true;
            }
        }

        return RecalcChunkMemorySize(m_ptrFirstChunk, m_uiMemoryChunkCount);
    }

    /**
     * Function that determines if the number of available
     * blocks are sufficient for the requested memory space
     * @param ptrChunk Pointer to the first available block
     * @param uiChunkCount Amount of blocks required
     * @return bool True if possible, False if the required chunks do not exist
     */
    bool DiskNode::RecalcChunkMemorySize(MemoryBlock *ptrChunk, unsigned int uiChunkCount) {
        unsigned int uiMemOffSet = 0;
        for (unsigned int i = 0; i < uiChunkCount; i++) {
            if (ptrChunk) {
                uiMemOffSet = (i * ((unsigned int) m_sMemoryChunkSize));
                ptrChunk->DataSize = (((unsigned int) m_sTotalMemoryPoolSize) - uiMemOffSet);
                ptrChunk = ptrChunk->Next;
            } else {
                assert(false && "Error : ptrChunk == NULL");
                return false;
            }
        }
        return true;
    }

    /**
     * Reset the blocks at the start to the default values for their use
     * @param ptrChunk pointer to reset
     * @return reset pointer
     */
    MemoryBlock *DiskNode::SetChunkDefaults(MemoryBlock *ptrChunk) {
        if (ptrChunk) {
            ptrChunk->DataSize = 0;
            ptrChunk->UsedSize = 0;
            ptrChunk->Next = NULL;
            ptrChunk->path_block = "DEFAULT";
        }
        return ptrChunk;
    }

    /**
     * Reset blocks to default values for reuse
     * @param ptrChunk pointer to reset
     */
    void DiskNode::SetChunktoDefault(MemoryBlock *ptrChunk) {
        if (ptrChunk) {
            ptrChunk->UsedSize = 0;
            ptrChunk->IsAllocationChunk = false;
        }
    }

    /**
     * Search blocks of a memory address of Data
     * @param ptrMemoryBlock Direction of the Data
     * @return block
     */
    MemoryBlock *DiskNode::FindChunkHoldingPointerTo(void *ptrMemoryBlock) {
        MemoryBlock *ptrTempChunk = m_ptrFirstChunk;
        while (ptrTempChunk) {
            if (ptrTempChunk->Data == ((TByte *) ptrMemoryBlock)) {
                break;
            }

            ptrTempChunk = ptrTempChunk->Next;
        }
        return ptrTempChunk;
    }

    /**
     * Reset the memory of the blocks when executing the Destroyer
     */
    void DiskNode::FreeAllAllocatedMemory() {
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        while (ptrChunk) {
            SetChunktoDefault(ptrChunk);
            ptrChunk = ptrChunk->Next;
        }
        m_ptrCursorChunk = m_ptrFirstChunk;
    }

    /**
     * Free the memory of the blocks when executing the Destroyer
     */
    void DiskNode::DeallocateAllChunks() {
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        MemoryBlock *ptrChunkToDelete = NULL;
        while (ptrChunk) {
            if (ptrChunk->IsAllocationChunk) {
                if (ptrChunkToDelete) {
                    free(((void *) ptrChunkToDelete));
                }
                ptrChunkToDelete = ptrChunk;
            }
            ptrChunk = ptrChunk->Next;
        }
    }

    DiskNode *DiskNode::getNext() const {
        return Next;
    }

    void DiskNode::setNext(DiskNode *next) {
        Next = next;
    }

    void DiskNode::SetParameters(string ip_, int port_, string path_, List<string> path_blocks) {
        ip = ip_;
        port = port_;
        path = path_;
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        int i = 0;
        while (ptrChunk) {
            string path_block_ = path_blocks.find(i)->getValue();
            ptrChunk->path_block = path_block_;
            ptrChunk = ptrChunk->Next;
            i++;
        }
    }

    string DiskNode::GetParameters() {
        string result = ip + " - " + to_string(port) + " - " + path + " - Blocks: ";
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        int i = 0;
        while (ptrChunk) {
            result += ptrChunk->path_block + " - ";
            ptrChunk = ptrChunk->Next;
            i++;
        }
        return result;
    }

    int DiskNode::getPort() const {
        return port;
    }

    /**
     * Look for the block to store the information
     * @param size to store
     * @return Path of the disk and the number of the block
     */
    string DiskNode::FindBlockSuitableToHoldMemory(int size) {
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        int i =0;
        while (ptrChunk){
            if (ptrChunk->UsedSize+size < ptrChunk->DataSize ){
                ptrChunk->UsedSize += size;
                string temp = path;
                temp.pop_back();
                return temp +"#"+to_string(i);
            }
            else{
                ptrChunk = ptrChunk->Next;
                i++;
            }
        }

    }

    /**
     * Parse strong to list
     * @param text to parse
     * @param char_ to limit
     * @return the result list
     */
    List<string> StoL (string text, char char_){
        string insert;
        List<string> result;
        for (int i = 0; i < text.size(); i++) {
            if (text[i]==char_){
                result.insertLast(insert);
                insert ="";
            }
            else{
                insert.push_back(text[i]);
            }
        }
        if (char_ != '$'){
            result.insertLast(insert);
        }
        return result;
    }

    /**
     * search for files matching a name
     * @param name to search
     * @return Path of the disk and the number of the block
     */
    string DiskNode::FindNameinBlock(string name){
        MemoryBlock *ptrChunk = m_ptrFirstChunk;
        int i =0;
        while (ptrChunk){
            for(auto& p_: directory_iterator(ptrChunk->path_block)){
                List<string> result = StoL(p_.path(), '/');
                string name_ = result.find(result.getSize()-1)->getValue();
                name_.erase(name_.end()-5, name_.end());
                cout << name_ << '\n';
                if (name == name_){
                    string temp = path;
                    temp.pop_back();
                    return temp +"#"+to_string(i);
                }
            }
            ptrChunk = ptrChunk->Next;
            i++;
        }
    }
    MemoryBlock *DiskNode::getMPtrFirstChunk() const {
        return m_ptrFirstChunk;
    }

}
