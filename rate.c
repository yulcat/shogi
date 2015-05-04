#include "rate.h"

int rate(Group animalsOnBoard, Move move){
	Board board = makeBoard(&animalsOnBoard,move);
	int i, x, y, score=0;
	for(i=0; i<animalsOnBoard.num; i++){
		applyReach(animalsOnBoard.animal[i], &board);
	}
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			score += getScore(board.tile[x][y]);
		}
	}
	return score;
}
Group moveGroup(Group group, int prevX, int prevY, int x, int y){
	if(isInBoard(x,y)==0)
		return group;
	int i;
	Group newGroup;
	newGroup.num = 0;
	for(i=0;i<group.num;i++){
		if(x==group.animal[i].x && y==group.animal[i].y){
			continue;
			//remove attacked animal
		}else if(prevX==group.animal[i].x && prevY==group.animal[i].y){
			group.animal[i].x = x;
			group.animal[i].y = y;
			//move animal
			if(y==0 && group.animal[i].type == 'C')
				group.animal[i].type = 'H';
				// chick to hen
		}
		newGroup.animal[i] = group.animal[i];
		newGroup.num++;
	}
	return newGroup;
}
Board makeBoard(Group* group, Move move){
	Board board;
	int x,y;
	for(x=0;x<3;x++){
		for(y=0;y<4;y++){
			board.tile[x][y].x = x;
			board.tile[x][y].y = y;
			board.tile[x][y].occupied = getTile(x,y,*group);
			board.tile[x][y].enemyNum = 0;
			board.tile[x][y].myNum = 0;
		}
	}
	if(move.dirc==PLACE){
		board.tile[move.prevX][move.prevY].occupied = move.type;
		group->animal[group->num] = newAnimal(move.type, move.prevX, move.prevY);
		group->num++;
	}else{
		x = moveX(move.prevX,move.dirc);
		y = moveY(move.prevY,move.dirc);
		*group = moveGroup(*group, move.prevX, move.prevY, x, y);
		board.tile[move.prevX][move.prevY].occupied = 'o';
		board.tile[x][y].occupied = move.type;
	}
	return board;
}
int getScore(Tile tile){
	int i, score=0;
	score += typeToScore(tile.occupied);
	for(i=0; i<tile.enemyNum; i++){
		score -= typeToScore(tile.occupied);
		tile.occupied = tile.enemyReach[i];
		if(i<tile.myNum){
			score -= typeToScore(tile.occupied);
			tile.occupied = tile.myReach[i];
		}else{
			break;
		}
	}
	return score;
}
void applyEnemy(char type, int prevX, int prevY, int dirc, Board* board){
	int x = moveX(prevX,dirc),
		y = moveY(prevY,dirc);
	if(isInBoard(x,y)==0)
		return;
	board->tile[x][y].enemyReach[board->tile[x][y].enemyNum] = type;
	board->tile[x][y].enemyNum++;
}
void applyMy(char type, int prevX, int prevY, int dirc, Board* board){
	int x = moveX(prevX,dirc),
		y = moveY(prevY,dirc);
	if(isInBoard(x,y)==0)
		return;
	board->tile[x][y].myReach[board->tile[x][y].enemyNum] = type;
	board->tile[x][y].myNum++;
}
void applyReach(Animal anim, Board* board){
	switch(anim.type){
	case 'L':
		applyMy('L', anim.x, anim.y, 1, board);
		applyMy('L', anim.x, anim.y, 2, board);
		applyMy('L', anim.x, anim.y, 3, board);
		applyMy('L', anim.x, anim.y, 4, board);
		applyMy('L', anim.x, anim.y, 6, board);
		applyMy('L', anim.x, anim.y, 7, board);
		applyMy('L', anim.x, anim.y, 8, board);
		applyMy('L', anim.x, anim.y, 9, board);
	case 'G':
		applyMy('G', anim.x, anim.y, 2, board);
		applyMy('G', anim.x, anim.y, 4, board);
		applyMy('G', anim.x, anim.y, 6, board);
		applyMy('G', anim.x, anim.y, 8, board);
	case 'E':
		applyMy('E', anim.x, anim.y, 1, board);
		applyMy('E', anim.x, anim.y, 3, board);
		applyMy('E', anim.x, anim.y, 7, board);
		applyMy('E', anim.x, anim.y, 9, board);
	case 'C':
		applyMy('C', anim.x, anim.y, 8, board);
	case 'H':
		applyMy('H', anim.x, anim.y, 2, board);
		applyMy('H', anim.x, anim.y, 4, board);
		applyMy('H', anim.x, anim.y, 6, board);
		applyMy('H', anim.x, anim.y, 7, board);
		applyMy('H', anim.x, anim.y, 8, board);
		applyMy('H', anim.x, anim.y, 9, board);
	case 'l':
		applyEnemy('l', anim.x, anim.y, 1, board);
		applyEnemy('l', anim.x, anim.y, 2, board);
		applyEnemy('l', anim.x, anim.y, 3, board);
		applyEnemy('l', anim.x, anim.y, 4, board);
		applyEnemy('l', anim.x, anim.y, 6, board);
		applyEnemy('l', anim.x, anim.y, 7, board);
		applyEnemy('l', anim.x, anim.y, 8, board);
		applyEnemy('l', anim.x, anim.y, 9, board);
	case 'g':
		applyEnemy('g', anim.x, anim.y, 2, board);
		applyEnemy('g', anim.x, anim.y, 4, board);
		applyEnemy('g', anim.x, anim.y, 6, board);
		applyEnemy('g', anim.x, anim.y, 8, board);
	case 'e':
		applyEnemy('e', anim.x, anim.y, 1, board);
		applyEnemy('e', anim.x, anim.y, 3, board);
		applyEnemy('e', anim.x, anim.y, 7, board);
		applyEnemy('e', anim.x, anim.y, 9, board);
	case 'c':
		applyEnemy('c', anim.x, anim.y, 2, board);
	case 'h':
		applyEnemy('h', anim.x, anim.y, 1, board);
		applyEnemy('h', anim.x, anim.y, 2, board);
		applyEnemy('h', anim.x, anim.y, 3, board);
		applyEnemy('h', anim.x, anim.y, 4, board);
		applyEnemy('h', anim.x, anim.y, 6, board);
		applyEnemy('h', anim.x, anim.y, 8, board);
	default:
		return;
	}
}
int typeToScore(char type){
	switch(type){
	case 'L':
		return 10000;
	case 'G':
		return 100;
	case 'E':
		return 100;
	case 'C':
		return 50;
	case 'H':
		return 200;
	case 'l':
		return -10000;
	case 'g':
		return -100;
	case 'e':
		return -100;
	case 'c':
		return -50;
	case 'h':
		return -200;
	default:
		return 0;
	}
}