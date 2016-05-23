/*
SudokuGame.cpp

Subclass of the GameBase that implements the game of Sudoku
*/
#include "stdafx.h"
#include "SudokuGame.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

SudokuGame::SudokuGame()
{
	height = 9;
	width = 9;
	min_coord = 0;
	buffer = 1;

	// fill the game_piece vector with empty game_piece structs
	for (int i = 0; i < height * width; ++i) {
		game_piece p = game_piece();
		p.display = " ";
		board.push_back(p);
	}

	//Load initial game state, and then overlay our saved game
	read_sudoku_file("sudoku0.txt", false);
	read_sudoku_file("sudoku.txt", true);
}

void SudokuGame::print() {
	cout << *this << endl;
}

bool SudokuGame::done() {
	bool win = check_rows() && check_cols() && check_squares();
	if (win) {
		cout << "You Win!" << endl;
	}
	return win;
}

//You can never draw in Sudoku...
bool SudokuGame::draw() {
	return false;
}

int SudokuGame::turn() {
	cout << endl;
	print();
	cout << endl;

	unsigned int x;
	unsigned int y;
	unsigned int num;
	int code = prompt(x, y, num);
	if (code == user_quit) {
		while (true) {
			cout << "Would you like to save the current game? (y/n) ";
			string y_n;
			cin >> y_n;
			if (y_n == "y") {
				save_sudoku_game();
				break;
			}
			else if (y_n == "n") {
				//write "NODATA" to our save file, sudoku.txt
				write_no_save("sudoku.txt");
				break;
			}
		}

		return user_quit;
	}

	int index = y * width + x;
	board[index].display = to_string(num);
	board[index].name = to_string(num);

	return success;
}

int SudokuGame::prompt(unsigned int &x, unsigned int &y, unsigned int &num) {
	while (true) {
		//use super of prompt from GameBase
		int code = GameBase::prompt(x, y);
		if (code == user_quit) {
			return user_quit;
		}
		int index = y * width + x;
		//Check to make sure that this was not a default square, as the user should not be changing these
		if (!board[index].editable) {
			cout << "Cannot change that square" << endl;
		}
		else {
			break;
		}
	}

	while (true) {
		cout << "Enter the number: ";
		string user_input;
		getline(cin, user_input);
		istringstream iss(user_input);
		int n;
		if (iss >> n) {
			num = n;
			if (num >= 1 && num <= 9) {
				return success;
			}
			else {
				cout << "Enter a number between 1 and 9" << endl;
			}
		}
		else {
			cout << "Enter a number between 1 and 9" << endl;
		}
	}
}

//Iterates up the rows to check if all rows properly have 1-9
bool SudokuGame::check_rows() {
	for (int i = 0; i < height; i++)
	{
		vector<int> rowVec;
		//Put all 9 items in the row into a vector
		for (int j = 0; j < width; j++)
		{
			istringstream iss(board[j + (i*width)].display);
			int n;
			iss >> n;
			rowVec.push_back(n);
		}
		//Sort the vector, and then compare each item to 1-9 to check if this row is done
		sort(rowVec.begin(), rowVec.end());
		for (unsigned int i = 0; i < rowVec.size(); i++)
		{
			if (rowVec[i] != (i + 1))
			{
				return false;
			}
		}
	}
	return true;
}

//Same as check_rows(), but for columns
bool SudokuGame::check_cols() {
	//check cols
	for (int x = 0; x < width; x++)
	{
		vector<int> colVec;
		for (int y = 0; y < height; y++)
		{
			int index = y * width + x;
			istringstream iss(board[index].display);
			int n;
			iss >> n;
			colVec.push_back(n);
		}
		sort(colVec.begin(), colVec.end());
		for (unsigned int i = 0; i < colVec.size(); i++)
		{
			if (colVec[i] != (i + 1))
			{
				return false;
			}
		}
	}
	return true;
}

// check that each 3x3 square has 1-9
bool SudokuGame::check_squares() {
	for (int x = 0; x <= 6; x += 3) {
		for (int y = 0; y <= 6; y += 3) {
			if (!check_square(x, y)) {
				return false;
			}
		}
	}
	return true;
}

// (x, y) is the lower left hand corner of the 3x3 box to check
// if that box is correct
bool SudokuGame::check_square(int x_coord, int y_coord) {
	vector<int> v;
	// 2 for loops to loop over the box
	for (int x = x_coord; x <= (x_coord + 2); x++) {
		for (int y = y_coord; y <= (y_coord + 2); y++) {
			int index = y * width + x;
			istringstream iss(board[index].display);
			int n;
			iss >> n;
			// put items into a vector
			v.push_back(n);
		}
	}

	// sort vector, and then compare with 1-9
	sort(v.begin(), v.end());
	for (int i = 0; i < width; i++) {
		if (v[i] != (i + 1)) {
			return false;
		}
	}
	return true;
}

int SudokuGame::read_sudoku_file(string filename, bool editable) {
	string line;
	ifstream myfile(filename);

	if (myfile.is_open()) {
		for (int i = 8; i >= 0; i--) {
			getline(myfile, line);

			if (i == 8 && line == "NODATA") {
				return no_data_saved;
			}

			istringstream iss(line);
			for (int j = 0; j <= 8; j++) {
				string value;
				iss >> value;
				int index = i * width + j;

				// editable is true, so we're loading squares changed by the user
				// so ignore 0's
				if (editable) {
					if (value != "0") {
						board[index].editable = editable;
						board[index].display = value;
						board[index].name = value;
					}
				}
				// editable is false, so we're loading the squares from the initial game state
				else {
					if (value == "0") {
						board[index].editable = true;
						board[index].display = " ";
						board[index].name = " ";
					}
					else {
						board[index].editable = editable;
						board[index].display = value;
						board[index].name = value;
					}
				}
				board[index].name = value;
			}
		}
		myfile.close();
		return success;
	}
	else {
		cout << "Sudoku File could not be opened." << endl;
		return could_not_open_file;
	}
}

//We write our Sudoku game to file the same way that Sudoku0.txt is formatted
// However, we only write the squares that the user is able to change
int SudokuGame::save_sudoku_game() {
	ofstream ofs("sudoku.txt");
	if (ofs.is_open()) {
		for (int y = 8; y >= 0; y--) {
			for (int x = 0; x < width; x++) {
				int index = y * width + x;
				string square = board[index].display;

				if (square == " ") {
					ofs << "0 ";
				}
				else {
					// here, only write squares changed by user
					// when loading, load sudoku0, then the saved file
					if (board[index].editable) {
						ofs << board[index].display << " ";
					}
					else {
						ofs << "0 "; // 0 represents a blank square
					}
				}
			}
			ofs << endl;
		}
		ofs.close();
		return success;
	}
	else {
		return could_not_open_file;
	}
}

ostream & operator<< (ostream &os, SudokuGame game) {
	for (int y = game.width - 1; y >= 0; y--) {
		os << y << ")||"; // print the index
		for (int x = 0; x <= game.width - 1; x++) {
			int index = y * game.width + x;
			if ((x + 1) % 3 == 0) {
				os << game.board[index].display << "||";
			}
			else {
				os << game.board[index].display << "|";
			}
		}
		os << endl;

		// print the horizontal diving line
		if (y % 3 == 0) {
			os << "=)||";
		}
		else {
			os << "-)||";
		}

		for (int x = 0; x <= game.width - 1; x++) {
			if (y % 3 == 0) {
				os << "=";
			}
			else {
				os << "-";
			}

			if ((x + 1) % 3 == 0) {
				os << "||";
			}
			else {
				os << "|";
			}
		}
		os << endl;
	}

	// print the index numbers on the bottom
	os << "#)||";
	for (int x = 0; x <= game.width - 1; x++) {
		os << x;
		if ((x + 1) % 3 == 0) {
			os << "||";
		}
		else {
			os << "|";
		}
	}

	return os;
}
