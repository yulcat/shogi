#include "move.h"

typedef struct{
	int x,y;
	char occupied;
	char enemyReach[8];
	char myReach[8];
} Tile;

typedef struct{
	Tile tile[3][4];
} Board;