/*
 * 二次再探型 hash table
 * key出现冲突时采用二次再探法寻找下一个key
 * 出现冲突按照链表形式在key后增加元素
 */

#ifndef CUCKOO_HASH_HPP
#define CUCKOO_HASH_HPP

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "../lib/uniform_random.h"

#define MAX_LOAD 0.40

namespace DS
{
    // 判断素数
    bool isPrime(int n)
    {
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

    // hash function for string
    int hashCode(const std::string& key)
    {
        int hashVal = 0;
        for(auto ch : key)
            hashVal = 37 * hashVal + ch;
        return hashVal;
    }

    int hashCode(int key)
    { return key; }

    template <typename AnyType>
    class CuckooHashFamily
    {
    public:
        virtual std::size_t hash(const AnyType & x, int which) const = 0;
        virtual int getNumberOfFunctions() = 0;
        virtual void generateNewFunctions() = 0;
    };

    template <int count>
    class StringHashFamily : public CuckooHashFamily<std::string>
    {
    public:
        StringHashFamily()
        : MULTIPLIERS_(count)
        {
            generateNewFunctions();
        }

        int getNumberOfFunctions() override
        { return count; }

        void generateNewFunctions() override
        {
            for(auto& mult : MULTIPLIERS_)
                mult = r_.nextInt();
        }

        // 根据which, 使用不同的hash函数
        std::size_t hash(const std::string& x, int which) const override
        {
            const int multiplier = MULTIPLIERS_[which];
            size_t hash_val = 0;

            for(auto ch : x)
                hash_val = multiplier * hash_val + ch;
            return hash_val;
        }
    private:
        std::vector<int> MULTIPLIERS_;
        UniformRandom r_;
    };

    // CuckooHashing Hash table class
    //
    // CONSTRUCTION: an approximate initial size or default of 101
    //
    // ******************PUBLIC OPERATIONS*********************
    // bool insert( x )       --> Insert x
    // bool remove( x )       --> Remove x
    // bool contains( x )     --> Return true if x is present
    // void makeEmpty( )      --> Remove all items
    // int hashCode( string str ) --> Global method to hash strings

    template <typename Object, typename HashFamily>
    class HashTable
    {
    public:
        explicit HashTable(int size = 101)
        : array_(nextPrime(size))
        {
            n_hash_functions_ = hash_functions_.getNumberOfFunctions();
            rehashes_ = 0;
            makeEmpty();
        }

        bool contains(const Object& x) const
        {
            return findPos(x) != -1;
        }

        void makeEmpty()
        {
            current_size_ = 0;
            for(auto& entry : array_)  // 惰性删除
                entry.is_active_ = false;
        }

        bool insert(const Object& x)
        {
            if(contains(x))
                return false;
            // 超出了最大容量
            if(current_size_ >= array_.size() * MAX_LOAD)
                expand();
            return insertHelper1(x);
        }

        bool insert(Object&& x)
        {
            if(contains(x))
                return false;
            // 超出了最大容量
            if(current_size_ >= array_.size() * MAX_LOAD)
                expand();
            return insertHelper1(std::move(x));
        }

        int size() const
        { return current_size_; }

        int capacity() const
        { return array_.size(); }

        bool remove(const Object& x)
        {
            int pos = findPos(x);
            if(!isActive(pos))
                return false;
            array_[pos].is_active_ = false;
            --current_size_;
            return true;
        }

    private:
        struct HashEntry
        {
            Object element_;
            bool is_active_;

            explicit HashEntry(const Object& e = Object(), bool a = false)
            : element_{e}, is_active_{a}
            {}

            explicit HashEntry(Object&& e, bool a = false)
            : element_{std::move(e)}, is_active_{a}
            {}
        };

        std::vector<HashEntry> array_;
        int current_size_;
        int n_hash_functions_;
        int rehashes_;
        UniformRandom r_;
        HashFamily hash_functions_;

        static const int ALLOWED_REHASHES = 5;

        bool insertHelper1(const Object& xx)
        {
            const int COUNT_LIMIT = 100;
            Object x = xx;
            while(true)
            {
                int last_pos = -1;
                int pos;

                for(int count = 0; count < COUNT_LIMIT; ++count)
                {
                    for(int i = 0; i < n_hash_functions_; ++i)
                    {
                        pos = myHash(x, i);

                        if(!isActive(pos))
                        {
                            array_[pos] = std::move(HashEntry{std::move(x), true});
                            ++current_size_;
                            return true;
                        }
                    }

                    // None of the spots are available. Kick out random one
                    int i = 0;
                    do
                    {
                        pos = myHash(x, r_.nextInt(n_hash_functions_));
                    }while(pos == last_pos && i++ < 5);

                    last_pos = pos;
                    std::swap(x, array_[pos].element_);
                }

                if(++rehashes_ > ALLOWED_REHASHES)
                {
                    expand(); // Make the table bigger
                    rehashes_ = 0;
                }
                else
                    rehash();
            }
        }

        bool insertHelper1(Object&& x)
        {
            const int COUNT_LIMIT = 100;

            while(true)
            {
                int last_pos = -1;
                int pos;

                for(int count = 0; count < COUNT_LIMIT; ++count)
                {
                    for(int i = 0; i < n_hash_functions_; ++i)
                    {
                        pos = myHash(x, i);

                        if(!isActive(pos))
                        {
                            array_[pos] = std::move(HashEntry{std::move(x), true});
                            ++current_size_;
                            return true;
                        }
                    }

                    // None of the spots are available. Kick out random one
                    int i = 0;
                    do
                    {
                        pos = myHash(x, r_.nextInt(n_hash_functions_));
                    }while(pos == last_pos && i++ < 5);

                    last_pos = pos;
                    std::swap(x, array_[ pos ].element_);
                }

                if(++rehashes_ > ALLOWED_REHASHES)
                {
                    expand(); // Make the table bigger
                    rehashes_ = 0;
                }
                else
                    rehash();
            }
        }

        bool isActive(int pos) const
        { return pos != -1 && array_[pos].is_active_; }

        // 寻找pos
        int findPos(const Object& x) const
        {
            for(int i = 0; i < n_hash_functions_; ++i)
            {
                int pos = myHash(x, i); // 使用第i个hash function
                if(isActive(pos) && array_[pos].element_ == x)
                    return pos;
            }
            return -1;
        }

        // which 用于指定使用HashFamily里的哪一个hash function
        int myHash(const Object& x, int which) const
        {
            return hash_functions_.hash(x, which) % array_.size();
        }

        void expand()
        { rehash(static_cast<int>(array_.size() / MAX_LOAD)); }

        void rehash()
        {
            hash_functions_.generateNewFunctions();
            rehash(array_.size());
        }

        // 再hash扩张
        void rehash(int size)
        {
            std::vector<HashEntry> old_table = array_;

            // 创建新的hash表，大致为原表大小两倍
            array_.resize(nextPrime(size));
            for(auto& entry : array_)
                entry.is_active_ = false;

            current_size_ = 0;
            for(auto& entry : old_table)
            {
                if(entry.is_active_)
                    insert(std::move(entry.element_));
            }
        }
    };
}
#endif //CUCKOO_HASH_HPP
