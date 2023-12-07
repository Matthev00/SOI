#include <semaphore>
#include <vector>
# include <counti

class FifoBuffer {
private:
    std::vector<int> buffer;
    size_t capacity;
    Semaphore mutex; // Semafor binarny do ochrony sekcji krytycznej
    Semaphore empty;  // Semafor do sygnalizowania, że bufor jest pusty
    Semaphore full;   // Semafor do sygnalizowania, że bufor jest pełny

    int evenCount;
    int oddCount;

public:
    FifoBuffer(size_t cap) : capacity(cap), mutex(1), empty(cap), full(0), evenCount(0), oddCount(0) {}

    void put(int value) {
        full.p(); // Czeka, aż będzie miejsce w buforze
        mutex.p(); // Blokuje dostęp do bufora
        buffer.push_back(value);
        updateCountsOnPut(value);
        mutex.v(); // Zwalnia dostęp do bufora
        empty.v(); // Sygnalizuje, że bufor nie jest pusty
    }

    int get(bool consumeEven) {
        int value = 0;
        empty.p(); // Czeka, aż bufor będzie miał elementy
        mutex.p(); // Blokuje dostęp do bufora
        if (!buffer.empty() && (consumeEven == isEven(buffer.front()))) {
            value = buffer.front();
            buffer.erase(buffer.begin());
            updateCountsOnGet(value);
        }
        mutex.v(); // Zwalnia dostęp do bufora
        full.v(); // Sygnalizuje, że w buforze jest miejsce
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