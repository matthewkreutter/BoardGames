/*
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Defines the TicTacToeGame header, including methods and instance variable, and
the<< operator for the TicTacToeGame class.
*/

#include "stdafx.h"
#include "GameBase.h"

class TicTacToeGame : public GameBase {
public:
	TicTacToeGame();
	bool done();
	bool draw();
	int turn();

	virtual void print();

private:
	bool all_same(int i1, int i2, int i3);
	void print_player_moves(string player);
	void print_win_message();
	friend ostream & operator<< (ostream &os, const TicTacToeGame &game);
};

ostream & operator<< (ostream &os, const TicTacToeGame &game);
