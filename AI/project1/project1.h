#ifndef PROJECT1_H_
#define PROJECT1_H_

#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <list>
#include <queue>

// The basic city node
typedef struct Node_t {
	int xpos;
	int ypos;
	int blocked;
	struct Node_t *parent;
	struct Node_t *left;
	struct Node_t *right;
	struct Node_t *top;
	struct Node_t *bottom;
	int h;
	int pc;

} Node;

#endif