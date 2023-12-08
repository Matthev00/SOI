unisgned int numOfProdEvenWaiting=0, numOfProdOddWaiting=0, numOfConsEvenWaiting=0, numOfConsOddWaiting=0;
Semaphore prodEvenSem(0), prodOddSem(0), consEvenSem(0), consOddSem(0);
Semaphore mutex(1);
vector<int> buffer;

void prodEven() {
	while(1) {
		mutex.P();
		if (!!!canProdEven()) {
			numOfProdEvenWaiting++;
			mutex.V();
			prodEvenSem.P();
			numOfProdEvenWaiting--;
		}
		int num = generateEvenNumber();	// wydajnosc!!!
		buffer.put(num);
		if (numOfProdOddWaiting > 0 && canProdOdd()) {
			prodOddSem.V();
		} else if (numOfConsEvenWaiting > 0 && canConsEven()) {
			consEvenSem.V();
		} else if (numOfConsOddWaiting > 0 && canConsOdd()) {
			consOddSem.V();
		} else {
			mutex.V();
		}
		sleep(...);
	}
}

int main() {
  thread_create(prodEven())
  thread_create
  thread_create
  thread_create
  thread_join
  thread_join
  thread_join
  thread_join
}

Testy: x2 (startowo pusty bufor i startowo pełny bufor)
1. prodEven
2. prodOdd
3. consEven
4. consOdd
5. prodEven prodOdd
6. consEven consOdd
7. prodEven consEven
8. prodOdd consOdd
9. prodEven prodOdd consEven consOdd
10. prodEven prodOdd consEven consOdd prodEven prodOdd consEven consOdd