/// @file    LambdaExample.cpp

// copy from source code
// using lambda
#include <string.h>
#include <vector>
#include <iostream>
#include <string>
#include <functional>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

// Generic findMax, with a function object, C++ style.
template <typename Object, typename Comparator>
const Object& findMax( const vector<Object> & arr, Comparator isLessThan )
{
	int maxIndex = 0;

	for( std::size_t i = 1; i < arr.size( ); ++i )
		if( isLessThan( arr[ maxIndex ], arr[ i ] ) )
			maxIndex = i;

	return arr[ maxIndex ];

}

template <typename Object>
const Object& findMax( const vector<Object> & arr )
{
	return findMax( arr, std::less<Object>( ) );
}

int main(int argc, char** argv)
{
	vector<string> arr = { "ZEBRA", "alligator", "crocodile" };
	cout << findMax(arr,
			[](const string& lhs, const string& rhs) {return strcmp(lhs.c_str(), rhs.c_str()) < 0; })
			<< endl;
	cout << findMax(arr) << endl;
	return 0;
}
