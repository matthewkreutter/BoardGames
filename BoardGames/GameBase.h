/*
GameBase.h
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Declares the GameBase class, including its
methods and instance variables.
*/

#pragma once
#include "GamePiece.h"
#include <vector>
#include <ostream>
#include <memory>

class GameBase
{
public:
	GameBase();
	static void checkGame(int, char *[]);
	int play();
	static shared_ptr<GameBase> instance();

protected:
	static shared_ptr<GameBase> game_pointer;

	int width;
	int height;
	int longest_display;
	int buffer;
	unsigned int min_coord;
	string moves_saved;
	bool current_turn = true;

	vector<game_piece> board;

	virtual void print() = 0;
	virtual bool done() = 0;
	virtual bool draw() = 0;
	virtual int turn() = 0;

	int prompt(unsigned int &x, unsigned int &y);
	bool is_square_empty(unsigned int, unsigned int);
	int save_game(string);
	int write_no_save(string);
	int read_save_file(string);
};



