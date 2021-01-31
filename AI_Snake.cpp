#include <iostream>
#include <cstdlib>
#include <ctime>	
#include <windows.h>

#define vDistance 5
#define maxSnakeSize 20
#define DefNOfTypes 2
#define DefNOfConnections 4
#define reactionsCount 3
#define eat 1
#define border -1
#define head -2
#define body -3
#define empty 0

using namespace std;

int DefReactions[DefNOfTypes][reactionsCount] = {
	{ eat, eat, eat}, // 1 - eat
	{ border, body, border} // -1 - border, -2 - head, -3 - body
};
/*
----------------------> y
|
|
|
|
x
*/
//up, down, left, right
int dx[DefNOfConnections] = { -1, 1, 0, 0 };
int dy[DefNOfConnections] = { 0, 0, -1, 1 };

struct NEURON {
	int** connections;
	int** reactions;
	int nOfTypes;
	int nOfConnections;

	NEURON() {

	}

	void init(int _nOfTypes, int _nOfConnections) {
		nOfTypes = _nOfTypes;
		nOfConnections = _nOfConnections;

		connections = new int* [nOfTypes];
		reactions = new int* [nOfTypes];

		for (int i = 0; i < nOfTypes; i++) {
			connections[i] = new int[nOfConnections];
			reactions[i] = new int[reactionsCount];

			for (int j = 0; j < reactionsCount; j++) {
				reactions[i][j] = DefReactions[i][j];
			}

			for (int j = 0; j < nOfConnections; j++) {
				connections[i][j] = 0;
			}
		}
	}
};

struct COOR {
	int x;
	int y;

	COOR() {
		x = 0;
		y = 0;
	}

	COOR(int _x, int _y) {
		x = _x;
		y = _y;
	}
};

struct BRAIN {
	NEURON** data;
	int size;
	int nOfTypes;
	int nOfConnections;

	BRAIN() {


	}

	BRAIN(int _size, int _nOfTypes, int _nOfConnections) {
		size = _size;
		nOfTypes = _nOfTypes;
		nOfConnections = _nOfConnections;

		data = new NEURON * [size];

		for (int i = 0; i < size; i++) {
			data[i] = new NEURON[size];

			for (int j = 0; j < size; j++) {
				data[i][j].init(nOfTypes, nOfConnections);
			}
		}
	}
};

struct MAP {
	int** value;
	int size;

	MAP(int _size) {
		size = _size;

		value = new int* [size];

		for (int i = 0; i < size; i++) {
			value[i] = new int[size];

			for (int j = 0; j < size; j++) {
				value[i][j] = 0;
			}
		}
	}

	void clear() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				value[i][j] = 0;
			}
		}
	}
};

MAP n(vDistance * 2 + 1);

struct SNAKE {
	BRAIN brain;
	int bSize;
	int bNOfTypes;
	int bNOfConnections;
	int sSize;
	COOR coords[maxSnakeSize];

	SNAKE(int _bSize, int _nOfTypes, int _nOfConnections) {
		sSize = 1;
		bSize = _bSize;
		bNOfTypes = _nOfTypes;
		bNOfConnections = _nOfConnections;
		BRAIN brain(bSize, bNOfTypes, bNOfConnections);
	}
};

void randomize() {
	srand(time(0));
}

int getRandom(int a, int b) {
	return rand() % (b - a + 1) + a;
}

bool isBetween(int n, int a, int b) {
	if (n >= a && n <= b) {
		return true;
	}
	return false;
}

void printArray(int* arr, int size) {
	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}

COOR getRandomCoord(int a, int b) {
	COOR coord(getRandom(a, b), getRandom(a, b));
	return coord;
}

int getValue(MAP map, COOR coord) {
	return map.value[coord.x][coord.y];
}

MAP setValue(MAP map, COOR coord, int value) {
	map.value[coord.x][coord.y] = value;
	return map;
}

MAP createEats(MAP map, int count) {
	COOR eatCoord;

	for (int i = 0; i < count; i++) {
		do {
			eatCoord = getRandomCoord(0, map.size - 1);
		} while (getValue(map, eatCoord) != empty);
		setValue(map, eatCoord, eat);
	}

	return map;
}

void printMap(MAP map) {
	for (int i = 0; i < map.size; i++) {
		for (int j = 0; j < map.size; j++) {
			if (map.value[i][j] == empty) {
				cout << "  ";
			}
			else {

				if (map.value[i][j] > 0) {
					cout << " " << map.value[i][j];
				}
				else {
					cout << map.value[i][j];
				}

			}
			
		}
		cout << endl;
	}
}

COOR convertSnakeToMap(MAP map, SNAKE snake, COOR coord) {
	//snake.coords[0].x, snake.coords[0].x - map
	//snake.size - 1 / 2 , snake.size - 1 / 2 - snake

	COOR mCoord(snake.coords[0].x + coord.x - (snake.bSize - 1) / 2, snake.coords[0].y + coord.y - (snake.bSize - 1) / 2);

	return mCoord;
}

bool isThere(int* arr, int n, int size) {
	for (int i = 0; i < size; i++) {
		if (arr[i] == n) {
			return true;
		}
	}
	return false;
}

int getIndexMax(int* arr, int size) {
	int maxI = 0;

	//cout << size << endl;

	/*for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;*/


	for (int i = 1; i < size; i++) {
		if (arr[i] > arr[maxI]) {
			maxI = i;
		}
	}

	return maxI;
}

int getMaximum(int* arr, int size) {
	return arr[getIndexMax(arr, size)];
}

void printCoord(COOR coord) {
	cout << "x = " << coord.x << " y = " << coord.y << endl;
}

int countIn(int* arr, int size, int n) {
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] == n) {
			count++;
		}
	}
	return count;
}

int getRandomIndex(int* arr, int size, int condition) {
	int count = countIn(arr, size, condition);
	int pos = getRandom(1, count);

	for (int i = 0; i < size; i++) {
		if (arr[i] == condition) {
			pos--;

			if (pos == 0) {
				return i;
			}
		}
	}

	return -1;
}

bool isCoordBetween(COOR coord, int a, int b) {
	if (isBetween(coord.x, a, b) && isBetween(coord.y, a, b)) {
		return true;
	}
	return false;
}

int checkNeuronActivation(MAP map, SNAKE snake, COOR coord) {
	COOR mCoord = convertSnakeToMap(map, snake, coord);

	if (isCoordBetween(mCoord, 0, map.size - 1)) {
		int value = getValue(map, mCoord);

		for (int i = 0; i < snake.bNOfTypes; i++) {
			if (isThere(snake.brain.data[coord.x][coord.y].reactions[i], value, reactionsCount)) {
				return i;
			}
		}
	}

	return -1;
}

/**int* addArrays(int a[], int b[], int size) {

	for (int i = 0; i < size; i++) {
		cout << a[i] << " ";
		cout << endl;
	}

	for (int i = 0; i < size; i++) {
		cout << b[i] << " ";
		cout << endl;
	}


	for (int i = 0; i < size; i++) {
		a[i] += b[i];
	}

	return a;
}*/

int getMaxConnections(MAP map, SNAKE snake) {
	int* cSum = new int[DefNOfConnections];

	for (int i = 0; i < DefNOfConnections; i++) {
		cSum[i] = 0;
	}


	for (int x = 0; x < snake.bSize; x++) {
		for (int y = 0; y < snake.bSize; y++) {
			//тут було написано if(x != y && y != (snake.bSize - 1) / 2) , але це неправильно
			if (!(y == (snake.bSize - 1) / 2 && x == (snake.bSize - 1) / 2)) { // <- це правильно

				COOR bCoord(x, y);

				int type = checkNeuronActivation(map, snake, bCoord);

				if (type != -1) {

					n = setValue(n, bCoord, type + 1);

					//printCoord(bCoord);
					//printCoord(convertSnakeToMap(map, snake, bCoord));
					//sum = addArrays(sum, snake.brain.data[bCoord.x][bCoord.y].connections[type], snake.bNOfConnections);
					//printArray(snake.brain.data[bCoord.x][bCoord.y].connections[type], snake.bNOfConnections);

					for (int i = 0; i < snake.bNOfConnections; i++) {
						cSum[i] += snake.brain.data[bCoord.x][bCoord.y].connections[type][i];
					}


				}
			}
		}
	}

	return getRandomIndex(cSum, snake.bNOfConnections, getMaximum(cSum, snake.bNOfConnections));
}

int bData[2][11][11][4] = { {
		{ {4, 0, 4, 0}, {5, 0, 4, 0}, {6, 0, 3, 0}, {7, 0, 2, 0}, {8, 0, 1, 0}, {10, 0, 0, 0}, {8, 0, 0, 1}, {7, 0, 0, 2}, {6, 0, 0, 3}, {5, 0, 0, 4}, {4, 0, 0, 4}},
		{ {4, 0, 5, 0}, {4, 0, 4, 0}, {6, 0, 3, 0}, {8, 0, 2, 0}, {12, 0, 2, 0}, {15, 0, 0, 0}, {12, 0, 0, 2}, {8, 0, 0, 2}, {6, 0, 3, 0}, {4, 0, 0, 4}, {4, 0, 0, 5}},
		{ {3, 0, 6, 0}, {3, 0, 6, 0}, {10, 0, 10, 0}, {10, 0, 10, 0}, {12, 0, 8, 0}, {20, 0, 0, 0}, {12, 0, 0, 8}, {10, 0, 0, 10}, {10, 0, 0, 10}, {3, 0, 0, 6}, {3, 0, 0, 6}},
		{ {2, 0, 7, 0}, {2, 0, 8, 0}, {10, 0, 10, 0}, {10, 0, 10, 0}, {13, 0, 12, 0}, {25, 0, 0, 0}, {13, 0, 0, 12}, {10, 0, 0, 10}, {10, 0, 0, 10}, {2, 0, 0, 8}, {2, 0, 0, 7}},
		{ {1, 0, 8, 0}, {2, 0, 12, 0}, {8, 0, 12, 0}, {12, 0, 13, 0}, {15, 0, 15, 0}, {35, 0, 0, 0}, {15, 0, 0, 15}, {12, 0, 0, 13}, {8, 0, 0, 12}, {2, 0, 0, 12}, {1, 0, 0, 8}},
		{ {0, 0, 10, 0}, {0, 0, 15, 0}, {0, 0, 20, 0}, {0, 0, 25, 0}, {0, 0, 35, 0}, {0, 0, 0, 0}, {0, 0, 0, 35}, {0, 0, 0, 25}, {0, 0, 0, 20}, {0, 0, 0, 15}, {0, 0, 0, 10}},
		{ {0, 1, 8, 0}, {0, 2, 12, 0}, {0, 8, 12, 0}, {0, 12, 13, 0}, {0, 15, 15, 0}, {0, 35, 0, 0}, {0, 15, 0, 15}, {0, 12, 0, 13}, {0, 8, 0, 12}, {0, 2, 0, 12}, {0, 1, 0, 8}},
		{ {0, 2, 7, 0}, {0, 2, 8, 0}, {0, 10, 10, 0}, {0, 10, 10, 0}, {0, 13, 12, 0}, {0, 25, 0, 0}, {0, 13, 0, 12}, {0, 10, 0, 10}, {0, 10, 0, 10}, {0, 2, 0, 8}, {0, 2, 0, 7}},
		{ {0, 3, 6, 0}, {0, 3, 6, 0}, {0, 10, 10, 0}, {0, 10, 10, 0}, {0, 12, 8, 0}, {0, 20, 0, 0}, {0, 12, 0, 8}, {0, 10, 0, 10}, {0, 10, 0, 10}, {0, 3, 0, 6}, {0, 3, 0, 6}},
		{ {0, 4, 5, 0}, {0, 4, 4, 0}, {0, 6, 3, 0}, {0, 8, 2, 0}, {0, 12, 2, 0}, {0, 15, 0, 0}, {0, 12, 0, 2}, {0, 8, 0, 2}, {0, 6, 0, 3}, {0, 4, 0, 4}, {0, 4, 0, 5}},
		{ {0, 4, 4, 0}, {0, 5, 4, 0}, {0, 6, 3, 0}, {0, 7, 2, 0}, {0, 8, 1, 0}, {0, 10, 0, 0}, {0, 8, 0, 1}, {0, 7, 0, 2}, {0, 6, 0, 3}, {0, 5, 0, 4}, {0, 4, 0, 4}}
}, {
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {-1, 1, -1, 1}, {-1, 0, -1, 1}, {-1, 0, -1, 1}, {-2, 0, 1, 1}, {-1, 0, 1, -1}, {-1, 0, 1, -1}, {-1, 1, 1, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {-1, 1, -1, 0}, {-2, 0, -2, 0}, {-3, 1, -3, 1}, {-7, 2, 2, 2}, {-3, 1, 1, -3}, {-2, 0, 0, -2}, {-1, 1, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {-1, 1, -1, 0}, {-3, 1, -3, 1}, {-5, 5, -5, 5}, {-99, 10, 10, 10}, {-5, 5, 5, -5}, {-3, 1, 1, -3}, {-1, 1, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, -2, 0}, {2, 2, -7, 2}, {10, 10, -99, 10}, {0, 0, 0, 0}, {10, 10, 10, -99}, {2, 2, 2, -7}, {1, 1, 0, -2}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {1, -1, -1, 0}, {1, -3, -3, 1}, {5, -5, -5, 5}, {10, -99, 10, 10}, {5, -5, 5, -5}, {1, -3, 1, -3}, {1, -1, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {1, -1, -1, 0}, {0, -2, -2, 0}, {1, -3, -3, 1}, {2, -7, 2, 2}, {1, -3, 1, -3}, {0, -2, 0, -2}, {1, -1, 0, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {1, -1, -1, 1}, {0, -1, -1, 1}, {0, -1, -1, 1}, {0, -1, 1, 1}, {0, -1, 1, -1}, {0, -1, 1, -1}, {1, -1, 1, -1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
		{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
} };

BRAIN createBrain(int data[DefNOfTypes][vDistance * 2 + 1][vDistance * 2 + 1][DefNOfConnections]) {
	BRAIN brain(vDistance * 2 + 1, DefNOfTypes, DefNOfConnections);

	for (int i = 0; i < brain.size; i++) {
		for (int j = 0; j < brain.size; j++) {
			for (int type = 0; type < brain.nOfTypes; type++) {
				for (int con = 0; con < DefNOfConnections; con++) {
					brain.data[i][j].connections[type][con] = data[type][i][j][con];
				}
			}
		}
	}

	return brain;
}

int main() {

	randomize();

	MAP map(20);

	SNAKE snake(vDistance * 2 + 1, DefNOfTypes, DefNOfConnections);
	snake.brain = createBrain(bData);
	map = createEats(map, 5);

	snake.coords[0].x = (map.size - 1) / 2;
	snake.coords[0].y = (map.size - 1) / 2;


	for (int i = 0; i < map.size; i++) {
		map.value[0][i] = border;
		map.value[map.size - 1][i] = border;
		map.value[i][0] = border;
		map.value[i][map.size - 1] = border;
	}

	while (true) {

		system("cls");

		n.clear();


		printCoord(snake.coords[0]);

		if (getValue(map, snake.coords[0]) == border || getValue(map, snake.coords[0]) == body) {
			cout << "Opps!" << endl;
			break;
		}

		/*if (snake.coords[0].x == -1) {
			snake.coords[0].x = map.size - 1;
		}
		if (snake.coords[0].x == map.size) {
			snake.coords[0].x = 0;
		}
		if (snake.coords[0].y == map.size) {
			snake.coords[0].y = 0;
		}
		if (snake.coords[0].y == -1) {
			snake.coords[0].y = map.size - 1;
		}*/



		int m = getMaxConnections(map, snake);

		COOR c(vDistance, vDistance);
		n = setValue(n, c, head);

		//printMap(n);


		/*for (int i = 0; i < snake.sSize; i++) {

			map = setValue(map, snake.coords[i], empty);

		}*/
		map = setValue(map, snake.coords[0], empty);
		/*for (int i = snake.sSize - 1; i > 0; i--) {
			snake.coords[i] = snake.coords[i - 1];
		}*/

		snake.coords[0].x += dx[m];
		snake.coords[0].y += dy[m];

		if (getValue(map, snake.coords[0]) == eat) {
			//snake.sSize++;
			map = createEats(map, 1);
		}

		/*for (int i = 0; i < snake.sSize; i++) {
			if (i == 0)
			map = setValue(map, snake.coords[i], head);
			else map = setValue(map, snake.coords[i], body);
		}*/
		map = setValue(map, snake.coords[0], head);

		printMap(map);
		
		//int a = 0;
		//cin >> a;

		Sleep(100);
	}
	printMap(map);
}