/// @file    findMax.cpp
/// @date    2019-02-24 16:59:45

#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

template <typename Comparable>
const Comparable& findMax(const vector<Comparable>& a)
{
	int max_idx = 0;
	for(std::size_t i = 1; i < a.size(); ++i)
		if(a[max_idx] < a[i])
			max_idx = i;
	return a[max_idx];
}

int main(int argc, char** argv)
{
	vector<int>     v1 = { 3, 5, 8, 2, 4 };
	vector<double>  v2 = { 2.9, 5.4, 2.1, 3.3 };
	vector<string>  v3 = { "hello", "world", "apple" };
	/*
	 * vector <IntCellPtr> v4(75)p
	 * can't use findMax(v4)
	 * because IntCellPtr need to define operator
	 * as it member storedValue is int*
	 */

	cout << findMax(v1) << endl;
	cout << findMax(v2) << endl;
	cout << findMax(v3) << endl;

	return 0;
}
