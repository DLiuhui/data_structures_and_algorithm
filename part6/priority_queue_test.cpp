#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

//
template <typename PriorityQueue>
void dumpContents(const string & msg, PriorityQueue & pq)
{
    cout << msg << ":" << endl;
    while(!pq.empty())
    {
        cout << pq.top() << endl;
        pq.pop();
    }
}

int main( )
{
    priority_queue<int> maxPQ; // 小根堆
    priority_queue<int, vector<int>, greater<int>> minPQ; // 大根堆

    vector<int> nums = {1,3,5,2,12,9,16,11,8,10};

    for(auto x : nums)
    {
        minPQ.push(x);
        maxPQ.push(x);
    }

    dumpContents("minPQ", minPQ);
    dumpContents("maxPQ", maxPQ);

    return 0;
}