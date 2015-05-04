#include "rate.h"

int rate(Group animalsOnBoard, Move move){
	Board board = makeBoard(&animalsOnBoard,move);
	int i, x, y, score;
	for(i=0; i<animalsOnBoard.num; i++){
		applyReach(animalsOnBoard.animal[i], board);
	}
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			score += getScore(board.tile[x][y]);
		}
	}
	return 0;
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
		group->animal[group->num] = move.type;
		group->num++;
	}else{
		x = moveX(move.prevX,dirc);
		y = moveY(move.prevY,dirc);
		*group = moveGroup(*group, move.prevX, move.prevY, x, y);
		board.tile[move.prevX][move.prevY].occupied = 'o';
		board.tile[x][y].occupied = move.type;
	}
	return board;
}