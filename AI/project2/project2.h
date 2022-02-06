#ifndef PROJECT2_H_
#define PROJECT2_H_

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <conio.h>
#include <math.h>
#include <list>
#include <queue>
#include <cstdlib>

using namespace std;

// The basic city node
typedef struct Node_t {
	int xpos;
	int ypos;
	int occby;
	int s;
	bool c;

} square;

typedef queue<square*> q;

typedef struct game_t {
	q que;
	int h;
	struct game_t *parent;
	struct game_t *child;
} game;

#endif