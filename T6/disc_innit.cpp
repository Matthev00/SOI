#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

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
    std::vector<uint32_t> dataBlockPointers; // Wektory s¹ elastyczne, ale rozmiar mo¿e byæ sta³y w zale¿noœci od wymagañ
    uint64_t fileSize;
    bool isDirectory;
    uint32_t referenceCount;
};

void initializeVirtualDisk(const std::string& diskName, uint32_t blockSize, uint32_t totalBlocks) {
    std::ofstream diskFile(diskName, std::ios::binary | std::ios::out);

    if (!diskFile) {
        std::cerr << "File can not be opened!" << std::endl;
        return;
    }

    // Inicjalizacja superblocku
    Superblock superblock{};

    superblock.diskSize = blockSize * totalBlocks;
    superblock.blockSize = blockSize;
    superblock.diskUsage = 0;  // Na pocz¹tku dysk jest pusty
    superblock.fileCount = 0;
    superblock.freeBlocks = totalBlocks;
    superblock.lastModified = std::time(nullptr);  // Ustawia aktualny czas

    // Zapis superblocku do pliku
    diskFile.write(reinterpret_cast<const char*>(&superblock), sizeof(Superblock));

    // Inicjalizacja i zapis i-nodów
    INode emptyINode{};
    emptyINode.creationTime = 0;
    emptyINode.modificationTime = 0;
    emptyINode.permissions = 0;
    emptyINode.fileSize = 0;
    emptyINode.isDirectory = false;
    emptyINode.referenceCount = 0;

    // Przyk³adowa liczba i-nodów (mo¿e byæ dostosowana)
    const uint32_t totalINodes = 100;
    for (uint32_t i = 0; i < totalINodes; ++i) {
        diskFile.write(reinterpret_cast<const char*>(&emptyINode), sizeof(INode));
    }

    // Inicjalizacja bloków danych
    char* emptyBlock = new char[blockSize];
    std::memset(emptyBlock, 0, blockSize);
    for (uint32_t i = 0; i < totalBlocks; ++i) {
        diskFile.write(emptyBlock, blockSize);
    }

    delete[] emptyBlock;

    diskFile.close();
    std::cout << "Virtual disk initialized." << std::endl;
}

int main() {
    // Przyk³adowe parametry inicjalizacji
    initializeVirtualDisk("virtual_disk.bin", 1024, 1000); // 1000 bloków po 1024 bajty ka¿dy
    return 0;
}


