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
// Each tiles has list of animals which can move to the tile.
typedef struct{
	Tile tile[3][4];
} Board;
// Array of tiles. fixed size.

int rate(Group onBoard, Move move, Group enemyHand);
Group moveGroup(Group group, int prevX, int prevY, int x, int y);
Board makeBoard(Group* group, Move move);
void applyReach(Animal animal, Board* board);
int catchLion(Board board);
int touchDown(Board board);
int getDanger(Tile tile);
int getProfit(Tile tile);
int getScore(Board board);
int placePenalty(Move move);
int effectRange(Board board);
int placeDanger(Board board, Group enemyHand);
int getPlaceDanger(Board board, Animal enemy);
int animalCanFlee(Board board, int x, int y, int type);
int lionPosition(Group animalsOnBoard);
void applyEnemy(char type, int prevX, int prevY, int dirc, Board* board);
void applyMy(char type, int prevX, int prevY, int dirc, Board* board);
void applyReach(Animal anim, Board* board);
int typeToScore(char type);