#include <stdio.h>
#define MOVE
#define MAX_ANIMALS 8
#define MAX_MOVE 100
#define PLACE 5
#define OUT_OF_BOARD '\0'
#define MY_CHICK 0
#define MY_GIRAFFE 1
#define MY_ELEPHANT 2
#define MY_LION 3
#define MY_HEN 4
#define ENEMY_CHICK 5
#define ENEMY_GIRAFFE 6
#define ENEMY_ELEPHANT 7
#define ENEMY_LION 8
#define ENEMY_HEN 9
// Move direction by int :
//
//   7   8   9
//     \ | /
//   4 - L - 6
//     / | \
//   1   2   3
//

typedef struct{
	int x, y;
	char type;
} Animal;
// Struct of Animal pieces.
typedef struct{
	Animal animal[MAX_ANIMALS];
	int num;
} Group;
// List of Animal pieces, with size.
typedef struct{
	char type;
	int prevX,prevY,dirc;
} Move;
// Possible Move
typedef struct{
	Move list[MAX_MOVE];
	int num;
} Movelist;
// List of Possible Moves, with size.

extern const int animalDirections[10][8];
extern const int animalDirectionCount[10];
Animal newAnimal(char type, int x, int y);
Move newMove(char type, int prevX, int prevY, int dirc);
void inputBoard(Group* onBoard, Group* myHand, Group* enemyHand);
int isInBoard(int x, int y);
char getTile(int x, int y, Group group);
int moveX(int x, int dirc);
int moveY(int y, int dirc);
int isMine(char target);
int isEnemy(char target);
int charToAnimalnum(char type);
int testDirc(int x, int y, int dirc, Group onBoard);
void addMove(char type, int x, int y, int dirc, Group onBoard, Movelist* list);
void animalMove(Animal anim, Group onBoard, Movelist* list);
void printMove(Move move);
Movelist getMovelist(Group onBoard, Group myHand);