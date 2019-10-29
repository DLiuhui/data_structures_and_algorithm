#ifndef RANDOM_HPP
#define RANDOM_HPP

// 随机数生成器
// Random class
// This code assumes 32-bit ints,
// which are standard on modern compilers.
//
// CONSTRUCTION: with (a) no initializer or (b) an integer
//     that specifies the initial state of the generator
//
// ******************PUBLIC OPERATIONS*********************
//     Return a random number according to some distribution:
// int randomInt( )                     --> Uniform, 1 to 2^31-1
// int random0_1( )                     --> Uniform, 0 to 1
// int randomInt( int low, int high )   --> Uniform low..high

namespace DS
{
    class Random
    {
    public:
        static const int A = 48271;
        static const int M = 2147483647;
        static const int Q = M / A;
        static const int R = M % A;

        explicit Random(int init = 1)
        {
            if(init < 0)
                init += M;
            state_ = init;
            if(state_ == 0)
                state_ = 1;
        }

        int randomInt()
        {
            int tmp = A * (state_ % Q) - R * (state_ / Q);
            if(tmp >= 0)
                state_ = tmp;
            else
                state_ = tmp + M;
            return state_;
        }

        // 可能溢出
        int randomIntWrong()
        {
            return state_ = (A * state_) % M;
        }

        double random0_1()
        {
            return static_cast<double>(randomInt()) / M;
        }

        int randomInt(int low, int high)
        {
            double partition_size = static_cast<double>(M) / (high - low + 1);
            return static_cast<int>(randomInt() / partition_size) + low;
        }

    private:
        int state_;
    };
}
#endif //RANDOM_HPP
