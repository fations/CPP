#ifndef PROJECT3_H_
#define PROJECT3_H_

// The basic city node
typedef struct Node_t {
	int xpos;
	int ypos;
	bool g;
	Node_t* parent;
} square;

typedef struct game_t {
	square grid[10][10];
	bool s[10][10],b[10][10],m[10][10],p[10][10];
} game;

typedef struct kb_t {
	int s[10][10],b[10][10],m[10][10],p[10][10];
} kb;

#endif