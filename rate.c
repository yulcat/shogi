#include "rate.h"

const int animalScores[10] =
{
	50,
	100,
	100,
	20000,
	200,
	-50,
	-100,
	-100,
	-10000,
	-200
};

int rate(Group animalsOnBoard, Move move, Group enemyHand){ // Score a Move
	Board board = makeBoard(&animalsOnBoard,move);
	// Get Tile info from Group and Move
	int i;
	for(i=0; i<animalsOnBoard.num; i++){
		applyReach(animalsOnBoard.animal[i], &board);
		// apply list of animals that can reach the tile
	}
	int score = getScore(board);
	score -= placePenalty(move);
	score += effectRange(board);
	score += catchLion(board);
	score += touchDown(board);
	score += placeDanger(board,enemyHand);
	score += lionPosition(animalsOnBoard);
	// Score is (board score) + (placement penalty)
	// + (moveable range) + (did we catched lion?) + (did lion get touchdown?)
	int x,y;
	for(y=0; y<4; y++){
		for(x=0; x<3; x++){
			printf("%c",board.tile[x][y].occupied);
		}
		printf("\n");
	}
	printf("place : %d\nlion : %d\ntotal : %d\n",placeDanger(board,enemyHand),lionPosition(animalsOnBoard),score);
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
		newGroup.animal[newGroup.num] = group.animal[i];
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
				return -5000;
			// If enemy lion can get touchdown, give penalty points
		}
	}
	return 0;
}
int placeDanger(Board board, Group enemyHand){
	int x,y,i,danger = 0;
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			if(board.tile[x][y].occupied != 'o')
				continue;
			if(board.tile[x][y].enemyNum < board.tile[x][y].myNum)
				continue;
			for(i=0; i<enemyHand.num; i++){
				Animal enemy = newAnimal(x,y,enemyHand.animal[i].type);
				danger += getPlaceDanger(board,enemy);
			}
		}
	}
	if(danger == 0)
		return 0;
	else
		return -danger;
}
int getPlaceDanger(Board board, Animal enemy){
	int i;
	int type = charToAnimalnum(enemy.type);
	for(i=0; i<animalDirectionCount[type]; i++){
		int dirc = animalDirections[type][i];
		int x = moveX(enemy.x, dirc);
		int y = moveY(enemy.y, dirc);
		if(x>=3 || x<0 || y>=4 || y<0)
			continue;
		char myType = charToAnimalnum(board.tile[x][y].occupied);
		if(!isMine(myType))
			continue;
		if(!animalCanFlee(board, x, y, myType))
			return typeToScore(board.tile[x][y].occupied)/2;
	}
	return 0;
}
int animalCanFlee(Board board, int x, int y, int type){
	int i, tileToFlee = 0;
	for(i=0; i<animalDirectionCount[type]; i++){
		int dirc = animalDirections[type][i];
		int myX = moveX(x, dirc);
		int myY = moveY(y, dirc);
		if(myX>3 || myX<0 || myY>4 || myY<0)
			continue;
		if(board.tile[myX][myY].occupied == 'o')
			tileToFlee = 1;
		if(isEnemy(board.tile[myX][myY].occupied) && board.tile[myX][myY].myNum == 1)
			return 0;
	}
	return tileToFlee;
}
int lionPosition(Group animalsOnBoard){
	// Lion position bonus
	int i;
	for(i=0; i<animalsOnBoard.num; i++){
		if(animalsOnBoard.animal[i].type == 'L')
			return (3-animalsOnBoard.animal[i].y)*3;
	}
	return 0;
}
int getDanger(Tile tile){ // Get danger from cost comparing
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
int getProfit(Tile tile){ // Get profit from cost comparing
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
int getScore(Board board){ // Sum of pieces on the board, with danger and profit
	int maxDanger=0, profits=0, x, y, score=0;
	for(x=0; x<3; x++){
		for(y=0; y<4; y++){
			Tile tile = board.tile[x][y];
			char target = tile.occupied;
			int targetScore = typeToScore(target);
			int danger = getDanger(tile);
			score += targetScore;
			// Score from number of pieces
			if(danger > maxDanger)
				maxDanger = danger;
			int profit = getProfit(tile);
			if(profit>0)
				profits += getProfit(tile);
			// Get dangers and profits
		}
	}
	score -= maxDanger;
	if(maxDanger == 0){
		// Add profits only when there is no danger.
		score += profits/2;
		// expected profit is evaluated half of the real one.
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
	int x,y,effect = 0;
	for(x=0;x<3;x++){
		for(y=0;y<4;y++){
			Tile tile = board.tile[x][y];
			if(isMine(tile.occupied)==0)
				effect += tile.myNum;
		}
	}
	return effect;
}
void applyEnemy(char type, int prevX, int prevY, int dirc, Board* board){
	// Apply enemy range to tile in the direction.
	int x = moveX(prevX,dirc),
		y = moveY(prevY,dirc);
	if(isInBoard(x,y)==0)
		return;
	board->tile[x][y].enemyReach[board->tile[x][y].enemyNum] = type;
	board->tile[x][y].enemyNum++;
}
void applyMy(char type, int prevX, int prevY, int dirc, Board* board){
	// Apply my range to tile in the direction.
	int x = moveX(prevX,dirc),
		y = moveY(prevY,dirc);
	if(isInBoard(x,y)==0)
		return;
	board->tile[x][y].myReach[board->tile[x][y].myNum] = type;
	board->tile[x][y].myNum++;
}
void applyReach(Animal anim, Board* board){
	// Apply moveable range to each tiles from animal piece.
	int i, type = charToAnimalnum(anim.type);
	for(i=0; i<animalDirectionCount[type]; i++){
		if(isMine(anim.type))
			applyMy(
				anim.type, 
				anim.x, 
				anim.y, 
				animalDirections[type][i], 
				board);
		else
			applyEnemy(
				anim.type, 
				anim.x, 
				anim.y, 
				animalDirections[type][i], 
				board);
	}
}
int typeToScore(char type){ // Animal type to score
	if(isMine(type)==0 && isEnemy(type)==0)
		return 0;
	else
		return animalScores[charToAnimalnum(type)];
}