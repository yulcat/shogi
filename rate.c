#include "rate.h"

int rate(Group animalsOnBoard, Move move){ // Score a Move
	Board board = makeBoard(&animalsOnBoard,move);
	// Get Tile info from Group and Move
	int i, x, y;
	for(i=0; i<animalsOnBoard.num; i++){
		applyReach(animalsOnBoard.animal[i], &board);
		// apply list of animals that can reach the tile
	}
	int score = getScore(board) - placePenalty(move) + effectRange(board) + catchLion(board) + touchDown(board);
	// Score is board score + place penalty + moveable range + did we catched lion? + did lion get touchdown?
	return score;
}

Group moveGroup(Group group, int prevX, int prevY, int x, int y){ // apply Move to Group
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
Board makeBoard(Group* group, Move move){ // Make Board from Group and Move
	Board board;
	int x,y;
	for(x=0;x<3;x++){
		for(y=0;y<4;y++){
			board.tile[x][y].x = x;
			board.tile[x][y].y = y;
			board.tile[x][y].occupied = getTile(x,y,*group);
			board.tile[x][y].enemyNum = 0;
			board.tile[x][y].myNum = 0;
			// Initialize Board with Group info
		}
	}
	if(move.dirc==PLACE){
		board.tile[move.prevX][move.prevY].occupied = move.type;
		group->animal[group->num] = newAnimal(move.type, move.prevX, move.prevY);
		group->num++;
		// Apply Placement
	}else{
		x = moveX(move.prevX,move.dirc);
		y = moveY(move.prevY,move.dirc);
		*group = moveGroup(*group, move.prevX, move.prevY, x, y);
		board.tile[move.prevX][move.prevY].occupied = 'o';
		board.tile[x][y].occupied = move.type;
		// Apply Movement
	}
	return board;
}
int catchLion(Board board){ // If catched lion, plus 100000 points
	int x,y;
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			if(board.tile[x][y].occupied == 'l')
				return 0;
		}
	}
	return 100000;
}
int touchDown(Board board){ // did my/enemy lion get touchdown?
	int x,i;
	for(x=0; x<3; x++){
		Tile tile = board.tile[x][0];
		if(tile.occupied == 'L' && tile.enemyNum == 0)
			return 10000;
		// If my lion got touchdown, give additional points
		
		tile = board.tile[x][3];
		for(i=0; i<tile.enemyNum; i++){
			if(tile.enemyReach[i] == 'l' && tile.myNum == 0)
				return -10000;
			// If enemy lion can get touchdown, give penalty points
		}
	}
	return 0;
}
int getDanger(Tile tile){
	char target = tile.occupied;
	if(isMine(target)==0 || tile.enemyNum == 0)
		return 0;
		// If target is not mine, or there is no enemy nearby, Danger is 0.
	int myLoss = typeToScore(tile.occupied);
	if(tile.enemyNum > tile.myNum)
		return myLoss;
		// If enemy is outnumbered, count it loss.
	int i, minEnemyLoss = typeToScore(tile.enemyReach[0]);
	for(i=0; i<tile.enemyNum; i++){
		int enemyLoss = typeToScore(tile.enemyReach[i]);
		if(minEnemyLoss < enemyLoss)
			minEnemyLoss = enemyLoss;
	}
	return myLoss + minEnemyLoss;
	// If we can retake the tile, calculate profit and loss.
}
int getProfit(Tile tile){
	char target = tile.occupied;
	if(isEnemy(target)==0 || tile.myNum == 0)
		return 0;
		// If target is not enemy, or there is no mine nearby, Profit is 0.
	int enemyLoss = typeToScore(tile.occupied);
	if(tile.myNum > tile.enemyNum)
		return -enemyLoss;
		// If mine is outnumbered, count it profit.
	int i, minMyLoss = typeToScore(tile.myReach[0]);
	for(i=0; i<tile.enemyNum; i++){
		int myLoss = typeToScore(tile.myReach[i]);
		if(minMyLoss > myLoss)
			minMyLoss = myLoss;
	}
	return -(enemyLoss + minMyLoss);
	// If enemy can retake the tile, calculate profit and loss.
}
int getScore(Board board){
	int maxDanger=0, profits=0, x, y, score=0;
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			Tile tile = board.tile[x][y];
			char target = tile.occupied;
			int targetScore = typeToScore(target);
			int danger = getDanger(tile);
			score += targetScore;
			if(danger > maxDanger)
				maxDanger = danger;
			int profit = getProfit(tile);
			if(profit>0)
				profits += getProfit(tile);
			// Get dangers and profits
		}
	}
	printf("sum : %d\ndanger : %d\n", score, maxDanger);
	score -= maxDanger;
	if(maxDanger == 0){
		// Add profits only when there is no danger.
		printf("profit : %d\n",profits);
		score += profits;
	}
	return score;
}
int placePenalty(Move move){ // Placement gives false additional score from number of pieces,
							 // So remove the additional score for fair evaluation.
	if(move.dirc == PLACE)
		return typeToScore(move.type);
	else
		return 0;
}
int effectRange(Board board){ // Getting various moves is strategic benefit.
							  // Give additional score for each possible moves.
	int x,y,i,effect = 0;
	for(x=0;x<3;x++){
		for(y=0;y<4;y++){
			Tile tile = board.tile[x][y];
			for(i=0;i<tile.myNum;i++){
				if(tile.myReach[i]=='L' || isMine(tile.occupied))
					continue;
				effect++;
			}
		}
	}
	return effect;
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
	board->tile[x][y].myReach[board->tile[x][y].myNum] = type;
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
		return;
	case 'G':
		applyMy('G', anim.x, anim.y, 2, board);
		applyMy('G', anim.x, anim.y, 4, board);
		applyMy('G', anim.x, anim.y, 6, board);
		applyMy('G', anim.x, anim.y, 8, board);
		return;
	case 'E':
		applyMy('E', anim.x, anim.y, 1, board);
		applyMy('E', anim.x, anim.y, 3, board);
		applyMy('E', anim.x, anim.y, 7, board);
		applyMy('E', anim.x, anim.y, 9, board);
		return;
	case 'C':
		applyMy('C', anim.x, anim.y, 8, board);
		return;
	case 'H':
		applyMy('H', anim.x, anim.y, 2, board);
		applyMy('H', anim.x, anim.y, 4, board);
		applyMy('H', anim.x, anim.y, 6, board);
		applyMy('H', anim.x, anim.y, 7, board);
		applyMy('H', anim.x, anim.y, 8, board);
		applyMy('H', anim.x, anim.y, 9, board);
		return;
	case 'l':
		applyEnemy('l', anim.x, anim.y, 1, board);
		applyEnemy('l', anim.x, anim.y, 2, board);
		applyEnemy('l', anim.x, anim.y, 3, board);
		applyEnemy('l', anim.x, anim.y, 4, board);
		applyEnemy('l', anim.x, anim.y, 6, board);
		applyEnemy('l', anim.x, anim.y, 7, board);
		applyEnemy('l', anim.x, anim.y, 8, board);
		applyEnemy('l', anim.x, anim.y, 9, board);
		return;
	case 'g':
		applyEnemy('g', anim.x, anim.y, 2, board);
		applyEnemy('g', anim.x, anim.y, 4, board);
		applyEnemy('g', anim.x, anim.y, 6, board);
		applyEnemy('g', anim.x, anim.y, 8, board);
		return;
	case 'e':
		applyEnemy('e', anim.x, anim.y, 1, board);
		applyEnemy('e', anim.x, anim.y, 3, board);
		applyEnemy('e', anim.x, anim.y, 7, board);
		applyEnemy('e', anim.x, anim.y, 9, board);
		return;
	case 'c':
		applyEnemy('c', anim.x, anim.y, 2, board);
		return;
	case 'h':
		applyEnemy('h', anim.x, anim.y, 1, board);
		applyEnemy('h', anim.x, anim.y, 2, board);
		applyEnemy('h', anim.x, anim.y, 3, board);
		applyEnemy('h', anim.x, anim.y, 4, board);
		applyEnemy('h', anim.x, anim.y, 6, board);
		applyEnemy('h', anim.x, anim.y, 8, board);
		return;
	default:
		return;
	}
}
int typeToScore(char type){
	switch(type){
	case 'L':
		return 20000;
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