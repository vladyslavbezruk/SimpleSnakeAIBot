#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <stdlib.h>

#define border 4
#define head 3
#define mSize 15
#define nDatas 4
#define vDistance 5
//up, down, left, right
int defDy[4]{ -1, 1, 0, 0 };
int defDx[4]{ 0, 0, -1, 1 };

struct NEURON {
	int* data;

	int* dy;
	int* dx;

	NEURON() {

	}

	void init(int nData) {
		data = new int[nData];
		dy = new int[nData];
		dx = new int[nData];

		dy = defDy;
		dx = defDx;

		for (int i = 0; i < nData; i++) {
			data[i] = 0;
		}
	}
};

struct BRAIN {
	NEURON** neurons;

	int** cordX;
	int** cordY;

	BRAIN(int size, int nData) {
		neurons = new NEURON * [size]; 
		cordX = new int * [size];
		cordY = new int* [size];

		for (int i = 0; i < size; i++) {
			neurons[i] = new NEURON[size];
			cordX[i] = new int [size];
			cordY[i] = new int[size];

			for (int j = 0; j < size; j++) {
				neurons[i][j].init(nData);
			}
		}
	}
};

struct MAP {
	int size;
	int** data;

	MAP(int s) {
		size = s;

		data = new int* [size];

		for (int i = 0; i < size; i++) {
			data[i] = new int[size];

			for (int j = 0; j < size; j++) {
				data[i][j] = 0;
			}
		}
	}
};

int max(int a, int b) {
	if (a >= b) {
		return a;
	}
	return b;
}

int maxA(int* num, int size) {
	int m = num[0];

	for (int i = 1; i < size; i++) {
		m = max(m, num[i]);
	}

	return m;
}

bool* isMax(int* num, int size) {
	bool* bMax = new bool[size];
	int max = maxA(num, size);

	for (int i = 0; i < size; i++) {
		if (max == num[i]) {
			bMax[i] = true;
		}
		else {
			bMax[i] = false;
		}
	}

	return bMax;
}

void clearDisp() {
	system("cls");
}

void printMap(MAP map) {
	printf("\n");
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++) {
			printf("%d", map.data[i][j]);
		}
		printf("\n");
	}
}

int random(int a) {
	srand(time(NULL));
	return round((a - 1) * (double)(rand() % 100) / 100);
}

bool isA(int a, int min, int max) {
	if (a > min && a < max) {
		return true;
	}
	return false;
}

MAP clearMap(MAP map) {
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++) {
			map.data[i][j] = 0;
		}
	}
	return map;
}

int main() {

	BRAIN brain(mSize, nDatas);
	BRAIN review(vDistance * 2 + 1, nDatas);

	MAP map(mSize);

	int cEat = 0;
	int x = mSize / 2, y = mSize / 2;

	map.data[y][x] = head;
	for (int k = 0; k < map.size; k++) {
		map.data[0][k] = border;
		map.data[map.size - 1][k] = border;
		map.data[k][0] = border;
		map.data[k][map.size - 1] = border;
	}

	for (int u = 0; u <= 100; u++) {

		clearDisp();
		int sum[nDatas];

		for (int i = 0; i < nDatas; i++) {
			sum[i] = 0;
		}

		int iA = 0;
		int jA = 0;

		for (int i = y - vDistance; i <= y + vDistance; i++) {
			for (int j = x - vDistance; j <= x + vDistance; j++) {
				if (jA == vDistance * 2) {
					jA = 0;
				}
				if (isA(i, -1, mSize) && isA(j, -1, mSize) && map.data[i][j] > 0 && i != y && j != x) {
					//printf("iA = %d, jA = %d\n", iA, jA);
					review.cordY[iA][jA] = i;
					review.cordX[iA][jA] = j;

					review.neurons[iA][jA].data = brain.neurons[i][j].data;
				}
				else {
					for (int k = 0; k < nDatas; k++) {
						review.neurons[iA][jA].data[k] = 0;
					}
				}
				jA++;
			}
			iA++;
		}

		for (int i = 0; i < vDistance * 2 + 1; i++) {
			for (int j = 0; j < vDistance * 2 + 1; j++) {
				for (int k = 0; k < nDatas; k++) {
					sum[k] += review.neurons[i][j].data[k];
				}
			}
		}

		bool* mA = isMax(sum, nDatas);
		int count = 0;

		for (int i = 0; i < nDatas; i++) {
			if (mA[i]) {
				count++;
			}
		}

		int r = random(count) + 1;
		int ri, k = 0;

		for (int i = 0; i < nDatas; i++) {
			if (mA[i]) {
				k++;
				if (k == r) {
					ri = i;
					break;
				}
			}
		}

		int oldX = x;
		int oldY = y;

		x += defDx[ri];
		y += defDy[ri];

		//столкновение
		if (map.data[y][x] == border) {
			x = oldX;
			y = oldY;

			for (int i = y - vDistance; i <= y + vDistance; i++) {
				for (int j = x - vDistance; j <= x + vDistance; j++) {
					if (isA(i, -1, mSize) && isA(j, -1, mSize) && map.data[i][j] > 0 && i != y && j != x) {
						brain.neurons[i][j].data[ri]--;
					}
				}
			}

			cEat = 0;
			x = mSize / 2;
			y = mSize / 2;

			/*printf("DATA: count = %d, sum[0] = %d, sum[1] = %d, sum[2] = %d, sum[3] = %d\n", count, sum[0], sum[1], sum[2], sum[3]);
			printf("RESULT: i = %d, dx = %d, dy = %d\n", ri, defDx[ri], defDy[ri]);

			printMap(map);

			printf("\n");
			for (int i = 0; i < map.size; i++) {
				for (int j = 0; j < map.size; j++) {
					printf("%d,%d,%d,%d ", brain.neurons[i][j].data[0], brain.neurons[i][j].data[1], brain.neurons[i][j].data[2], brain.neurons[i][j].data[3]);
				}
				printf("\n");
			}

			return 0;
			*/
		}

		map = clearMap(map);
		map.data[y][x] = head;

		for (int k = 0; k < map.size; k++) {
			map.data[0][k] = border;
			map.data[map.size - 1][k] = border;
			map.data[k][0] = border;
			map.data[k][map.size - 1] = border;
		}

		printf("DATA: count = %d, sum[0] = %d, sum[1] = %d, sum[2] = %d, sum[3] = %d\n", count, sum[0], sum[1], sum[2], sum[3]);
		printf("RESULT: i = %d, dx = %d, dy = %d\n", ri, defDx[ri], defDy[ri]);

		printMap(map);
	}

	printf("\n");
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++) {
			printf("%d,%d,%d,%d ", brain.neurons[i][j].data[0], brain.neurons[i][j].data[1], brain.neurons[i][j].data[2], brain.neurons[i][j].data[3]);
		}
		printf("\n");
	}

	return 0;
}
