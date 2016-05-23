/*
Utility.h

Implements the usage function for the program.
*/

#include "stdafx.h"
#include "Utility.h"
#include <iostream>
using namespace std;

// show the user the proper usage of this program
int usage(char * program, char * parameter) {
	cout << "Invalid arguments provided." << endl;
	cout << "Usage: " << program << " " << parameter << endl;
	return wrong_args;
}