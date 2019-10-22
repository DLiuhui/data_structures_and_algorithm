#include "binomial_queue.hpp"
#include <iostream>
using namespace std;
using DS::BinomialQueue;

int main()
{
    int numItems = 100000;
    BinomialQueue<int> h;
    BinomialQueue<int> h1;
    BinomialQueue<int> h2;
    int i = 37;

    cout << "Begin test..." << endl;
    for(i = 37; i != 0; i = (i + 37) % numItems)
    {
        if (i % 2 == 0)
        {
            h1.insert(i);
        } else
        {
            h.insert(i);
        }
    }

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

    cout << "End of test... no output is good" << endl;

    return 0;
}
