#include "left_heap.hpp"
#include <iostream>
using namespace std;
using DS::LeftistHeap;

int main()
{
    int numItems = 10000;
    LeftistHeap<int> h;
    LeftistHeap<int> h1;
    LeftistHeap<int> h2;
    int i = 37;

    cout << "Begin test..." << endl;
    for(i = 37; i != 0; i = (i + 37) % numItems)
        if(i % 2 == 0)
            h1.insert(i);
        else
            h.insert(i);
    h.merge(h1);
    h2 = h;

    for(i = 1; i < numItems; ++i)
    {
        int x;
        h2.pop(x);
        if(x != i)
            cout << "Oops! " << i << endl;
    }

    if(!h1.empty())
        cout << "Oops! h1 should have been empty!" << endl;

    cout << "End test... no other output is good" << endl;

    return 0;
}
