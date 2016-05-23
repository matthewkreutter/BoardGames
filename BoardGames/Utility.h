/*
Utility.h
Authors: Aaron Gordon, Kevin Susai, Matthew Kreutter
Emails: aarongordon@wustl.edu, kevin.susai@wustl.edu, matthew.kreutter@wustl.edu

Declares the useful constant used in the program,
such as error codes, and the usage message.
*/

enum {
	success, user_quit, draw_game, wrong_args, catch_all, alloc_error, could_not_open_file,
	no_data_saved
};
enum { program_name, game_name, board_size, win_length };
enum exceptions { no_errors, nullpointer, not_nullpointer, board_size_error, win_not_possible, wrong_num_args, invalid_game };

const int expected_num_args = 2;
const int min_board_size = 1;
const int max_board_size = 50;
const int max_win_length = 3;

// show a usage message
int usage(char * program, char * parameter);


