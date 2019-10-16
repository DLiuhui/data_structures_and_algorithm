#include <iostream>
#include <sstream>
#include <string>
#include "binary_heap.hpp"
using namespace std;
using DS::BinaryHeap;

// Test program
int main()
{
    int minItem = 10000;  // same number of digits
    int maxItem = 99999;
//    auto fun = [](const string& a, const string& b) -> bool{
//        return a > b;
//    }; // 用于大根堆
//    BinaryHeap<string, decltype(fun)> h(fun);
    BinaryHeap<string> h;
    string str = "hello";
    int i = 37;
    string x;

    cout << "Begin test... " << endl;

    for(i = 37; i != 0; i = (i + 37) % maxItem)
    {
        // should use to_string in C++11
        ostringstream sout;
        sout << "hello" << i;
        if(i >= minItem)
        {
            string s = sout.str();
            h.insert(s);
        }
    }

    for(i = minItem; i < maxItem; ++i)
    {
        ostringstream sout;
        sout << "hello" << i;

        h.pop(x);

        if(x != sout.str())
            cout << "Oops! " << i << endl;
    }

    cout << "End test... no other output is good" << endl;
    return 0;
}
