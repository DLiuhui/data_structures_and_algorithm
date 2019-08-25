/// @file    square_test.cpp
/// @date    2019-02-24 20:49:13
#include "square.h"
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main( )
{
	vector<Square> v = { Square{ 3.0 }, Square{ 2.0 }, Square{ 2.5 } };
	cout << "Largest square: " << findMax( v ) << endl;
	for(std::size_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	mySwap(v[0], v[1]);
	for(std::size_t i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
	return 0;
}
