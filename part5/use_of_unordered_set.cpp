#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// unordered_set 使用，提供定制的equal(==)和hash函数
// 实现一个大小写不敏感的字符串的无序集合

string toLower(const string& s)
{
    string copy = s; // 避免对s的修改
    transform(copy.begin(), copy.end(), copy.begin(),
            [](unsigned char c) -> unsigned char {return tolower(c);});
    return copy;
}

bool equalsIgnoreCase(const string& lhs, const string& rhs)
{
    return toLower(lhs) == toLower(rhs);
}
class CaseInsensitive
{
public:
    size_t operator() (const string& s) const  // 重载的hash函数
    {
        static hash<string> hf;
        return hf(toLower(s));
    }

    bool operator() (const string& lhs, const string& rhs) const // 重载的比较函数
    {
        return equalsIgnoreCase(lhs, rhs);
    }
};

int main()
{
    unordered_set<string, CaseInsensitive, CaseInsensitive> s;

    s.insert("HELLO");
    s.insert("helLo");
    s.insert("WORLD");
    s.insert("world");
    s.insert("hello");

    auto itr = s.begin();

    while(itr != s.end())
        cout << *itr++ << endl;
    return 0;
}
