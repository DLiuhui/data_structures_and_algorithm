#ifndef _MY_RANDOM_H
#define _MY_RANDOM_H

#include <iostream>
// Random class
// This code assumes 32-bit ints
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the initial state of the generator
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution:
// int randomInt( )                     --> Uniform, 1 to 2^31-1
// int random( )                     --> Uniform, 0 to 1
// int randomInt( int low, int high )   --> Uniform low..high

namespace DS
{
    class Random
    {
    public:
        explicit Random(int init_val = 1);
        int randomInt(); // Uniform, 1 to 2^31-1
        int randomInt(int low, int high); // Uniform [low..high]
        double random(); // Uniform, [0, 1]
        int randomIntWRONG();
    private:
        int state;
    };
}
#endif //_MY_RANDOM_H
