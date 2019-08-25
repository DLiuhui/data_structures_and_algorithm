/// @file    binarySearch_test.cpp

#include "binarySearch.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::sort;
using std::vector;

int main(int argc, char** argv)
{
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	vector<int> vec100, vec1000, vec10000, vec100000;
	for(int i = 0; i < 100000; ++i)
	{
		if(i < 100)
		{
			vec100.push_back(1 + std::rand() % 100);
			vec1000.push_back(1 + std::rand() % 1000);
			vec10000.push_back(1 + std::rand() % 10000);
			vec100000.push_back(1 + std::rand() % 100000);
		}
		else if(i < 1000)
		{
			vec1000.push_back(1 + std::rand() % 1000);
			vec10000.push_back(1 + std::rand() % 10000);
			vec100000.push_back(1 + std::rand() % 100000);
		}
		else if(i < 10000)
		{
			vec10000.push_back(1 + std::rand() % 10000);
			vec100000.push_back(1 + std::rand() % 100000);
		}
		else
			vec100000.push_back(1 + std::rand() % 100000);
	}
	sort(vec100.begin(), vec100.end());
	sort(vec1000.begin(), vec1000.end());
	sort(vec10000.begin(), vec10000.end());
	sort(vec100000.begin(), vec100000.end());
	// analysis
	time_t start, end;
	start = std::clock();
	cout << binarySearch(vec100, 50) << " ";
	end = std::clock();
	cout << "size 100, time cost: " << double(end - start) / CLOCKS_PER_SEC << endl;
	start = std::clock();
	cout << binarySearch(vec1000, 500) << " ";
	end = std::clock();
	cout << "size 1000, time cost: " << double(end - start) / CLOCKS_PER_SEC << endl;
	start = std::clock();
	cout << binarySearch(vec10000, 5000) << " ";
	end = std::clock();
	cout << "size 10000, time cost: " << double(end - start) / CLOCKS_PER_SEC << endl;
	start = std::clock();
	cout << binarySearch(vec100000, 50000) << " ";
	end = std::clock();
	cout << "size 100000, time cost: " << double(end - start) / CLOCKS_PER_SEC << endl;
	return 0;
}
