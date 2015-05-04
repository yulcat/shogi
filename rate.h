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
Group moveGroup(Group group, int prevX, int prevY, int x, int y);
Board makeBoard(Group* group, Move move);
void applyReach(Animal animal, Board* board);
int getMinBenefit(Tile tile);
int getScore(Board board);
int placePenalty(Move move);
int effectRange(Board board);
void applyEnemy(char type, int prevX, int prevY, int dirc, Board* board);
void applyMy(char type, int prevX, int prevY, int dirc, Board* board);
void applyReach(Animal anim, Board* board);
int typeToScore(char type);