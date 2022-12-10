#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include <string>
#include <conio.h>

void print_board(char board[9]) {
	system("cls");
	for (short x = 0; x < 9; x++) {
		std::cout << board[x];
		if ((x + 1) % 3 == 0) std::cout << std::endl;
	}
	std::cout << std::endl;
}

short pick_random_square(short squares_filled[3]) {
	short index;
	index = rand() % 9;
	for (short i = 0; i < 3; i++) {
		if (squares_filled[i] != '\0' && index == squares_filled[i]) {
			i = 0;
			index = rand() % 9;
		}
	}
	return index;
}


int main() {
	char board[9] = {'1','2','3','4','5','6','7','8','9'};
	srand(time(0));
	print_board(board);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	// pickking 3 random squares to start
	short squares_filled[3];
	for (short i = 0; i < 3; i++) {
		squares_filled[i] = pick_random_square(squares_filled);
		board[squares_filled[i]] = '#';
	}

	print_board(board);

	while (true) {
		char ch_user_square = _getch();
		short user_square = ((short)(ch_user_square) - 48) - 1;
		
		if (board[user_square] == '#') {
			board[user_square] = ch_user_square;
			
			//removes index
			for (short i = 0; i < 3; i++) {
				if (squares_filled[i] == user_square) {
					squares_filled[i] = pick_random_square(squares_filled);
					board[squares_filled[i]] = '#';
				}
			}
			
		}

		print_board(board);
	}
	

	return 0;
}