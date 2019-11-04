#include <iostream>
#include "kd_tree.hpp"

using namespace std;
using DS::KdTree;

int main()
{
    KdTree<int> t;

    cout << "Starting program" << endl;
    for (int i = 300; i < 370; ++i)
    {
        vector<int> it(2);
        it[0] = i;
        it[1] = 2500 - i;
        t.insert(it);
    }

    vector<int> low(2), high(2);
    low[0] = 70;
    low[1] = 2186;
    high[0] = 1200;
    high[1] = 2200;

    t.printRange(low, high);

    return 0;
}