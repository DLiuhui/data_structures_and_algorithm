/*
 * 分离链接型hash table
 * 采用链表再key处扩增元素
 * 出现冲突按照链表形式在key后增加元素
 */

#ifndef SEPARATE_CHAINING_HPP
#define SEPARATE_CHAINING_HPP

#include <vector>
#include <list>
#include <string>
#include <functional>
#include <iostream>

namespace DS
{
    // 判断素数
    bool isPrime(int n) {
        if (n < 2 || (n & 1) == 0)
            return false;
        if (n == 2 || n == 3)
            return true;
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    int nextPrime(int n)
    {
        if(n < 1)
            return 2;
        if((n & 1) == 0)
            ++n;
        for(; !isPrime(n); n += 2)
            ;
        return n;
    }

    // A hash function for string objects
    size_t hash(const std::string& key)
    {
        size_t hash_val = 0;
        for(char ch : key)
            hash_val = 37 * hash_val + int(ch);
        return hash_val;
    }

    // A hash function for ints
    size_t hash(int key)
    {
        return key;
    }

    // SeparateChaining Hash table class
    //
    // CONSTRUCTION: an approximate initial size or default of 101
    //
    // ******************PUBLIC OPERATIONS*********************
    // bool insert( x )       --> Insert x
    // bool remove( x )       --> Remove x
    // bool contains( x )     --> Return true if x is present
    // void makeEmpty( )      --> Remove all items
    template <typename Object>
    class HashTable
    {
    public:
        explicit HashTable(int size = 101)
            : current_size_{0}
        { hash_table_.resize(size); }

        bool contains(const Object& x) const
        {
            // 调用hash函数返回第一级关键字对应的list
            const std::list<Object>& which_list = hash_table_[myHash(x)];
            // 在链表中寻找
            return which_list.end() != myFind(which_list.begin(), which_list.end(), x);
        }

        void makeEmpty()
        {
            for(std::list<Object>& the_list : hash_table_)
            {
                the_list.clear();
            }
        }

        bool insert(const Object& x)
        {
            std::list<Object>& which_list = hash_table_[myHash(x)];
            if(myFind(which_list.begin(), which_list.end(), x) != which_list.end())
                return false;
            which_list.push_back(x);

            if(++current_size_ > hash_table_.size())
                rehash();
            return true;
        }

        bool insert(Object&& x)
        {
            std::list<Object>& which_list = hash_table_[myHash(x)];
            if(myFind(which_list.begin(), which_list.end(), x) != which_list.end())
                return false;
            which_list.push_back(std::move(x));

            if(++current_size_ > hash_table_.size())
                rehash();
            return true;
        }

        bool remove(const Object& x)
        {
            std::list<Object>& which_list = hash_table_[myHash(x)];
            typename std::list<Object>::iterator iter = myFind(which_list.begin(), which_list.end(), x);
            if(iter == which_list.end())
                return false;
            which_list.erase(iter);
            --current_size_;
            return true;
        }

    private:
        std::vector<std::list<Object>> hash_table_;
        int current_size_ ;

        size_t myHash(const Object& x) const
        {
            // 定义hash函数
            static std::hash<Object> hash_function;
            return hash_function(x) % hash_table_.size();
        }

        typename std::list<Object>::iterator myFind(
                typename std::list<Object>::iterator begin,
                typename std::list<Object>::iterator end,
                const Object& x) const
        {
            typename std::list<Object>::iterator iter = begin;
            while(iter != end)
            {
                if(*iter == x)
                    return iter;
                ++iter;
            }
            return end;
        }

        typename std::list<Object>::const_iterator myFind(
                typename std::list<Object>::const_iterator begin,
                typename std::list<Object>::const_iterator end,
                const Object& x) const
        {
            typename std::list<Object>::const_iterator iter = begin;
            while(iter != end)
            {
                if(*iter == x)
                    return iter;
                ++iter;
            }
            return end;
        }

        // 再hash扩张
        void rehash()
        {
            std::vector<std::list<Object>> old_table = hash_table_;

            // 创建新的hash表，大致为原表大小两倍
            hash_table_.resize(nextPrime(2 * hash_table_.size()));
            for(auto& the_list : hash_table_)
                the_list.clear();

            // 复制原表
            current_size_ = 0;
            for(std::list<Object>& the_list : old_table)
            {
                for(Object& x : the_list)
                    insert(std::move(x));
            }
        }
    };
}
#endif //SEPARATE_CHAINING_HPP
