#include <semaphore>
#include <vector>
# include <counti

class FifoBuffer {
private:
    std::vector<int> buffer;
    size_t capacity;
    Semaphore mutex; // Semafor binarny do ochrony sekcji krytycznej
    Semaphore empty;  // Semafor do sygnalizowania, że bufor jest pusty
    Semaphore full; // Semafor do sygnalizowania, że bufor jest pełny
    Semaphore semEvenLessThan10;
    Semaphore semMoreEvensThanOdds;
    Semaphore semAtLeastThree;
    Semaphore semAtLeastSeven;
    Semaphore semFrontEven;
    Semaphore semFrontOdd;

    int evenCount;
    int oddCount;

public:
    FifoBuffer(size_t cap) : capacity(cap), mutex(1), empty(cap), full(0), evenCount(0), oddCount(0) {}

    void put(int value) {
        if (isEven(value)) {
            semEvenLessThan10.p(); // Procesy A1 czekają, gdy jest 10 lub więcej liczb parzystych
        } else {
            semMoreEvensThanOdds.p(); // Procesy A2 czekają, gdy jest więcej lub tyle samo liczb nieparzystych co parzystych
        }
        full.p(); // Czeka, aż będzie miejsce w buforze
        mutex.p(); // Blokuje dostęp do bufora
        buffer.push_back(value);
        updateCountsOnPut(value);
        mutex.v(); // Zwalnia dostęp do bufora
        empty.v(); // Sygnalizuje, że bufor nie jest pusty
        if (buffer.size() >= 3) {
            semAtLeastThree.v();
        }
        if (buffer.size() >= 7) {
            semAtLeastSeven.v();
        }
    }

    int getB1() {
        semAtLeastThree.p(); // Czeka, aż w buforze będzie co najmniej 3 elementy
        semFrontEven.p(); // Czeka, aż na froncie bufora będzie liczba parzysta
        empty.p();
        mutex.p();

        int value = buffer.front();
        buffer.erase(buffer.begin());
        updateCountsOnGet(value);

        updateSemaphores(); // Aktualizuje semafory na podstawie nowego stanu bufora
        mutex.v();
        full.v();

        return value;
    }

    int getB2() {
        semAtLeastSeven.p(); // Czeka, aż w buforze będzie co najmniej 7 elementów
        semFrontOdd.p(); // Czeka, aż na froncie bufora będzie liczba nieparzysta
        empty.p();
        mutex.p();

        int value = buffer.front();
        buffer.erase(buffer.begin());
        updateCountsOnGet(value);

        updateSemaphores(); // Aktualizuje semafory na podstawie nowego stanu bufora
        mutex.v();
        full.v();

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
    void updateSemaphores() {
        if (evenCount < 10) {
            semEvenLessThan10.v(); // Podnosi semafor, jeśli liczba parzystych jest mniejsza niż 10
        } else {
            semEvenLessThan10.p(); // Opuszcza semafor, jeśli liczba parzystych jest 10 lub więcej
        }

        if (evenCount > oddCount) {
            semMoreEvensThanOdds.v(); // Podnosi semafor, jeśli jest więcej parzystych niż nieparzystych
        } else {
            semMoreEvensThanOdds.p(); // Opuszcza semafor, jeśli jest tyle samo lub mniej parzystych niż nieparzystych
        }

        if (buffer.size() >= 3) {
            semAtLeastThree.v();
        } else {
            semAtLeastThree.p();
        }

        if (buffer.size() >= 7) {
            semAtLeastSeven.v();
        } else {
            semAtLeastSeven.p();
        }

        if (!buffer.empty()) {
            if (isEven(buffer.front())) {
                semFrontEven.v();
                semFrontOdd.p(); // Opcjonalnie, możesz zablokować semafor semFrontOdd
            } else {
                semFrontOdd.v();
                semFrontEven.p(); // Opcjonalnie, możesz zablokować semafor semFrontEven
            }
        }
    }
};