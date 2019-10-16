#include <iostream>
#include <string>
#include "cuckoo_hash.hpp"
using namespace std;
using namespace DS;

// Simple main
int main()
{
    const int NUMS = 200000;
    const int GAP  =   37;
    const int ATTEMPTS = 1;
    int i;

    cout << "Checking... (no more output means success)" << endl;

    for(int att = 0; att < ATTEMPTS; ++att)
    {
        cout << "ATTEMPT: " << att << endl;

        HashTable<string, StringHashFamily<3>> h1;
        HashTable<string, StringHashFamily<3>> h2;

        for(i = GAP; i != 0; i = (i + GAP) % NUMS)
        {
            if(!h1.insert(to_string(i)))
            {
                cout << "OOPS insert fails?! " << i << endl;
            }
        }

        for(i = GAP; i != 0; i = (i + GAP) % NUMS)
            if(h1.insert(to_string(i)))
                cout << "INSERT OOPS!!! " << i << endl;

        h2 = h1;

        for(i = 1; i < NUMS; i += 2)
            h2.remove(to_string(i));

        for(i = 2; i < NUMS; i += 2)
            if(!h2.contains(to_string(i)))
                cout << "Contains fails " << i << endl;

        for(i = 1; i < NUMS; i += 2)
        {
            if(h2.contains(to_string(i)))
                cout << "CONTAINS OOPS!!! " << i << endl;
        }

        cout << "END OF ATTEMPT" << endl;

        if(h2.capacity() > NUMS * 4)
            cout << "LARGE CAPACITY " << h2.capacity() << endl;
    }
    return 0;
}
