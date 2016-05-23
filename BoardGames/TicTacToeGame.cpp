/*
TicTacToeGame.cpp

A subclass of GameBase, TicTacToeGame class which implements a
game of tic tac toe, with the X and O players
alternating turns.
*/

#include "stdafx.h"
#include "TicTacToeGame.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

TicTacToeGame::TicTacToeGame()
{
	moves_saved = "TicTacToe\n";
	width = 5;
	height = 5;
	buffer = 2;
	min_coord = 1;

	// fill the game_piece vector with empty game_piece structs
	for (int i = 0; i < height * width; ++i) {
		game_piece p = game_piece();
		board.push_back(p);
	}

	read_save_file("tictactoe.txt");
}

// check whether the game_pieces at the 3 indexes are all the same
// and all valid (i.e. not empty)
bool TicTacToeGame::all_same(int i1, int i2, int i3) {
	// check to make sure they are all valid (not empty)
	if (board[i1].is_valid() && board[i2].is_valid() && board[i3].is_valid()) {
		bool one_two_same = board[i1].display.compare(board[i2].display) == 0;
		bool two_three_same = board[i2].display.compare(board[i3].display) == 0;

		// if 1 and 2 are the same, and 2 and 3 are the same, then by
		// transitivity they are all the same
		return (one_two_same && two_three_same);
	}
	else {
		return false;
	}
}

// check if either player has won the game
bool TicTacToeGame::done() {
	for (int i = 1; i < (height - 1); ++i) {
		// check if 3 indexes of the ith row are the same
		int index1 = i * width + 1;
		int index2 = i * width + 2;
		int index3 = i * width + 3;
		if (all_same(index1, index2, index3)) {
			print_win_message();
			return true;
		}

		// check if 3 indexes of the ith column are the same
		int i1 = 1 * width + i;
		int i2 = 2 * width + i;
		int i3 = 3 * width + i;
		if (all_same(i1, i2, i3)) {
			print_win_message();
			return true;
		}
	}

	// now check the diagonals
	int i1 = 1 * width + 1;
	int i2 = 2 * width + 2;
	int i3 = 3 * width + 3;
	if (all_same(i1, i2, i3)) {
		print_win_message();
		return true;
	}

	i1 = 1 * width + 3;
	// i2 the same as before
	i3 = 3 * width + 1;
	if (all_same(i1, i2, i3)) {
		print_win_message();
		return true;
	}

	// no 3 in a row found
	return false;
}

// check if the game ended in a draw
bool TicTacToeGame::draw() {
	if (done()) {
		// if there's a winner, then it's not a draw
		return false;
	}
	else {
		for (int x = 1; x < (width - 1); ++x) {
			for (int y = 1; y < (height - 1); ++y) {
				int index = y * width + x;
				if (!board[index].is_valid()) {
					// found a non-empty square, so there's a 
					// possible move remaining
					return false;
				}
			}
		}
		// if we haven't returned yet, the whole board is filled
		// and nobody has won, so it's a draw
		write_no_save("tictactoe.txt"); // clear the save file
		return true;
	}
}


int TicTacToeGame::turn() {
	print();
	string piece_string;
	if (current_turn) {
		cout << "Player X's turn" << endl;
		piece_string = "X";
	}
	else {
		cout << "Player O's turn" << endl;
		piece_string = "O";
	}
	unsigned int x;
	unsigned int y;

	// get the coordinates from the user
	// if they enter a coordinate that is already taken
	// just keep asking them until the user enters a valid one or quits
	while (true) {
		int code = prompt(x, y);
		if (code == user_quit) {
			while (true) {
				cout << "Would you like to save the current game? (y/n) ";
				string y_n;
				cin >> y_n;
				if (y_n == "y") {
					save_game("tictactoe.txt");
					break;
				}
				else if (y_n == "n") {
					write_no_save("tictactoe.txt");
					break;
				}
			}
			return code;
		}

		if (is_square_empty(x, y)) {
			// valid user input, so break out of the while loop
			break;
		}
		else {
			cout << "That square is already taken." << endl;
		}
	}

	// put the player's piece on the game board
	int index = y * width + x;
	board[index].display = piece_string;
	board[index].name = piece_string;

	// add this move to the save string
	string space(" ");
	string newline("\n");
	string save_line = piece_string + space + to_string(x) + space + to_string(y) + newline;
	moves_saved += save_line;

	// update longest_display variable
	int string_length = board[index].display.length();
	if (string_length > longest_display) {
		longest_display = string_length;
	}

	// print out the game board and moves made
	// by the player who just had their turn
	print_player_moves(piece_string);
	cout << "" << endl;

	// switch the current_turn to the other player
	current_turn = !current_turn;
	return success;
}


// prints out all the valid moves a player has made in the game
void TicTacToeGame::print_player_moves(string player) {
	cout << "Player " << player << ": ";

	// loop through the positions on the game board
	for (int x = 1; x < (width - 1); ++x) {
		for (int y = 1; y < (height - 1); ++y) {
			int index = y * width + x;

			// if they match the player we want, print out the move
			if (board[index].display.compare(player) == 0) {
				cout << x << "," << y << "; ";
			}
		}
	}
	cout << "" << endl;
}

void TicTacToeGame::print() {
	cout << *this << endl;
}

// the turn flips the current_turn variable before the done method
// is called
// so who won is reversed from current_turn
void TicTacToeGame::print_win_message() {
	print();
	if (current_turn) {
		cout << "Player O wins" << endl << endl;
	}
	else {
		cout << "Player X wins" << endl << endl;
	}

	write_no_save("tictactoe.txt"); // save the 
}

// this function adapted from GameBoard.cpp print_game_board function
ostream & operator<< (ostream &os, const TicTacToeGame &game) {
	// loop through the height and width of the game board
	// and print out the pieces
	for (int y = game.height - 1; y >= 0; --y) {
		cout << y; // print the row number
		for (int x = 0; x < game.width; ++x) {
			unsigned int index = game.width * y + x;
			unsigned int max_index = game.width * game.height;
			if (index < max_index) {
				// print out the game piece at this position
				cout << setw(game.longest_display) << game.board[index].display;
			}
		}
		cout << "" << endl;
	}

	// print out the column number along the bottom
	cout << " ";
	for (int i = 0; i < game.width; ++i) {
		cout << setw(game.longest_display) << i;
	}
	cout << "" << endl;

	return os;
}

