#include <iostream>
#include <string>
#include "monitor.h"
#include <vector>


int const threadsCounts = 8;  //liczba wątków

int numOfProdEvenWaiting = 0, numOfProdOddWaiting = 0, numOfConsEvenWaiting = 0, numOfConsOddWaiting = 0;
Semaphore prodEvenSem(0), prodOddSem(0), consEvenSem(0), consOddSem(0);
Semaphore mutex(1);
std::vector<int> buffer;


void printBuffer() {
    std::cout << "Buffer: ";
    for (int i = 0; i < buffer.size(); i++) {
        std::cout << buffer[i] << " ";
    }
    std::cout << std::endl;
}

int generateEvenNumber() {
	return rand() % 25 * 2;
}

int generateOddNumber() {
	return rand() % 24 * 2 + 1;
}

int countEvenBuffer() {
	int count = 0;
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i] % 2 == 0) {
			count++;
		}
	}
	return count;
}

int countOddBuffer() {
	int count = 0;
	for (int i = 0; i < buffer.size(); i++) {
		if (buffer[i] % 2 == 1) {
			count++;
		}
	}
	return count;
}

bool canProdEven() {
	return countEvenBuffer() < 10;
}

bool canProdOdd() {
	return countEvenBuffer() > countOddBuffer();
}

bool canConsEven() {
	return buffer.size() >= 3 && buffer[0] % 2 == 0;
}

bool canConsOdd() {
	return buffer.size() >= 7 && buffer[0] % 2 == 1;
}

void* prodEven(void* arg) {
	while(1) {
		mutex.p();
		if (!canProdEven()) {
			numOfProdEvenWaiting++;
			mutex.v();
			prodEvenSem.p();
			mutex.p();
			numOfProdEvenWaiting--;
		}
		buffer.push_back(generateEvenNumber());
		printBuffer();
		if (numOfConsEvenWaiting > 0 && canConsEven()) {
			mutex.v();
			consEvenSem.v();
		} else if (numOfConsOddWaiting > 0 && canConsOdd()) {
			mutex.v();
			consOddSem.v();
		} else if (numOfProdOddWaiting > 0 && canProdOdd()) {
			mutex.v();
			prodOddSem.v();
		} else {
			mutex.v();
		}
		sleep(1);
	}
	return NULL;
}

void* prodOdd(void* arg) {
	while(1) {
		mutex.p();
		if (!canProdOdd()) {
			numOfProdOddWaiting++;
			mutex.v();
			prodOddSem.p();
			mutex.p();
			numOfProdOddWaiting--;
		}
		buffer.push_back(generateOddNumber());
		printBuffer();
		if (numOfConsEvenWaiting > 0 && canConsEven()) {
			mutex.v();
			consEvenSem.v();
		} else if (numOfConsOddWaiting > 0 && canConsOdd()) {
			mutex.v();
			consOddSem.v();
		} else if (numOfProdEvenWaiting > 0 && canProdEven()) {
			mutex.v();
			prodEvenSem.v();
		} else {
			mutex.v();
		}
		sleep(1);
	}
	return NULL;
}

void* consEven(void* arg) {
    while(1) {
        mutex.p();
        if (!canConsEven()) {
            numOfConsEvenWaiting++;
            mutex.v();
            consEvenSem.p();
			mutex.p();
            numOfConsEvenWaiting--;
        }
        buffer.erase(buffer.begin());
		printBuffer();
        if (numOfProdEvenWaiting > 0 && canProdEven()) {
            mutex.v();
            prodEvenSem.v();
        } else if (numOfProdOddWaiting > 0 && canProdOdd()) {
            mutex.v();
            prodOddSem.v();
        } else if (numOfConsOddWaiting > 0 && canConsOdd()) {
            mutex.v();
            consOddSem.v();
        } else {
            mutex.v();
        }
        sleep(1);
    }
	return NULL;
}

void* consOdd(void* arg) {
    while(1) {
        mutex.p();
        if (!canConsOdd()) {
            numOfConsOddWaiting++;
            mutex.v();
            consOddSem.p();
			mutex.p();
            numOfConsOddWaiting--;
        }
        buffer.erase(buffer.begin());
		printBuffer();
        if (numOfProdEvenWaiting > 0 && canProdEven()) {
            mutex.v();
            prodEvenSem.v();
        } else if (numOfProdOddWaiting > 0 && canProdOdd()) {
            mutex.v();
            prodOddSem.v();
        } else if (numOfConsEvenWaiting > 0 && canConsEven()) {
            mutex.v();
            consEvenSem.v();
        } else {
            mutex.v();
        }
        sleep(1);
    }
	return NULL;
}

int main( int argc, char* argv[])
{
	if (argc != 2) {
        std::cout << "Usage: ./program <test_number>\n";
        return 1;
    }
	int test_number = std::stoi(argv[1]);
#ifdef _WIN32
	////
#else
	pthread_t tid[threadsCounts];
	int threadsCounts = 0;
	switch (test_number) {
    case 1:
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        break;
    case 2:
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        break;
    case 3:
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        break;
    case 4:
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        break;
    case 5:
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        break;
    case 6:
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        break;
    case 7:
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        break;
    case 8:
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        break;
    case 9:
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        break;
    case 10:
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, prodEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, prodOdd, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consEven, NULL);
        pthread_create(&(tid[threadsCounts++]), NULL, consOdd, NULL);
        break;
}

// Dołączanie wątków
for (int i = 0; i < threadsCounts; i++) {
    pthread_join(tid[i], (void**)NULL);
}
	
#endif
	return 0;
}
