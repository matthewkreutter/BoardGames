/*
SudokuGame.h
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Defines the SudokuGame class, including its methods and instance variables.
*/

#include "GameBase.h"
class SudokuGame : public GameBase
{
public:
	SudokuGame();
	friend ostream & operator<< (ostream &, SudokuGame);

	virtual void print();
	virtual bool done();
	virtual bool draw();
	virtual int turn();

	int read_sudoku_file(string, bool);
	int save_sudoku_game();
	bool check_rows();
	bool check_cols();
	bool check_square(int, int);
	bool check_squares();

	int prompt(unsigned int &x, unsigned int &y, unsigned int &num);
};

ostream & operator<< (ostream &os, SudokuGame game);