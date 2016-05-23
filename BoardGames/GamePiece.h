/*
GamePiece.h
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Defines the game_piece struct. Mostly the same as from
Lab 1, although some parts have been removed. Defines
instance variables and the is_all_whitespace function.
*/

#include <string>
using namespace std;

bool is_all_whitespace(const string &s);

// this struct represents a single game piece
// with color, name, and display
struct game_piece {
	game_piece();
	bool is_valid();
	string name;
	string display;
	bool editable;
};
