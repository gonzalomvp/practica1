// practica1.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "conio.h"
#include "windows.h"

#define ESC        27
#define MOVE_LEFT  'j'
#define MOVE_RIGHT 'k'
#define FIRE_LEFT  'h'
#define FIRE_RIGHT 'l'

void checkCollisions();

int character = 50;
int bullet    = -1;
int bulletDir = 0;
int enemy     = -1;
int enemyDir  = 0;
int mushroom  = -1;
int points    = 0;
int lives     = 3;

int main()
{
	printf("\n\n\n\n");
	char key = NULL;

	while (key != ESC) {

		//check input
		key = NULL;
		if (_kbhit()) {
			key = _getch();
		}

		//process input
		switch (key) {
		case MOVE_LEFT:
			character--;
			if (character < 0) {
				character = 0;
			}
			break;

		case MOVE_RIGHT:
			character++;
			if (character > 99) {
				character = 99;
			}
			break;

		case FIRE_LEFT:
			if (bullet == -1) {
				bullet = character - 1;
				bulletDir = -1;
			}
			break;

		case FIRE_RIGHT:
			if (bullet == -1) {
				bullet = character + 1;
				bulletDir = 1;
			}
			break;
		}

		//check collisions
		checkCollisions();

		//move enemy
		enemy += enemyDir;

		//check collisions
		checkCollisions();

		//move bullet
		bullet += bulletDir;

		//check collisions
		checkCollisions();
			
		//enemy generation
		if (enemy == -1) {
			if (rand() % 2) {
				enemy = 0;
				enemyDir = 1;
			}
			else {
				enemy = 99;
				enemyDir = -1;
			}
		}

		//mushroom generation
		if (mushroom == -1) {
			mushroom = rand() % 100;
		}

		//Pintado
		printf("L: %d ", lives);
		for (unsigned int i = 0; i < 100; i++) {
			
			if (i == character) {
				printf("X");
			}
			else if (i == bullet) {
				if (bulletDir == -1) {
					printf("<");
				}
				else {
					printf(">");
				}
			}
			else if (i == enemy) {
				printf("@");
			}
			else if (i == mushroom) {
				printf("T");
			}
			else {
				printf("_");
			}
		}
		printf(" P: %d", points);
		printf("\r");

		Sleep(50);
	}
    return 0;
}

void checkCollisions()
{
	//check if bullet killed enemy
	if (bullet == enemy) {
		enemy = -1;
		enemyDir = 0;
		bullet = -1;
		bulletDir = 0;
	}

	//check if enemy killed character
	if (enemy == character) {
		enemy = -1;
		enemyDir = 0;
		bullet = -1;
		bulletDir = 0;
		character = 50;

		//decrease life
		lives--;
		if (lives >= 0) {
			character = 50;
		}
		else {
			character = -1;
		}
	}

	//check if character picked mushroom
	if (character == mushroom) {
		mushroom = -1;
		points += 10;
	}

	//check if enemy killed mushroom
	if (enemy == mushroom) {
		mushroom = -1;
	}
}