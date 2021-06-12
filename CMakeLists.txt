cmake_minimum_required(VERSION 3.19)
project(TECFileSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(TECFileSystem main.cpp DiskNode.h DiskNode.cpp
                IMemoryBlock.h MemoryBlock.h
                List.h Node.h
                RAID_5.cpp RAID_5.h TECFS-Disk.h
                tinyxml2.h tinyxml2.cpp)