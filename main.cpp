#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include <string>
#include <conio.h>
#include <Windows.h>
#include <iomanip>

#define set_text_red std::cout << "\033[0;31m"
#define set_text_green std::cout << "\033[0;32m"
#define set_text_cyan std::cout << "\033[0;36m"

struct Coords {
	short x;
	short y;
};

void increase_text_size() {
	static CONSOLE_FONT_INFOEX  fontex;
	fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetCurrentConsoleFontEx(hOut, 0, &fontex);
	fontex.FontWeight = 700;
	fontex.dwFontSize.X = 36;
	fontex.dwFontSize.Y = 36;
	SetCurrentConsoleFontEx(hOut, NULL, &fontex);
}

void print_board(char board[3][3]) {
	system("cls");
	short space = 52;
	set_text_red;

	//for (short lines_down = 0; lines_down < 10; lines_down++) {
	//	std::cout << std::endl;
	//}
	
	//std::cout << std::setw(space);
	std::cout << "|";
	for (short y = 0; y < 3; y++) {
		
		for (short x = 0; x < 3; x++) {
			if (board[y][x] == '#') set_text_green;
			else set_text_cyan;
			std::cout << board[y][x];
			set_text_red;
			std::cout << "|";
			
		}
		
		if (y != 2) {
			//std::cout << std::setw(space);
			std::cout << std::endl << "|";
		}
		
	}
	std::cout << std::endl;
}

Coords pick_random_square(char board[3][3]) {
	Coords coords;
	coords.x = rand() % 3;
	coords.y = rand() % 3;
	//makes sure not already picked
	bool picked = false;
	do {
		if (board[coords.y][coords.x] == '#') {
			coords.x = rand() % 3;
			coords.y = rand() % 3;
		}
		else picked = true;
	} while (!picked);
	return coords;
}

Coords user_move(char board[3][3]) {
	char board_template[3][3] = {
		{'7','8','9'},
		{'4','5','6'},
		{'1','2','3' } };

	char user_square;

	while (true) {
		user_square = _getch();
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				if (user_square == board_template[y][x] && board[y][x] == '#') {
					Coords position;
					position.x = x;
					position.y = y;
					return position;
				}
			}
		}
	}

}
int main() {
	char board[3][3] = {
		{'O','O','O'},
		{'O','O','O'},
		{'O','O','O' } };

	increase_text_size();
	srand(time(0));
	print_board(board);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// pickking 3 random squares to start
	Coords squares_filled[3];
	for (short i = 0; i < 3; i++) {
		squares_filled[i] = pick_random_square(board);
		board[squares_filled[i].y][squares_filled[i].x] = '#';
	}

	print_board(board);

	short score = 0;
	while (true) {
		
		Coords coords = user_move(board);

		//removes index
		for (short i = 0; i < 3; i++) {
			if (squares_filled[i].x == coords.x && squares_filled[i].y == coords.y) {
				squares_filled[i] = pick_random_square(board);
				board[squares_filled[i].y][squares_filled[i].x] = '#';
				/*std::cout << "Y: " << squares_filled[i].y << std::endl;
				std::cout << "X: " << squares_filled[i].x << std::endl;
				system("pause");*/
				score++;
			}
		}

		board[coords.y][coords.x] = 'O';
		print_board(board);
	}

	return 0;
}