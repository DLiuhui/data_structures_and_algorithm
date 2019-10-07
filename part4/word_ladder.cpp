//
// Created by DDRHb on 2019/10/7.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

void readWords(ifstream& in, vector<string>& container)
{
    string line;
    while(in >> line)
        container.push_back(line);
}

// Computes a map in which the keys are words and values are vectors of words
// that differ in only one character from the corresponding key.
// Uses an efficient algorithm that is O(N log N) with a map, or
// O(N) is a hash_map is used.
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

int main()
{
    clock_t start, end;
    ifstream file("../../dict.txt");
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
}