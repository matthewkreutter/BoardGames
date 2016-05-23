/*
GomokuGame.h
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Defines the GomokuGame class, including its methods and instance variables.
*/


#include "GameBase.h"
#include <ostream>

class GomokuGame : public GameBase {
public:
	GomokuGame(int size = 19, int num_in_a_row = 5);
	virtual void print();
	virtual bool done();
	virtual bool draw();
	virtual int turn();

protected:
	int turn_count = 0;
	int in_a_row = 5;

	bool winner_in_row(int);
	bool winner_in_col(int);
	bool winner_in_diag(int, int);
	bool winner_in_diag_neg_slope(int, int);
	void show_win_message();

	friend ostream & operator<< (ostream &, GomokuGame);
};

ostream & operator<< (ostream &os, GomokuGame g);
