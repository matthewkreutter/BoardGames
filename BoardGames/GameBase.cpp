/*
GameBase.cpp

Defines the GameBase class which provides a base for the TicTacToeGame, SudokuGame,
and GomokuGame classes
*/

#include "stdafx.h"
#include "GameBase.h"
#include "Utility.h"
#include "TicTacToeGame.h"
#include "GomokuGame.h"
#include "SudokuGame.h"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

shared_ptr<GameBase> GameBase::game_pointer = nullptr;

shared_ptr<GameBase> GameBase::instance() {
	if (GameBase::game_pointer != nullptr) {
		return GameBase::game_pointer;
	}
	else {
		return nullptr;
	}
}

GameBase::GameBase() :
	longest_display(3) {}

int GameBase::play() {
	int num_turns = 0;
	while (true) {
		// have the player take their turn
		int code = turn();
		if (code == user_quit) {
			cout << "There were " << num_turns << " turns played." << endl;
			cout << "The user quit." << endl;
			return user_quit;
		}

		num_turns++;
		if (done()) {
			return success;
		}

		// the game ends in a draw
		if (draw()) {
			print();
			cout << "It's a draw." << endl;
			cout << "There were " << num_turns << " turns played." << endl;
			return draw_game;
		}

	}
}

int GameBase::prompt(unsigned int &x, unsigned int &y) {
	while (true) {
		cout << "Enter a coordinate (or type 'quit' to quit): ";
		string user_input;
		getline(cin, user_input);
		if (user_input.compare("quit") == 0) {
			return user_quit;
		}
		else {
			// find the index of the comma
			size_t comma_index = user_input.find(",");
			if (comma_index != string::npos) {
				// now replace comma with a space
				user_input.replace(comma_index, 1, " ");

				// wrap in istringstream and then extract the coordinates
				istringstream iss(user_input);
				if (iss >> x >> y) {
					unsigned int max_width = width - buffer;
					unsigned int max_height = height - buffer;

					// check to make sure the coordinates are in the bounds of the board
					if (x >= min_coord && x <= max_width && y >= min_coord && y <= max_height) {
						return success;
					}
					else {
						cout << "Coordinate out of bounds. Must be within bounds of board." << endl;
					}
				}
				else {
					cout << "Invalid input. Please input 2 integers separated by a comma." << endl;
				}

			}
			else {
				cout << "Invalid input. Please input 2 integers separated by a comma." << endl;
			}
		}
	}
}

bool GameBase::is_square_empty(unsigned int x, unsigned y) {
	int index = y * width + x;
	return !board[index].is_valid();
}

int GameBase::save_game(string filename) {
	ofstream save_file;
	save_file.open(filename);

	if (save_file.is_open()) {
		save_file << moves_saved;
		save_file.close();
		return success;
	}
	else {
		return could_not_open_file;
	}

}

int GameBase::write_no_save(string filename) {
	ofstream save_file;
	save_file.open(filename);

	if (save_file.is_open()) {
		save_file << "NODATA";
		save_file.close();
		return success;
	}
	return could_not_open_file;
}

//Read save files for TicTacToe and Gomoku
int GameBase::read_save_file(string filename) {
	string line;
	ifstream myfile(filename);
	string gamepiece;
	int x_coordinate;
	int y_coordinate;

	if (myfile.is_open()) {
		getline(myfile, line);
		string game_type = line;
		if (game_type == "NODATA") {
			return no_data_saved;
		}
		// create a new game of type "game_type"
		while (getline(myfile, line)) {
			istringstream iss(line);
			iss >> gamepiece;
			iss >> x_coordinate;
			iss >> y_coordinate;

			// add piece to vector
			int index = y_coordinate * width + x_coordinate;
			board[index].display = gamepiece;
			board[index].name = gamepiece;

			// put the moves loaded into the string that stores the moves so far
			moves_saved = moves_saved + gamepiece + " " + to_string(x_coordinate) + " " + to_string(y_coordinate) + "\n";
			current_turn = !current_turn;
		}

		myfile.close();
		return success;
	}
	else {
		cout << "File could not be opened." << endl;
		return could_not_open_file;
	}
}

void GameBase::checkGame(int argc, char * argv[]) {
	if (game_pointer != nullptr) {
		//throw exceptions::not_nullpointer;
	}

	if (argc >= expected_num_args)
	{
		string arg1(argv[game_name]);
		string tictactoe("TicTacToe");
		string gomoku("Gomoku");
		string sudoku("Sudoku");

		if (arg1 == tictactoe)
		{
			if (argc == expected_num_args) {
				game_pointer = make_shared<TicTacToeGame>();
			}
		}
		else if (arg1 == gomoku) {
			// user specified board size
			if (argc == 2) {
				game_pointer = make_shared<GomokuGame>();
			}

			else if (argc == 3) {
				string string_size(argv[board_size]);
				istringstream iss(string_size);
				int board_size;
				if (iss >> board_size) {
					if (board_size >= min_board_size && board_size <= max_board_size) {
						game_pointer = make_shared<GomokuGame>(board_size);
					}
					else {
						cout << "Board size must be between 1 and 40" << endl;
						throw (int)exceptions::board_size_error;
					}
				}
				else {
					cout << "Board size must be an integer" << endl;
					throw (int)exceptions::board_size_error;
				}
			}
			// user specified board size and winning length
			else if (argc == 4) {
				string string_size(argv[board_size]);
				string winning_length(argv[win_length]);
				string both = string_size + " " + winning_length;

				int board_size, win_length;
				istringstream iss(both);
				if (iss >> board_size >> win_length) {
					if (board_size >= min_board_size && board_size <= max_board_size) {
						if (win_length >= max_win_length && win_length <= board_size) {
							game_pointer = make_shared<GomokuGame>(board_size, win_length);
						}
						else {
							cout << "Win length greater than board size. Winning not possible" << endl;
							throw (int)exceptions::win_not_possible;
						}
					}
					else {
						cout << "Board size must be between 1 and 40" << endl;
						throw (int)exceptions::board_size_error;
					}
				}
				else {
					cout << "Board size must be an integer" << endl;
					throw (int)exceptions::board_size_error;
				}
			}
			else {
				throw (int)exceptions::wrong_num_args;
			}
		}
		else if (arg1 == sudoku) {
			game_pointer = make_shared<SudokuGame>();

		}
		else {
			throw (int)exceptions::invalid_game;
		}
	}
	else
	{
		throw (int)exceptions::wrong_num_args;
	}
}
