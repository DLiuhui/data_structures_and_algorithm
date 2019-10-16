#include "my_random.h"

namespace DS
{
    static const int A = 48271;
    static const int M = 2147483647;
    static const int Q = M / A;
    static const int R = M % A;

    // Construct with initialValue for the state.
    Random::Random(int init_val)
    {
        if(init_val < 0)
            init_val += M;
        state = init_val;
        if(state == 0)
            state = 1;
    }

    // Return a pseudorandom int, and change the internal state
    int Random::randomInt()
    {
        int tmp_state = A * (state % Q) - R * (state / Q);
        if(tmp_state >= 0)
            state = tmp_state;
        else
            state = tmp_state + M;
        return state;
    }

    // Return an int in the closed range [low,high], and
    // change the internal state. This is a poor implementation and
    // will be biased toward some numbers, especially if the range is large.
    int Random::randomInt(int low, int high)
    {
        double partition_size = static_cast<double>(M) / (high - low + 1);
        return static_cast<int>(randomInt() / partition_size) + low;
    }

    // Return a pseudorandom double in the open range 0..1
    // and change the internal state.
    double Random::random()
    {
        return static_cast<double>(randomInt()) / M;
    }

    // Return a pseudorandom int, and change the
    // internal state. DOES NOT WORK.
    int Random::randomIntWRONG()
    {
        return state = (A * state) % M;
    }
}


