// 书P395
// 素性判断
#include <iostream>
#include "random.hpp"

using namespace std;

typedef long long HugeNum;

// 实现基本的素性测试函数
// 如果witness不返回1，n肯定是一个合数
// 通过计算a^i(mod n)并随时查看1的非平凡的平方根得到
HugeNum witness(const HugeNum& a, const HugeNum& i, const HugeNum& n)
{
    if(i == 0)
        return 1;
    HugeNum x = witness(a, i / 2, n);
    if(x == 0) // 如果n递归为合数，停止
        return 0;

    // 如果我们找到1的一个非平凡平方根，则n不是素数
    HugeNum y = (x * x) % n;
    if(y == 1 && x != 1 && x != n - 1)
        return 0;
    if(i % 2 != 0)
        y = (a * y) % n;
    return y;
}

// 在随机化测试中所查询的witness次数
const int TRIALS = 5;

// 随机化素性测试
// 调整TRIALS以增加可信度
// n是要测试的数
// 如果返回值false，n肯定不是素数
// 返回true，n很可能是素数
bool isPrime(const HugeNum& n)
{
    DS::Random r;
    for(int counter = 0; counter < TRIALS; ++counter)
    {
        if(witness(r.randomInt(2, n - 2), n - 1, n) != 1)
            return false;
    }
    return true;
}

int main()
{
    for(int i = 101; i < 200; i += 2)
        if(isPrime(i))
            cout << i << " is prime" << endl;

    return 0;
}
