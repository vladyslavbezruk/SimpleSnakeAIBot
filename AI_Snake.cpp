#include <iostream>
#include <cstdlib>
#include <ctime>	

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
    { border, head, body} // -1 - border, -2 - head, -3 - body
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
int dx[DefNOfConnections] = {-1, 1, 0, 0};
int dy[DefNOfConnections] = {0, 0, -1, 1};

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

			reactions[i] = DefReactions[i];

			for (int j = 0; j < nOfConnections; j++) {
				connections[i][j] = 0;
			}
		}
	}
};

struct COORD {
	int x;
	int y;

	COORD() {
		x = 0;
		y = 0;
	}

	COORD(int _x, int _y) {
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

		data = new NEURON* [size];
		
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

struct SNAKE {
	BRAIN brain;
	int bSize;
	int bNOfTypes;
	int bNOfConnections;
	int sSize;
	COORD coords[maxSnakeSize];

	SNAKE(int _bSize, int _nOfTypes, int _nOfConnections) {
		sSize = 1;
		bSize = _bSize;
		bNOfTypes = _nOfTypes;
		bNOfConnections = _nOfConnections;
		BRAIN brain(bSize, bNOfTypes, bNOfConnections);
	}
};

int getRandom(int a, int b) {
	return rand() % (b - a + 1) + a;
}

void randomize() {
	srand(time(0));
}

bool isBetween(int n, int a, int b) {
	if (n >= a && n <= b) {
		return true;
	}
	return false;
}

COORD getRandomCoord(int a, int b) {
	COORD coord(getRandom(a, b), getRandom(a, b));
	return coord;
}

int getValue(MAP map, COORD coord) {
	return map.value[coord.x][coord.y];
}

MAP setValue(MAP map, COORD coord, int value) {
	map.value[coord.x][coord.y] = value;
	return map;
}

MAP createEats(MAP map, int count) {
	COORD eatCoord;

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
			cout << map.value[i][j];
		}
		cout << endl;
	}
}

COORD convertSnakeToMap(MAP map, SNAKE snake, COORD coord) {
	//snake.coords[0].x, snake.coords[0].x - map
	//snake.size - 1 / 2 , snake.size - 1 / 2 - snake

	COORD mCoord(snake.coords[0].x + coord.x - (snake.bSize - 1) / 2, snake.coords[0].y + coord.y - (snake.bSize - 1) / 2);

	return mCoord;
}

bool isThere(int* arr, int n, int size) {
	for (int i = 0; i < size; i++) {
		if (n == arr[i]) {
			return true;
		}
	}
	return false;
}

int getIndexMax(int* arr, int size) {
	int maxI = 0;

	for (int i = 1; i < size; i++) {
		if (arr[i] > arr[maxI]) {
			maxI = i;
		}
	}

	return maxI;
}

int getMax(int* arr, int size) {
	return arr[getIndexMax(arr, size)];
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

			if (!pos) {
				return i;
			}
		}
	}

	return -1;
}

int checkNeuronActivation(MAP map, SNAKE snake, COORD coord) {
	COORD mCoord = convertSnakeToMap(map, snake, coord);

	int value = getValue(map, mCoord);

	for (int i = 0; i < snake.bNOfTypes; i++) {
		if (isThere(snake.brain.data[coord.x][coord.y].reactions[i], value, snake.bNOfConnections)) {
			return i;
		}
	}

	return -1;
}

int* addArrays(int* a, int* b, int size) {
	for (int i = 0; i < size; i++) {
		a[i] += b[i];
	}

	return a;
}

int getMaxConnections(MAP map, SNAKE snake) {
	int* sum = new int[snake.bNOfConnections];
	sum = { 0 };

	for (COORD bCoord(0, 0); bCoord.y < snake.bSize; bCoord.y++) {
		for (bCoord.x = 0; bCoord.x < snake.bSize; bCoord.x++) {
			if (bCoord.x != bCoord.y && 2 * bCoord.y != snake.bSize - 1) {
				int type = checkNeuronActivation(map, snake, bCoord);

				if (type != -1) {
					sum = addArrays(sum, snake.brain.data[bCoord.x][bCoord.y].connections[type], snake.bNOfConnections);
				}
			}
		}
	}

	return getRandomIndex(sum, snake.bNOfConnections, getMax(sum, snake.bNOfConnections));
}

int main() {

	randomize();
	MAP map(10);

	map = createEats(map, 1);

	printMap(map);

}