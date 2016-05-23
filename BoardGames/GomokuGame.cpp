/*
GomokuGame.cpp

Subclass of the GameBase that implements the game of Gomoku
*/

#include "stdafx.h"
#include "GomokuGame.h"
#include "Utility.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

GomokuGame::GomokuGame(int size, int num_in_a_row) {
	moves_saved = "Gomoku\n";
	height = size;
	width = size;
	in_a_row = num_in_a_row;
	buffer = 0;
	min_coord = 1;
	// fill the game_piece vector with empty game_piece structs
	for (int i = 0; i < height * width; ++i) {
		game_piece p = game_piece();
		board.push_back(p);
	}

	read_save_file("gomoku.txt");
}

void GomokuGame::print() {
	cout << *this << endl;
}

bool GomokuGame::draw() {
	write_no_save("gomoku.txt");
	return !done() && turn_count == width * height;
}

int GomokuGame::turn() {
	print();
	string piece_string;

	if (current_turn) {
		cout << "Player Black's turn" << endl;
		piece_string = "B";
	}
	else {
		cout << "Player White's turn" << endl;
		piece_string = "W";
	}

	unsigned int x;
	unsigned int y;

	while (true) {
		int code = prompt(x, y);
		if (code == user_quit) {
			while (true) {
				cout << "Would you like to save the current game? (y/n) ";
				string y_n;
				cin >> y_n;
				if (y_n == "y") {
					save_game("gomoku.txt");
					break;
				}
				else if (y_n == "n") {
					write_no_save("gomoku.txt");
					break;
				}
			}

			return user_quit;
		}

		// increment x and y to match the index (because Gomoku boards go from 1-19 instead of 0-18)
		x--;
		y--;

		if (is_square_empty(x, y)) {
			break;
		}
		else {
			cout << "That square is taken. Choose a different square" << endl;
		}
	}

	int index = y * width + x;
	board[index].display = piece_string;
	board[index].name = piece_string;
	turn_count++;

	// add this move to the save string
	// note: this stores the indexes in the array, not what the user entered
	string space(" ");
	string newline("\n");
	string save_line = piece_string + space + to_string(x) + space + to_string(y) + newline;
	moves_saved += save_line;

	// update longest_display variable
	int string_length = board[index].display.length();
	if (string_length > longest_display) {
		longest_display = string_length;
	}

	current_turn = !current_turn;
	return success;
}

bool GomokuGame::winner_in_row(int row_height) {
	game_piece first_piece = board[row_height * width];
	string last = first_piece.display; // the first one
	int num_in_row;

	if (first_piece.is_valid()) {
		num_in_row = 1;
	}
	else {
		num_in_row = 0;
	}

	for (int x = 1; x < width; x++) {
		game_piece current_piece = board[row_height * width + x];
		if (current_piece.is_valid()) {
			if (current_piece.display == last) {
				num_in_row++;
				if (num_in_row == in_a_row)
				{
					return true;
				}
			}
			else
			{
				num_in_row = 1;
			}
		}
		else
		{
			num_in_row = 0;
		}
		last = current_piece.display;
	}
	return false;
}

bool GomokuGame::winner_in_col(int col_width) {
	game_piece first_piece = board[col_width];
	string last = first_piece.display; // the first one
	int num_in_row;

	if (first_piece.is_valid()) {
		num_in_row = 1;
	}
	else {
		num_in_row = 0;
	}

	for (int y = 1; y < height; y++) {
		game_piece current_piece = board[y * width + col_width];
		if (current_piece.is_valid()) {
			if (current_piece.display == last) {
				num_in_row++;
				if (num_in_row == in_a_row)
				{
					return true;
				}
			}
			else
			{
				num_in_row = 1;
			}
		}
		else
		{
			num_in_row = 0;
		}
		last = current_piece.display;
	}
	return false;
}

bool GomokuGame::winner_in_diag(int row, int col)
{
	// change to starting by an index position (row, col)
	game_piece first_piece = board[row * width + col];
	string last = first_piece.display; // the first one
	int num_in_row;

	if (first_piece.is_valid()) {
		num_in_row = 1;
	}
	else {
		num_in_row = 0;
	}

	int max_i = min(width - col, height - row);
	for (int i = 1; i < max_i; ++i) {
		int x = col + i;
		int y = row + i;
		game_piece current_piece = board[y * width + x];
		if (current_piece.is_valid()) {
			if (current_piece.display == last) {
				num_in_row++;
				if (num_in_row == in_a_row)
				{
					return true;
				}
			}
			else
			{
				num_in_row = 1;
			}
		}
		else
		{
			num_in_row = 0;
		}
		last = current_piece.display;
	}
	return false;
}

bool GomokuGame::winner_in_diag_neg_slope(int row, int col)
{
	// change to starting by an index position (row, col)
	game_piece first_piece = board[row * width + col];
	string last = first_piece.display; // the first one
	int num_in_row;

	if (first_piece.is_valid()) {
		num_in_row = 1;
	}
	else {
		num_in_row = 0;
	}

	int max_i = min(width - col, row);
	for (int i = 1; i < max_i; ++i) {
		int x = col + i;
		int y = row - i;
		game_piece current_piece = board[y * width + x];
		if (current_piece.is_valid()) {
			if (current_piece.display == last) {
				num_in_row++;
				if (num_in_row == in_a_row)
				{
					return true;
				}
			}
			else
			{
				num_in_row = 1;
			}
		}
		else
		{
			num_in_row = 0;
		}
		last = current_piece.display;
	}
	return false;
}

bool GomokuGame::done()
{
	for (int i = 0; i < width; ++i) {
		if (winner_in_diag(0, i)) {
			show_win_message();
			return true;
		}
		if (winner_in_diag(i, 0)) {
			show_win_message();
			return true;
		}

		if (winner_in_diag_neg_slope(i, 0)) {
			show_win_message();
			return true;
		}

		if (winner_in_diag_neg_slope(height - 1, i)) {
			show_win_message();
			return true;
		}
	}

	for (int i = 0; i < height; ++i)
	{
		if (winner_in_row(i))
		{
			show_win_message();
			return true;
		}
	}
	for (int i = 0; i < width; ++i)
	{
		if (winner_in_col(i))
		{
			show_win_message();
			return true;
		}
	}

	return false;
}

void GomokuGame::show_win_message() {
	print();
	if (current_turn) {
		cout << "White wins!" << endl << endl;
	}
	else {
		cout << "Black wins!" << endl << endl;
	}

	write_no_save("gomoku.txt"); // write "NODATA" since the game ended
}

ostream & operator<< (ostream &os, GomokuGame game) {
	// loop through the height and width of the game board
	// and print out the pieces
	for (int y = game.height - 1; y >= 0; --y) {
		os << y + 1; // print the row number
		for (int x = 0; x < game.width; ++x) {
			unsigned int index = game.width * y + x;
			unsigned int max_index = game.width * game.height;
			if (index < max_index) {
				// print out the game piece at this position
				os << setw(game.longest_display) << game.board[index].display;
			}
		}
		os << "" << endl;
	}
	// print out the column number along the bottom
	os << "X";
	for (int i = 1; i <= game.width; ++i) {
		os << setw(game.longest_display) << i;
	}
	os << "" << endl;

	return os;
}
