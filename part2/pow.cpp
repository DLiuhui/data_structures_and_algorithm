/// @file    pow.cpp

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

bool isEven(int n)
{
	return n % 2 == 0;
}

long long pow(long long x, int n)
{
	if(n == 0)
		return 1;
	if(n == 1)
		return x;
	if(isEven(n))
		return pow(x * x, n / 2);
	else
		return pow(x * x, n / 2) * x;
}

int main(int argc, char** argv)
{
	cout << "2^61 = " << pow(2, 61) << endl;
	cout << "2^62 = " << pow(2, 62) << endl;
	return 0;
}
