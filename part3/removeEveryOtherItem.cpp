#include <ctime>
#include <list>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::vector;

// 从第一个元素开始，间隔删除
template <typename Container>
void removeEveryOtherItem(Container& lst)
{
	// using decltype to infer the iterator type
    decltype(lst.begin()) itr = lst.begin();
	while(itr != lst.end())
	{
		itr = lst.erase(itr);
		if(itr != lst.end())
			++itr;
	}
}

template <typename Container>
void print(const Container& c, std::ostream& out = cout)
{
	if(c.empty())
		out << "(empty)" << endl;
	else
	{
		auto itr = std::begin(c);
		out << "[ " << *itr++;
		while(itr != std::end(c))
			out << ", " << *itr++;
		out << " ]" << endl;
	}
}

int main(int argc, char** argv)
{
	list<int> lst;
	for(int i = 0; i < 9; ++i)
		lst.push_back( i );

	removeEveryOtherItem(lst);
	print(lst, cout);

	clock_t start, end;

	for( int N = 100001; N <= 1000000; N *= 2 )
	{
		list<int> list1;
		vector<int> vec;

		for( int i = 0; i < N; ++i )
		{
            list1.push_back( i );
			vec.push_back( i );
		}

		start = clock( );
		removeEveryOtherItem( list1 );
		end = clock( );
		cout << "list " << N << " " << double(end-start)/CLOCKS_PER_SEC << endl;

		start = clock( );
		removeEveryOtherItem( vec );
		end = clock( );
		cout << "vector " << N << " " << double(end-start)/CLOCKS_PER_SEC << endl;
	}
	return 0;
}
