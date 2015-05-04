#ifndef MOVE
#include "move.h"
#endif

typedef struct{
	int x,y;
	char occupied;
	char enemyReach[8];
	int enemyNum;
	char myReach[8];
	int myNum;
} Tile;

typedef struct{
	Tile tile[3][4];
} Board;

int rate(Group onBoard, Move move);