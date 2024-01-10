#ifndef VIRTUALDISKMANAGER_H
#define VIRTUALDISKMANAGER_H

#include <string>

class VirtualDiskManager {
public:
    void createVirtualDisk(const std::string& diskName, uint32_t blockSize, uint32_t totalBlocks);
    void createDirectory(const std::string& path);
    void removeDirectory(const std::string& path);
    void createHardLink(const std::string& source, const std::string& linkName);
    void removeLink(const std::string& linkName);
    void displayDiskUsage() const;
    void removeFile(const std::string& fileName);
};

#endif // VIRTUALDISKMANAGER_H