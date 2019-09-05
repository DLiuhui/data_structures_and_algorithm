//
// Created by DDRHb on 2019/9/5.
//
#include "binary_search_tree.hpp"
#include <iostream>
#include <vector>
#include <random>
using namespace std;
using DS::BinarySearchTree;

int main()
{
    vector<int> vec;
    BinarySearchTree<int> t;
    int NUMS = 40000;
    const int GAP = 1000;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for( i = GAP; i != 0; i = ( i + GAP ) % NUMS )
        vec.push_back(i);

    random_shuffle(vec.begin(), vec.end());

    for(auto x : vec)
    {
        cout << x << endl;
        t.insert(x);
    }
    cout << "===========" << endl;

    t.printTree(cout);
    cout << "===========" << endl;

    for( i = 0; i < NUMS; i += 3000 )
        t.remove( i );
    t.printTree(cout);
    cout << "===========" << endl;

    t.printTree(cout, true);
    cout << "===========" << endl;

    cout << t.findMax() << endl;
    cout << t.findMin() << endl;

    for(i = 0; i < NUMS; i += 4000)
    {
        if (t.contains(i))
            cout << "Find " << i << endl;
    }
//
    for(i = 0; i < NUMS; i+=5000)
    {
        if(!t.contains(i))
            cout << "Not find " << i << endl;
    }
//
//    BinarySearchTree<int> t2;
//    t2 = t;
//
//    for( i = 2; i < NUMS; i+=2 )
//        if( !t2.contains( i ) )
//            cout << "Find error1!" << endl;
//
//    for( i = 1; i < NUMS; i+=2 )
//    {
//        if( t2.contains( i ) )
//            cout << "Find error2!" << endl;
//    }

    cout << "Finished testing" << endl;

    return 0;
}

