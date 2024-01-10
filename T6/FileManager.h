#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>

class FileManager {
public:
    void copyFileToVirtualDisk(const std::string& source, const std::string& destination);
    void copyFileToSystem(const std::string& source, const std::string& destination);
    void addBytesToFile(const std::string& filename, size_t bytes);
    void removeBytesFromFile(const std::string& filename, size_t bytes);
    void displayDirectoryInfo(const std::string& path) const;
};

#endif // FILEMANAGER_H
