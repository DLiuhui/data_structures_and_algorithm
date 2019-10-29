#include "random.hpp"
#include <iostream>
using namespace std;
using DS::Random;

// Test program
int main()
{
    Random r{1};

    for (int i = 0; i < 20; ++i)
        cout << r.randomInt() << endl;

    return 0;
}