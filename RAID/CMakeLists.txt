cmake_minimum_required(VERSION 3.17)
project(TECFileSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(TECFileSystem main.cpp
                IMemoryBlock.h
                DiskNode.h DiskNode.cpp
                MemoryBlock.h
                List.h Node.h
                RAID_5.h RAID_5.cpp TECFS-Disk.h
                tinyxml2.h tinyxml2.cpp)