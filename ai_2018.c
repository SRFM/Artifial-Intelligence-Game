#include <stdio.h>
#include <stdlib.h>

#define M 5
#define K1 3
#define K2 2
#define K3 2

#define RED 0
#define GREEN 1
#define YELLOW 2
#define TRUE 1
#define FALSE 0

typedef struct
{
	int number, color;
} Move;

typedef struct
{
	int red, green, yellow;
	Move currMove;
} Table;

Move possibleMoves[6]; // global range

void initializePossibleMoves()
{
	possibleMoves[0].number = 1;
	possibleMoves[0].color = RED;
		
	possibleMoves[1].number = 1;
	possibleMoves[1].color = GREEN;
		
	possibleMoves[2].number = 1;
	possibleMoves[2].color = YELLOW;
		
	possibleMoves[3].number = K1;
	possibleMoves[3].color = RED;
		
	possibleMoves[4].number = K2;
	possibleMoves[4].color = GREEN;
		
	possibleMoves[5].number = K3;
	possibleMoves[5].color = YELLOW;
}

void initializeHead(Table *t)
{
		t -> red = M;
		t -> green = M;
		t -> yellow = M;
}

void placeMove(Table *t, Move m)
{
	switch (m.color)
	{
		case RED: t -> red -= m.number; break;
		case GREEN: t-> green -= m.number; break;
		case YELLOW: t-> yellow -= m.number; break;
	}
}

Table newTableAfterMove(Table t, Move m)
{
	Table tempTable;
	tempTable.red = t.red;
	tempTable.green = t.green;
	tempTable.yellow = t.yellow;
	placeMove(&tempTable, m);
	return tempTable;
}

int checkGameOver(Table t)
{
	return (t.red == 0 && t.green == 0 && t.yellow == 0);
}

void printTable(Table t)
{
	int i;
	for (i = 0; i < t.red; i++)
	{
		printf("|r| ");
	}
	for (i = 0; i < t.green; i++)
	{
		printf("|g| ");
	}
	for (i = 0;i < t.yellow; i++)
	{
		printf("|y| ");
	}
	printf("\n");
}

int isValidMove(Table t, Move m)
{
	switch (m.color)
	{
		case RED: if (m.number <= t.red && (m.number == 1 || m.number == K1)) return TRUE; break;
		case GREEN: if (m.number <= t.green && (m.number == 1 || m.number == K2)) return TRUE; break;
		case YELLOW: if (m.number <= t.yellow && (m.number == 1 || m.number == K3)) return TRUE; break;
		default : return FALSE;
	}
	return FALSE;
}

void userPlay(Table *t)
{
	int num, col;
	char ch;
	printf("Make your move!\n");
userInput:
	printf("What color of cubes do you wish to take? [r/g/y]\n");
	scanf(" %c",&ch);
	if (ch == 'r')
	{
		col = RED;
		printf("How many red cubes do you wish to remove? [1/%d]\n", K1);
	}
	else if (ch == 'g')
	{
		col = GREEN;
		printf("How many green cubes do you wish to remove? [1/%d]\n", K2);
	}
	else if (ch == 'y')
	{
		col = YELLOW;
		printf("How many yellow cubes do you wish to remove? [1/%d]\n", K3);
	}
	else {printf("Invalid input!Please try again! [r/g/y]\n"); goto userInput;}
	scanf("%d", &num);
	Move m;
	m.color = col;
	m.number = num;
	if (!isValidMove(*t, m)) {printf("Invalid input! Please try again!\n"); goto userInput;}
	placeMove(&(*t), m);
}

int getScore(int depth, int aiWins)
{
	if (aiWins) {return (100 - depth); }
	else {return (depth - 100); }
}

int minimax(Table t, int aiTurn, int depth)
{
	if (checkGameOver(t))	return getScore(depth, aiTurn);
	if (aiTurn)
	{
		int i, max = -14000, curr;
		for (i = 0; i < 6; i++)
		{
			if (isValidMove(t, possibleMoves[i]))
			{
				curr = minimax(newTableAfterMove(t, possibleMoves[i]), FALSE, depth + 1);
				if (curr > max) max = curr;
			}
		}
		return max;
	}
	else {
		int i, min = 14000, curr;
		for (i = 0; i < 6; i++)
		{
			if (isValidMove(t, possibleMoves[i]))
			{
				curr = minimax(newTableAfterMove(t, possibleMoves[i]), TRUE, depth + 1);
				if (curr < min) min = curr;
			}
		}
		return min;
	}
}

int nextMoveAi(Table t)
{
	int i, max = -14000, maxIndex = -1, curr;
	for (i = 0; i < 6; i++)
	{
		if (isValidMove(t, possibleMoves[i]))
		{
			curr = minimax(newTableAfterMove(t, possibleMoves[i]), TRUE, 0);
			if (curr > max)
			{
				max = curr;
				maxIndex = i;
			}
		}
	}
	return maxIndex;
}

void computerPlay(Table *t)
{
	printf("It's the Computer's turn!\n");
	placeMove(&(*t), possibleMoves[nextMoveAi(*t)]);
}

int main ()
{
	int gameOver, aiWins;
	initializePossibleMoves();
	Table tbl;
	initializeHead(&tbl);
	printTable(tbl);
	 while (1)
	 {
		computerPlay(&tbl);
		if (checkGameOver(tbl)) { gameOver = TRUE; aiWins = TRUE; break;}
		printTable(tbl);
		userPlay(&tbl);
		if (checkGameOver(tbl)) { gameOver = TRUE; break;}
		printTable(tbl);
		
	}
	if (aiWins) printf("A.I. won!\n");
	else printf("You won!\n"); 
	return 0;
}