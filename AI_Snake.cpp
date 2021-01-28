#include <iostream>
#include <cstdlib>
#include <ctime>	

using namespace std;

int getRandom(int a, int b) {
	srand(time(0));
	return rand() % (b - a + 1) + a;
}

int main() {





}