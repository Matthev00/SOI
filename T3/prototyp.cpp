#include <iostream>
#include <string>
#include "monitor.h"
#include <vector>


int const threadsCounts = 5;  //liczba wątków

int const bufferSize = 20;
int numOfProdEvenWaiting = 0, numOfProdOddWaiting = 0, numOfConsEvenWaiting = 0, numOfConsOddWaiting = 0;
Semaphore prodEvenSem(0), prodOddSem(0), consEvenSem(0), consOddSem(0);
Semaphore mutex(1);
std::vector<int> buffer;


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

void prodEven() {
	while(1) {
		mutex.p();
		if (!!!canProdEven()) {
			numOfProdEvenWaiting++;
			mutex.v();
			prodEvenSem.p();
			numOfProdEvenWaiting--;
		}
		int num = generateEvenNumber();
		buffer.put(num);
		if (numOfProdOddWaiting > 0 && canProdOdd()) {
			prodOddSem.v();
		} else if (numOfConsEvenWaiting > 0 && canConsEven()) {
			consEvenSem.v();
		} else if (numOfConsOddWaiting > 0 && canConsOdd()) {
			consOddSem.v();
		} else {
			mutex.v();
		}
		sleep(1000);
	}
}

int main()
{
#ifdef _WIN32
	HANDLE tid[threadsCounts];
	DWORD id;

	// utworzenie wątków
	tid[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadProd, 0, 0, &id);
	tid[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsA, 0, 0, &id);
	tid[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threadConsB, 0, 0, &id);

	// czekaj na zakończenie wątków
	for (int i = 0; i <= threadsCounts; i++)
		WaitForSingleObject(tid[i], INFINITE);
#else
	pthread_t tid[threadsCounts];

	// utworzenie wątków
	pthread_create(&(tid[0]), NULL, prodEven, NULL);
	pthread_create(&(tid[1]), NULL, prodOdd, NULL);
	pthread_create(&(tid[2]), NULL, consEven, NULL);
	pthread_create(&(tid[3]), NULL, consOdd, NULL);

	//czekaj na zakończenie wątków
	for (int i = 0; i < threadsCounts; i++)
		pthread_join(tid[i], (void**)NULL);
#endif
	return 0;
}

// Testy: x2 (startowo pusty bufor i startowo pełny bufor)
// 1. prodEven
// 2. prodOdd
// 3. consEven
// 4. consOdd
// 5. prodEven prodOdd
// 6. consEven consOdd
// 7. prodEven consEven
// 8. prodOdd consOdd
// 9. prodEven prodOdd consEven consOdd
// 10. prodEven prodOdd consEven consOdd prodEven prodOdd consEven consOdd