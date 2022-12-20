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

struct Changes {
	char board[3][3];
	Coords squares_filled[3];

	void set_changes(char _board[3][3], Coords _squares_filled[3]) {
		for (short y = 0; y < 3; y++) {
			for (short x = 0; x < 3; x++) {
				board[y][x] = _board[y][x];
			}
			squares_filled[y] = _squares_filled[y];
		}
	}
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

void count_down(short seconds) {
	for (short second = 0; second < seconds; second++) {
		system("cls");
		std::cout << seconds - second;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
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

char menu() {
	char option;
	bool valid_move = false;
	char valid_moves[] = { '1'};

	while (!valid_move) {
		system("cls");
		std::cout << "Menu\n";
		std::cout << "1) Timed Mode\n";
		std::cout << std::endl;
		std::cout << ">> ";
		option = _getch();
		for (char _valid_move : valid_moves) if (option == _valid_move) valid_move = true;

	}

	return option;

}


Changes init_game(char board[3][3]) {
	// picking 3 random squares to start
	Coords squares_filled[3];
	Changes changes;
	for (short i = 0; i < 3; i++) {
		squares_filled[i] = pick_random_square(board);
		board[squares_filled[i].y][squares_filled[i].x] = '#';
	}

	changes.set_changes(board, squares_filled);

	return changes;
}

Changes run_frame(char board[3][3], Coords squares_filled[3]) {
	Coords coords = user_move(board);
	Changes changes;

	//replaces square
	for (short i = 0; i < 3; i++) {
		if (squares_filled[i].x == coords.x && squares_filled[i].y == coords.y) {
			squares_filled[i] = pick_random_square(board);
			board[squares_filled[i].y][squares_filled[i].x] = '#';
		}
	}

	board[coords.y][coords.x] = 'O';

	changes.set_changes(board, squares_filled);

	return changes;
}


int main() {
	while (true) {
		Coords squares_filled[3];
		char board[3][3] = {
			{'O','O','O'},
			{'O','O','O'},
			{'O','O','O' } };

		increase_text_size();
		srand(time(0));


		char option = menu();

		if (option == '1') {
			count_down(3);
		}
		auto start = std::chrono::high_resolution_clock::now();


		Changes changes = init_game(board);
		for (short y = 0; y < 3; y++) {
			for (short x = 0; x < 3; x++) {
				board[y][x] = changes.board[y][x];
			}
			squares_filled[y] = changes.squares_filled[y];
		}

		print_board(board);

		for (short round = 0; round < 100; round++) {

			Changes frame = run_frame(board, squares_filled);

			for (short y = 0; y < 3; y++) {
				for (short x = 0; x < 3; x++) {
					board[y][x] = frame.board[y][x];
				}
				squares_filled[y] = frame.squares_filled[y];
			}

			print_board(board);
		}
		auto end = std::chrono::high_resolution_clock::now();
		float time_taken = (float)std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/(float)1000;
		system("cls");
		std::cout << "Time: " <<  time_taken << "s\n";
		char x;
		std::cin >> x;
	}
	return 0;
}