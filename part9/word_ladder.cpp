//
// Created by DDRHb on 2019/10/7.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <queue>
#include <algorithm>

using namespace std;

void readWords(ifstream& in, vector<string>& container)
{
    string line;
    while(in >> line)
        container.push_back(line);
}

void computeAdjacentWords(const vector<string>& words, map<string, vector<string>>& adj_words)
{
    map<int, vector<string>> words_by_length;

    // 根据单词长度划分分组
    for(auto & word : words)
        words_by_length[word.size()].push_back(word);

    // 对于每个分组
    for(auto & entry : words_by_length)
    {
        const vector<string> & group_words{entry.second};
        int word_length = entry.first;

        // 对单词的每一个位置
        for(int i = 0; i < word_length; ++i)
        {
            // Remove one character in specified position, computing representative.
            // Words with same representatives are adjacent
            map<string, vector<string>> rep_to_word;
            for(auto & word : group_words)
            {
                string rep = word;
                rep.erase(i, 1);
                rep_to_word[rep].push_back(word);
            }

            // and then look for map values with more than one string
            for(auto & word : rep_to_word)
            {
                const vector<string> & clique = word.second;
                if(clique.size() > 1)
                {
                    // 单词两两之间配对，加入adj_words
                    for(size_t p = 0; p < clique.size(); ++p)
                    {
                        for(size_t q = p + 1; q < clique.size(); ++q)
                        {
                            adj_words[clique[p]].push_back(clique[q]);
                            adj_words[clique[q]].push_back(clique[p]);
                        }
                    }
                }
            }
        }
    }
}

void printHighChangeables(const map<string, vector<string>>& adj_word, int min_words = 15)
{
    if(min_words <= 0)
        return;
    for(auto & entry : adj_word)
    {
        const vector<string>& words = entry.second;
        if(words.size() >= min_words)
        {
            cout << entry.first << " (" << words.size() << "):";
            for(auto & str : words)
                cout << " " << str;
            cout << endl;
        }
    }
}

vector<string> getChainFromPreviousMap(const map<string,string>& previous,
        const string& first, const string& second)
{
    vector<string> result;
    auto& prev = const_cast<map<string, string>&>(previous);
    for(string current = second; !current.empty(); current = prev[current])
        result.push_back(current);
    reverse(result.begin(), result.end());
    return result;
}

// Runs the shortest path calculation from the adjacency map, returning a vector
// that contains the sequence of word changes to get from first to second.
// 利用广度优先BFS找单词first到second的最快替换路线
vector<string> findChain(const map<string, vector<string>>& adjacent_words,
        const string& first, const string& second)
{
    map<string, string> previous_word; // 记录已经访问的节点
    queue<string> q;
    previous_word[first] = first;
    q.push(first);
    // BFS 广度优先搜索
    while(!q.empty() && q.front() != second)
    {
        string current = q.front();
        q.pop();
        auto iter = adjacent_words.find(current);
        if(iter != adjacent_words.end())
        {
            const vector<string>& adj = iter->second;
            for(auto& str : adj)
            {
                if(previous_word.find(str) == previous_word.end())
                {
                    previous_word[str] = current;
                    q.push(str);
                }
            }
        }
    }
    previous_word[first] = "";
    if(previous_word.find(second) != previous_word.end())
        return getChainFromPreviousMap(previous_word, first, second);
    else
        return vector<string>{};
}

int main()
{
    clock_t start, end;
    ifstream file("../dict.txt");
    if (!file.is_open())
    {
        cout << "Error opening file";
        exit (1);
    }
    vector<string> words(32, string{});
    // read words from dict.txt
    readWords(file, words);

    map<string, vector<string>> adjacent_words;

    start = clock();
    computeAdjacentWords(words, adjacent_words);
    end = clock();
    cout << "Elapsed time FAST: " << double(end - start) / CLOCKS_PER_SEC << endl;

    printHighChangeables(adjacent_words, 15);

    while(true)
    {
        cout << "Enter two words:";
        string w1, w2;
        cin >> w1 >> w2;
        vector<string> path = findChain(adjacent_words, w1, w2);
        cout << path.size() << endl;
        for(string& word : path)
            cout << word << " ";
        cout << endl;
    }
    return 0;
}