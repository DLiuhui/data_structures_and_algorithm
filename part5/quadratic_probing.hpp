/*
 * 二次再探型 hash table
 * key出现冲突时采用二次再探法寻找下一个key
 * 出现冲突按照链表形式在key后增加元素
 */

#ifndef QUADRATIC_PROBING_HPP
#define QUADRATIC_PROBING_HPP

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

    // QuadraticProbing Hash table class
    // CONSTRUCTION: an approximate initial size or default of 101
    //
    // ******************PUBLIC OPERATIONS*********************
    // bool insert( x )       --> Insert x
    // bool remove( x )       --> Remove x
    // bool contains( x )     --> Return true if x is present
    // void makeEmpty( )      --> Remove all items
    // int hashCode( string str ) --> Global method to hash strings
    template <typename Object>
    class HashTable
    {
    public:
        explicit HashTable(int size = 101)
        : array_(nextPrime(size))
        { makeEmpty(); }

        bool contains(const Object& x) const
        {
            return isActive(findPos(x));
        }

        void makeEmpty()
        {
            current_size_ = 0;
            for(auto& entry : array_)  // 惰性删除
                entry.info_ = EMPTY;
        }

        bool insert(const Object& x)
        {
            int pos = findPos(x);
            if(isActive(pos))
                return false;
            if(array_[pos].info_ != DELETED)
                ++current_size_;
            array_[pos].element_ = x;
            array_[pos].info_ = ACTIVE;

            if(current_size_ > array_.size() / 2)
                rehash();
            return true;
        }

        bool insert(Object&& x)
        {
            int pos = findPos(x);
            if(isActive(pos))
                return false;
            if(array_[pos].info_ != DELETED)
                ++current_size_;
            array_[pos].element_ = std::move(x);
            array_[pos].info_ = ACTIVE;

            if(current_size_ > array_.size() / 2)
                rehash();
            return true;
        }

        bool remove(const Object& x)
        {
            int pos = findPos(x);
            if(!isActive(pos))
                return false;
            array_[pos].info_ = DELETED;
            --current_size_;
            return true;
        }

        enum EntryType{ACTIVE, EMPTY, DELETED};

    private:
        struct HashEntry
        {
            Object element_;
            EntryType info_;  // 惰性删除使用，标记元素是否被删除

            explicit HashEntry(const Object& e = Object{}, EntryType i = EMPTY)
            : element_{e}, info_{i}
            {}

            explicit HashEntry(Object&& e, EntryType i = EMPTY)
            : element_{std::move(e)}, info_{i}
            {}
        };

        std::vector<HashEntry> array_;
        std::size_t current_size_;

        bool isActive(int pos) const
        { return array_[pos].info_ == ACTIVE; }

        // 关键：二次再探法中的pos探查
        std::size_t findPos(const Object& x) const
        {
            std::size_t offset = 1;
            std::size_t pos = myHash(x);
            // 出现EMPTY元素后作为搜索终点
            while(array_[pos].info_ != EMPTY && array_[pos].element_ != x)
            {
                // 二次数列
                // offset 1 3 5 7 9...
                // pos+ 1 4 9 16 25...
                pos += offset;
                offset += 2;
                if(pos >= array_.size())
                    pos -= array_.size();
            }
            return pos;
        }

        std::size_t myHash(const Object& x) const
        {
            // 定义hash函数
            static std::hash<Object> hash_function;
            return hash_function(x) % array_.size();
        }

        // 再hash扩张
        void rehash()
        {
            std::vector<HashEntry> old_table = array_;

            // 创建新的hash表，大致为原表大小两倍
            array_.resize(nextPrime(2 * old_table.size()));
            for(auto& entry : array_)
                entry.info_ = EMPTY;

            // 复制原表处于激活状态的元素
            current_size_ = 0;
            for(auto& entry : old_table)
            {
                if(entry.info_ == ACTIVE)
                    insert(std::move(entry.element_));
            }
        }
    };
}
#endif //SEPARATE_CHAINING_HPP
