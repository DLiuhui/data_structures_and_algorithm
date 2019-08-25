/// @file    1_02.cpp

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

inline void printDigit(int n)
{
	cout << n;
}

void printOut(int n)
{
	if(n >= 10)
		printOut(n / 10);
	printDigit(n % 10);
}

int main(int argc, char** argv)
{
	printOut(1369);
	cout << endl;
	return 0;
}
