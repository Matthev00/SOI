#include <semaphore>
#include <vector>

class FifoBuffer {
private:
    std::vector<int> buffer;
    size_t capacity;
    std::counting_semaphore<1> mutex; // Semafor binarny do ochrony sekcji krytycznej
    std::counting_semaphore<> empty;  // Semafor do sygnalizowania, że bufor jest pusty
    std::counting_semaphore<> full;   // Semafor do sygnalizowania, że bufor jest pełny

    int evenCount;
    int oddCount;

public:
    FifoBuffer(size_t cap) : capacity(cap), mutex(1), empty(cap), full(0), evenCount(0), oddCount(0) {}

    void put(int value) {
        full.acquire(); // Czeka, aż będzie miejsce w buforze
        mutex.acquire(); // Blokuje dostęp do bufora
        buffer.push_back(value);
        updateCountsOnPut(value);
        mutex.release(); // Zwalnia dostęp do bufora
        empty.release(); // Sygnalizuje, że bufor nie jest pusty
    }

    int get(bool consumeEven) {
        int value = 0;
        empty.acquire(); // Czeka, aż bufor będzie miał elementy
        mutex.acquire(); // Blokuje dostęp do bufora
        if (!buffer.empty() && (consumeEven == isEven(buffer.front()))) {
            value = buffer.front();
            buffer.erase(buffer.begin());
            updateCountsOnGet(value);
        }
        mutex.release(); // Zwalnia dostęp do bufora
        full.release(); // Sygnalizuje, że w buforze jest miejsce
        return value;
    }

    int peek() const {
        if (!buffer.empty()) {
            return buffer.front();
        }
        return -1; // Lub inna wartość wskazująca na pusty bufor
    }

    int countEven() const { return evenCount; }
    int countOdd() const { return oddCount; }

private:
    void updateCountsOnPut(int value) {
        if (isEven(value)) {
            ++evenCount;
        } else {
            ++oddCount;
        }
    }

    void updateCountsOnGet(int value) {
        if (isEven(value)) {
            --evenCount;
        } else {
            --oddCount;
        }
    }

    bool isEven(int value) const {
        return value % 2 == 0;
    }
};