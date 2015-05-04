#include "rate.h"

int rate(Group onBoard, Move move){
	Board board = makeBoard(&onBoard,move);
	
	return 0;
}
Group moveGroup(Group group, int prevX, int prevY, int x, int y){
	if(isInBoard(x,y)==0)
		return onBoard;
	int i;
	Group newBoard;
	newBoard.num = 0;
	for(i=0;i<group.num;i++){
		if(x==group.animal[i].x && y==group.animal[i].y){
			continue;
		}else if(prevX==group.animal[i].x && prevY==group.animal[i].y){
			group.animal[i].x = x;
			group.animal[i].y = y;
		}
		newBoard.animal[i] = group.animal[i];
		newBoard.num++;
	}
	return newBoard;
}
Board makeBoard(Group* onBoard, Move move){
	Board board;
	int x,y;
	for(x=0;x<3;x++){
		for(y=0;y<4;y++){
			board.tile[x][y].x = x;
			board.tile[x][y].y = y;
			board.tile[x][y].occupied = getTile(x,y,*onBoard);
			board.tile[x][y].enemyNum = 0;
			board.tile[x][y].myNum = 0;
		}
	}
	if(move.dirc==PLACE){
		board.tile[move.prevX][move.prevY].occupied = move.type;
	}else{
		x = moveX(move.prevX,dirc);
		y = moveY(move.prevY,dirc);
		*onBoard = moveGroup(*onBoard, move.prevX, move.prevY, x, y);
		board.tile[move.prevX][move.prevY].occupied = 'o';
		board.tile[x][y].occupied = move.type;
	}
	return board;
}