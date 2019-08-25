/// @file    matrixTest.cpp
/// @date    2019-02-24 18:11:52

#include "matrix.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	Matrix<double> mat(4,4);
	Matrix<double> mat2({
			{1,2,3,4},
			{5,6,7,8},
			{9,10,11,12},
			{13,14,15,16}
			});
	cout << mat << endl;
	cout << mat2 << endl;
	return 0;
}
