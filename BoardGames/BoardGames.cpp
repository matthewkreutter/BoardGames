/*
BoardGames.cpp

Contains the main function for the program.
Uses the GameBase class to implement a game
of tic tac toe, gomoku, or sudoku
*/

#include "stdafx.h"
#include "TicTacToeGame.h"
#include "GameBase.h"
#include "Utility.h"
#include <iostream>
using namespace std;

int main(int argc, char * argv[])
{
	try {
		GameBase::checkGame(argc, argv);
	}
	catch (int e) {
		usage(argv[program_name], "GameType");
		return e;
	}
	catch (const bad_alloc&) {
		return alloc_error;
	}
	catch (...) {
		return catch_all;
	}

	int result_code = GameBase::instance()->play();
	return result_code;
}
