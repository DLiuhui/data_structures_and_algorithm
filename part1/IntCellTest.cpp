/// @file    TestIntCell.cpp

#include "IntCell.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	IntCell m;
	m.write(5);
	cout << "Cell contents: " << m.read() << endl;
	IntCellPtr pm;
	cout << "Cell ptr contents: " << pm.read() << endl;
	pm.write(3);
	IntCellPtr pm2{pm};
	cout << "Cell ptr contents: " << pm2.read() << endl;
	return 0;
}
