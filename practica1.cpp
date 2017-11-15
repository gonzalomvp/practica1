#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <list>

#define ESC        27
#define MOVE_LEFT  'j'
#define MOVE_RIGHT 'k'
#define FIRE_LEFT  'h'
#define FIRE_RIGHT 'l'
#define WIDTH      50
#define T_ENEMY    30
#define T_MUSHROOM 100

void checkCollisions();

struct bullet {
	int pos;
	int dir;
};

struct enemy {
	int pos;
	int dir;
};

int g_character = (WIDTH / 2) - 1;
int g_mushroom  = -1;
int g_points    = 0;
int g_lives     = 3;

std::list<bullet> bullets;
std::list<enemy> enemies;

int main()
{
	//hide cursor
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 0;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);

	int enemyTimer    = T_ENEMY;
	int mushroomTimer = T_MUSHROOM;

	printf("\n\n\n\n");

	char key = NULL;
	while (key != ESC && g_lives >= 0) {
		//check input
		key = NULL;
		if (_kbhit()) {
			key = _getch();
		}

		//process input
		switch (key) {
			case MOVE_LEFT :g_character--; if (g_character < 0) g_character = 0;          break;
			case MOVE_RIGHT:g_character++; if (g_character > WIDTH) g_character = WIDTH;  break;
			case FIRE_LEFT :bullets.push_back(bullet{ g_character -1, -1});               break;
			case FIRE_RIGHT:bullets.push_back(bullet{ g_character +1,  1});               break;
		}
		checkCollisions();

		//move bullets
		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			it->pos += it->dir;
		}
		checkCollisions();
			
		//enemy movement and generation
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			it->pos += it->dir;
		}

		if (enemyTimer == 0) {
			if (rand() % 2) {
				enemies.push_back(enemy{0, 1});
			}
			else {
				enemies.push_back(enemy{WIDTH, -1});
			}
			enemyTimer = T_ENEMY;
		}
		else {
			enemyTimer--;
		}
		checkCollisions();

		//mushroom generation
		if (g_mushroom == -1) {
			if (mushroomTimer == 0) {
				g_mushroom = rand() % WIDTH;
				mushroomTimer = T_MUSHROOM;
			}
			else {
				mushroomTimer--;
			}
		}

		//Pintado
		printf("L: %d ", g_lives);
		for (unsigned int i = 0; i <= WIDTH; i++) {
			char *toDraw = "_";

			for (auto it = bullets.begin(); it != bullets.end(); ++it)
			{
				if (it->pos == i) {
					if (it->dir == -1) {
						toDraw = "<";
					}
					else {
						toDraw = ">";
					}
				}
			}

			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				if (it->pos == i) {
					toDraw = "@";
				}
			}

			if (i == g_character) {
				toDraw = "X";
			}
			
			if (i == g_mushroom) {
				toDraw = "T";
			}

			printf("%s", toDraw);
		}

		printf(" P: %d", g_points);
		printf("\r");

		Sleep(50);
	}

	if (g_lives < 0) {
		printf("GAME OVER");
		getchar();
	}

    return 0;
}

void checkCollisions()
{
	auto itBullets = bullets.begin();
	while (itBullets != bullets.end()) {
		//check if bullet is out of screen
		if (itBullets->pos > WIDTH || itBullets->pos < 0) {
			itBullets = bullets.erase(itBullets);
		}
		else {
			//check if bullet hits enemy
			auto itEnemies = enemies.begin();
			bool hit = false;
			while (!hit && itEnemies != enemies.end()) {
				if (itBullets->pos == itEnemies->pos) {
					hit = true;
					enemies.erase(itEnemies);

				}
				else {
					itEnemies++;
				}
			}

			if (hit) {
				itBullets = bullets.erase(itBullets);
			}
			else {
				itBullets++;
			}
		}
	}

	auto itEnemies = enemies.begin();
	while (itEnemies != enemies.end()) {
		//check if enemy is out of screen
		if (itEnemies->pos > WIDTH || itEnemies->pos < 0) {
			itEnemies = enemies.erase(itEnemies);
		}
		else {
			//check if enemy hits character
			if (g_character == itEnemies->pos) {
				//decrease life and reset
				g_lives--;
				g_character = (WIDTH / 2) - 1;
				enemies.clear();
				bullets.clear();
				g_mushroom = -1;
				break;
			}
			//check if enemy hits mushroom
			if (g_mushroom == itEnemies->pos) {
				g_mushroom = -1;
			}
			itEnemies++;
		}
	}

	//check if character picked mushroom
	if (g_character == g_mushroom) {
		g_mushroom = -1;
		g_points += 10;
	}
}