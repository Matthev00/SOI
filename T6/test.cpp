#include <iostream>
#include <fstream>
#include <cstring>

// Definicja struktury nag³ówka dla wirtualnego dysku
struct DiskHeader {
    char signature[4]; // Sygnatura dysku, np. "VDISK"
    uint32_t blockSize; // Rozmiar bloku w bajtach
    uint32_t totalBlocks; // Ca³kowita liczba bloków na dysku
};

// Funkcja do inicjalizacji wirtualnego dysku
void initializeVirtualDisk(const std::string& diskName, uint32_t blockSize, uint32_t totalBlocks) {
    // Utworzenie pliku binarnego
    std::ofstream diskFile(diskName, std::ios::binary | std::ios::out);

    // Sprawdzenie, czy plik zosta³ poprawnie otwarty
    if (!diskFile) {
        std::cerr << "Nie mo¿na utworzyæ wirtualnego dysku!" << std::endl;
        return;
    }

    // Utworzenie i konfiguracja nag³ówka dysku
    DiskHeader header;
    std::strcpy(header.signature, "VDISK");
    header.blockSize = blockSize;
    header.totalBlocks = totalBlocks;

    // Zapisanie nag³ówka do pliku
    diskFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Zainicjowanie bloków danych na dysku
    char* emptyBlock = new char[blockSize];
    std::memset(emptyBlock, 0, blockSize);
    for (uint32_t i = 0; i < totalBlocks; ++i) {
        diskFile.write(emptyBlock, blockSize);
    }

    delete[] emptyBlock;
    diskFile.close();
    std::cout << "Wirtualny dysk zosta³ zainicjalizowany." << std::endl;
}

int main() {
    // Przyk³ad u¿ycia
    initializeVirtualDisk("virtual_disk.bin", 1024, 100); // 100 bloków po 1024 bajty
    return 0;
}