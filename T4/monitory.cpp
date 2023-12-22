#include <iostream>
#include <vector>
#include <unistd.h>
#include "monitor.h"
 
using namespace std;

class MyMonitor : Monitor {
	std::vector<int> buffer;
	Condition prodEvenCond, consEvenCond, prodOddCond, consOddCond;
public:
	void putEven(int element);
	int getEven();
	void putOdd(int element);
	int getOdd();
    vector<int> getBuffer() {
        return buffer;
    }
};
 
MyMonitor myMonitor;
int const threadsCounts = 8; 

int generateEvenNumber() {
	return rand() % 25 * 2;
}

int generateOddNumber() {
	return rand() % 24 * 2 + 1;
}

int countEvenBuffer() {
	int count = 0;
	for (int i = 0; i < myMonitor.getBuffer().size(); i++) {
		if (myMonitor.getBuffer()[i] % 2 == 0) {
			count++;
		}
	}
	return count;
}

int countOddBuffer() {
	int count = 0;
	for (int i = 0; i < myMonitor.getBuffer().size(); i++) {
		if (myMonitor.getBuffer()[i] % 2 == 1) {
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
	return myMonitor.getBuffer().size() >= 3 && myMonitor.getBuffer()[0] % 2 == 0;
}

bool canConsOdd() {
	return myMonitor.getBuffer().size() >= 7 && myMonitor.getBuffer()[0] % 2 == 1;
}
 
void MyMonitor::putEven(int element) {
	enter();
	if (!canProdEven()) {
		wait(prodEvenCond);
	}
	buffer.push_back(element);
	std::cout << buffer.size() << std::endl;
	if (prodOddCond.getWaitingCount() && canProdOdd()) {
		signal(prodOddCond);
	} else if (consOddCond.getWaitingCount() && canConsOdd()) {
		signal(consOddCond);
	} else if (consEvenCond.getWaitingCount() && canConsEven()) {
		signal(consEvenCond);
	} else {		// ???????????????
		leave();	// ???????????????
	}			// ???????????????
}

void MyMonitor::putOdd(int element) {
	enter();
	if (!canProdOdd()) {
		wait(prodOddCond);
	}
	buffer.push_back(element);
	std::cout << buffer.size() << std::endl;
	if (prodEvenCond.getWaitingCount() && canProdEven()) {
		signal(prodEvenCond);
	} else if (consOddCond.getWaitingCount() && canConsOdd()) {
		signal(consOddCond);
	} else if (consEvenCond.getWaitingCount() && canConsEven()) {
		signal(consEvenCond);
	} else {		// ???????????????
		leave();	// ???????????????
	}			// ???????????????
}

int MyMonitor::getOdd() {
	enter();
	if (!canConsOdd()) {
		wait(consOddCond);
	}
	buffer.erase(buffer.begin());
	std::cout << buffer.size() << std::endl;
    if (consEvenCond.getWaitingCount() && canConsEven()) {
		signal(consEvenCond);
    } else if (prodEvenCond.getWaitingCount() && canProdEven()) {
		signal(prodEvenCond);
	} else if (prodOddCond.getWaitingCount() && canProdOdd()) {
		signal(prodOddCond);
	} else {		// ???????????????
		leave();	// ???????????????
	}			// ???????????????
}

int MyMonitor::getEven() {
	enter();
	if (!canConsEven()) {
		wait(consEvenCond);
	}
	buffer.erase(buffer.begin());
	std::cout << buffer.size() << std::endl;
    if (consOddCond.getWaitingCount() && canConsOdd()) {
		signal(consOddCond);
    } else if (prodEvenCond.getWaitingCount() && canProdEven()) {
		signal(prodEvenCond);
	} else if (prodOddCond.getWaitingCount() && canProdOdd()) {
		signal(prodOddCond);
	} else {		// ???????????????
		leave();	// ???????????????
	}			// ???????????????
}
 
void* prodEven(void* arg) {
	while(1) {
		myMonitor.putEven(generateEvenNumber());
		sleep(1);	
	}
}

void* prodOdd(void* arg) {
	while(1) {
		myMonitor.putOdd(generateOddNumber());
		sleep(1);	
	}
}

void* consOdd(void* arg) {
	while(1) {
		myMonitor.getOdd();
		sleep(1);	
	}
}

void* consEven(void* arg) {
	while(1) {
		myMonitor.getEven();
		sleep(1);	
	}
}


int main( int argc, char* argv[])
{
    // for (int i = 0; i < 9; i++) {
    //     myMonitor.getBuffer().push_back(i);
    // }
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