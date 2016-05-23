/*
GamePiece.cpp

Implements the game_piece struct and its methods,
as well as the is_all_whitespace function.
*/

#include "stdafx.h"
#include "GamePiece.h"
#include <cctype>

// create a constructor for game piece
// makes name an empty string and display a space
game_piece::game_piece()
	: display(" "), name(""), editable(true) {}

// checks to see if its a valid, nonempty piece
bool game_piece::is_valid() {
	if (is_all_whitespace(display)) {
		// is the display string is all whitespace
		// this location is an empty square (no piece)
		return false;
	}

	if (name.empty()) {
		return false;
	}
	return true;
}


bool is_all_whitespace(const string &s) {
	for (auto c : s) {
		if (!isspace(c)) {
			return false; // found a non whitespace character so return false
		}
	}
	// haven't returned yet, so we didn't find a non whitespace character
	return true;
}