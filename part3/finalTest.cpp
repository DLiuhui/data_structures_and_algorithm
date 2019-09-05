//
// Created by DDRHb on 2019/8/31.
//

#include "Vector.h"
#include "List.h"
#include "Stack.h"
#include "Queue.h"
#include <cstdlib>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using DS::Vector;
using DS::List;
using DS::Stack;
using DS::Queue;

static const int NUMS_PER_LINE = 14;

template <typename Collection>
void printCollection(const Collection& c)
{
    cout << "Collection contains: " << c.size() << " items" << endl;
    int i = 1;

    if(c.empty())
        cout << "Empty container." << endl;
    else
    {
        for(auto x : c)
        {
            cout << x << " ";
            if(i++ % NUMS_PER_LINE == 0)
                cout << endl;
        }
        cout << endl;

        if(c.size() > NUMS_PER_LINE)
            return;
        cout << "In reverse: " << endl;
        for(auto ritr = std::end( c ); ritr != std::begin( c );)
            cout << *(--ritr) << " ";
        cout << endl << endl;
    }
}

int jos( int people, int passes, List<int> & order )
{
    List<int> theList;
    List<int>::iterator p = std::begin( theList );
    List<int>::iterator tmp;
    Stack<int> s;
    Queue<int> q;

    order = List<int>{ };

    int i;
    for( i = people; i >= 1; --i )
        p = theList.insert( p, i );

    while( people-- != 1 )
    {
        for( i = 0; i < passes; ++i )
            if( ++p == std::end( theList ) )
                p = std::begin( theList );

        order.push_back( *p );
        s.push( *p );
        q.enQueue( *p );
        tmp = p;
        if( ++p == std::end( theList ) )
            p = std::begin( theList);
        theList.erase( tmp );
    }

    if( order.size( ) % 2 == 0 )
    {
        s.push( 0 );
        q.enQueue( 0 );
    }

    while( !s.empty( ) && !q.empty( ) )
    {
        int x, y;
        s.pop();
        x = s.top();
        q.deQueue();
        y = q.front();
        if( x == y )
            cout << "Middle removed is " << x << endl;
    }
    cout << "Only unremoved is ";
    return *std::begin( theList );
}

void print(const Vector<List<int>>& arr)
{
    int n_total = arr.size();
    for(int i = 0; i < n_total; ++i)
    {
        cout << "arr[" << i << "]:";
        for(auto x : arr[i])
            cout << " " << x;
        cout << endl;
    }
}

void nonsense(int people, int passes)
{
    List<int> last_few, the_list;
    cout << jos(people, passes, last_few) << endl;

    cout << "(Removal order)";
    printCollection(last_few);
}

// 定义自己的比较函数
class CompareList
{
public:
    bool operator() (const List<int>& lhs, const List<int>& rhs) const
    { return lhs.size() < rhs.size(); }
};

int main()
{
    const int N = 20;
    Vector<List<int>> arr(N);
    List<int> lst;

    for(int i = N - 1; i > 0; --i)
    {
        lst.push_back(i);
        arr[i] = lst;
    }

    print(arr);

    clock_t start = clock();
    std::sort(std::begin(arr), std::end(arr), CompareList{});
    clock_t end = clock();
    cout << "Sorting time: " << (end - start) << endl;

    print(arr);

    nonsense(12, 0);
    nonsense(12, 1);

    return 0;
}