#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "../lib/uniform_random.h"

using namespace std;
using namespace DS;

// 基数排序实现
// 假设字符串中只出现ASCII码
// 如果字符串长度不同，补'\0'填充
void radixSortA(vector<string>& arr, int max_string_len)
{
    const int BUCKETS = 256;
    vector<vector<string>> buckets(BUCKETS); // 桶
    for(int i = 1; i <= max_string_len; ++i)
    {
        // 从低位到高位，依次进行桶排序迭代
        // 对于string而言就是从字符串尾到头
        // 长度不够补0('\0')
        for(string& s : arr)
        {
            // 计算插入的桶序号
            int idx = static_cast<int>((s.size() >= i) ? s[s.size() - i] : '\0');
            buckets[idx].push_back(std::move(s));
        }

        int pos = 0;
        for(auto& bucket : buckets)
        {
            for(string& s : bucket)
                arr[pos++] = std::move(s);
            bucket.clear();
        }
    }
}

// 对数组进行计数基数排序
// 假设字符串中只出现ASCII码
// 假设所有字符串一样长
// 详解见书P266-267
void countingRadixSort(vector<string>& arr, int max_string_len)
{
    const int BUCKETS = 256;
    int n_total = arr.size();
    vector<string> buffer(n_total);

    vector<string>* in = &arr;
    vector<string>* out = &buffer;

    for(int i = 1; i <= max_string_len; ++i)
    {
        vector<int> count(BUCKETS + 1);
        int idx = 0;
        for(string& s : (*in))
        {
            idx = static_cast<int>((s.size() >= i) ? s[s.size() - i] : '\0');
            ++count[idx + 1];
        }
        for(int b = 1; b < BUCKETS; ++b)
            count[b] += count[b - 1];
        for(string& s : (*in))
        {
            idx = static_cast<int>((s.size() >= i) ? s[s.size() - i] : '\0');
            (*out)[count[idx]++] = std::move(s);
        }
        std::swap(in, out);
    }
    // if odd number of passes, in is buffer, out is arr; so copy back
    if(max_string_len % 2 == 1)
    {
        for(int i = 0; i < arr.size(); ++i)
            (*out)[i] = std::move((*in)[i]);
    }
}

// 标准基数排序
// 假设字符串中只有ASCII
// 边长字符串
void radixSort(vector<string>& arr, int max_len)
{
    const int BUCKETS = 256;
    vector<vector<string>> word_by_length(max_len + 1);
    vector<vector<string>> buckets(BUCKETS);
    // 按照字符串长度分类
    for(string& s : arr)
    {
        word_by_length[s.length()].push_back(std::move(s));
    }
    // 将按照长度分类后的字符串放回arr(短在前，长在后)
    int idx = 0;
    for(auto& word_list : word_by_length)
    {
        for(string& s : word_list)
            arr[idx++] = std::move(s);
    }

    int starting_idx = arr.size();
    // 从字符串尾到头
    for(int pos = max_len - 1; pos >= 0; --pos)
    {
        starting_idx -= word_by_length[pos + 1].size();
        // 对starting_idx后的所有元素排序
        // 更长的字符串被包含在了含有短字符串的新的集合内
        for(int i = starting_idx; i < arr.size(); ++i)
            buckets[arr[i][pos]].push_back(std::move(arr[i]));
        idx = starting_idx;
        // 出桶
        for(auto& bucket : buckets)
        {
            for(string& s : bucket)
                arr[idx++] = std::move(s);
            bucket.clear();
        }
    }
}

int main()
{
    vector<string> lst;
    UniformRandom r;
    const int N = 100000;
    const int LEN = 5;
    const int ADD = 7;

    for (int i = 0; i < N; ++i)
    {
        string str = "";
        int len = LEN + ADD; //r.nextInt( ADD ); // between 5 and 11 characters

        for (int j = 0; j < len; ++j)
            str += static_cast<char>( 'a' + r.nextInt(26));

        lst.push_back(str);
    }

    vector<string> arr1 = lst;
    vector<string> arr2 = lst;
    vector<string> arr3 = lst;
    vector<string> arr4 = lst;

    sort(begin(arr1), end(arr1));
    radixSortA(arr2, LEN + ADD);
    countingRadixSort(arr3, LEN + ADD);
    radixSort(arr4, LEN + ADD);

    cout << "radixSortA" << endl;
    for (int i = 0; i < arr1.size(); ++i)
        if (arr1[i] != arr2[i])
            cout << "OOPS!! " << i << endl;
    cout << "=====" << endl;

    cout << "countingRadixSort" << endl;
    for (int i = 0; i < arr1.size(); ++i)
        if (arr1[i] != arr3[i])
            cout << "OOPS!! " << i << endl;
    cout << "=====" << endl;

    cout << "radixSort" << endl;
    for (int i = 0; i < arr1.size(); ++i)
        if (arr1[i] != arr4[i])
            cout << "OOPS!! " << i << endl;
    cout << "=====" << endl;

    return 0;
}