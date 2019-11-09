#include <iostream>
#include "rb_tree.hpp"
using namespace std;
using DS::RedBlackTree;

// Test program
int main()
{
    RedBlackTree<int> t;
    int NUMS = 400000;
//    int NUMS = 1000;
    const int GAP = 37;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    cout << "Build check" << endl;
    for (i = GAP; i != 0; i = (i + GAP) % NUMS)
    {
        t.insert(i);
    }
    if (NUMS < 1000)
        t.print();
    if (t.findMin() != 1)
        cout << "FindMin error!" << endl;
    if(t.findMax() != NUMS - 1)
        cout << "FindMax error!" << endl;

    for (i = 1; i < NUMS; ++i)
        if (!t.contains(i))
            cout << "Find error1!" << endl;
    if (t.contains(0))
        cout << "Oops!" << endl;

    cout << "============" << endl;
    cout << "Clone check" << endl;
    RedBlackTree<int> t2;
    t2 = t;

    for (i = 1; i < NUMS; ++i)
        if (!t2.contains(i))
            cout << "Find error1!" << endl;
    if (t2.contains(0))
        cout << "Oops!" << endl;

    cout << "============" << endl;
    cout << "Remove check" << endl;
    for(i = 1; i < NUMS; i += 2)
        t.remove(i);
    for(i = 1; i < NUMS; i += 2)
        if(t.contains(i))
            cout << "Find error2!" << endl;
    for(i = 2; i < NUMS; i += 2)
        if(!t.contains(i))
            cout << "Find error3!" << endl;
    if (NUMS < 1000)
        t.print();

    cout << "Test complete..." << endl;
    return 0;
}