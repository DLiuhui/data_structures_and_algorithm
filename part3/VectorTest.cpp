//
// Created by DDRHb on 2019/8/25.
//
#include "Vector.h"
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <ctime>

using std::cin;
using std::cout;
using std::endl;
using DS::Vector;

template <typename Object>
void print( const Vector<Vector<Object>>& arr )
{
    int n_vecs = arr.size( );

    for(int i = 0; i < n_vecs; ++i)
    {
        cout << "arr[" << i << "]:";
        for(size_t j = 0; j < arr[i].size(); ++j)
            cout << " " << arr[i][j];
        cout << endl;
    }
}

class CompareVector
{
public:
    bool operator() (const Vector<int>& lhs, const Vector<int>& rhs)
    { return lhs.size() < rhs.size(); }
};

int main()
{
    const int N = 20;
    Vector<Vector<int>> arr(N);
    Vector<int> v;

    for(int i = N - 1; i > 0; --i)
    {
        v.push_back(i);
        arr[i] = v;  // 此处执行的是拷贝过程（v仍旧要使用），copy =过程调用swap，调用了两次move
    }

    print(arr);

    clock_t start = clock();
    std::sort(arr.begin(), arr.end(), CompareVector{ });
    clock_t end = clock();
    printf("Sorting time: %ld\n", end - start);

    print(arr);

    return 0;
}

