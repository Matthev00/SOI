#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>

struct Superblock {
    uint64_t diskSize;
    uint32_t blockSize;
    uint32_t diskUsage;
    uint32_t fileCount;
    uint32_t freeBlocks;
    time_t lastModified;
};

struct INode {
    time_t creationTime;
    time_t modificationTime;
    uint32_t permissions;
    std::vector<uint32_t> dataBlockPointers;
    uint64_t fileSize;
    bool isDirectory;
    uint32_t referenceCount;
};

const uint32_t totalINodes = 100;

void readSuperblock(const std::string& diskPath, Superblock& superblock) {
    std::ifstream diskFile(diskPath, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file to read superblock.\n";
        return;
    }
    diskFile.read(reinterpret_cast<char*>(&superblock), sizeof(Superblock));
}

void writeSuperblock(const std::string& diskPath, const Superblock& superblock) {
    std::ofstream diskFile(diskPath, std::ios::binary | std::ios::in | std::ios::out);
    if (!diskFile) {
        std::cerr << "Error opening disk file to write superblock.\n";
        return;
    }
    diskFile.write(reinterpret_cast<const char*>(&superblock), sizeof(Superblock));
}

void readINode(const std::string& diskPath, uint32_t iNodeIndex, INode& inode) {
    std::ifstream diskFile(diskPath, std::ios::binary);
    if (!diskFile) {
        std::cerr << "Error opening disk file to read i-node.\n";
        return;
    }
    diskFile.seekg(sizeof(Superblock) + iNodeIndex * sizeof(INode));
    diskFile.read(reinterpret_cast<char*>(&inode), sizeof(INode));
}

void writeINode(const std::string& diskPath, uint32_t iNodeIndex, const INode& inode) {
    std::ofstream diskFile(diskPath, std::ios::binary | std::ios::in | std::ios::out);
    if (!diskFile) {
        std::cerr << "Error opening disk file to write i-node.\n";
        return;
    }
    diskFile.seekp(sizeof(Superblock) + iNodeIndex * sizeof(INode));
    diskFile.write(reinterpret_cast<const char*>(&inode), sizeof(INode));
}

uint32_t findFreeINode(const std::string& diskPath) {
    INode inode;
    for (uint32_t i = 0; i < totalINodes; ++i) {
        readINode(diskPath, i, inode);
        if (inode.referenceCount == 0) {
            return i;
        }
    }
    return -1;
}
