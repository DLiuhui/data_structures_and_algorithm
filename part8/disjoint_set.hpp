#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP

// DisjSets class
//
// CONSTRUCTION: with int representing initial number of sets
//
// ******************PUBLIC OPERATIONS*********************
// void union( root1, root2 ) --> Merge two sets
// int find( x )              --> Return set containing x
// ******************ERRORS********************************
// No error checking is performed

#include <unordered_map>
#include <vector>
#include <utility>
#include "../lib/dsexceptions.h"

namespace DS
{
    template <typename Object>
    class DisjSets
    {
    public:
        // init disjoint sets
        // 一开始所有的元素都互相独立
        explicit DisjSets(const std::vector<Object>& elements)
        {
            for(auto& x : elements)
            {
                s_[x].flag_ = true;
                s_[x].value_.height_ = 0;
            }
        }

//        // Perform a find.
//        // Return the set containing x.
//        const Object& find(const Object& x) const
//        {
//            auto iter = s_.find(x);
//            if(iter == s_.end())
//                throw IllegalArgumentException{};
//            if(iter == nullptr)
//                return s_[x];
//            else
//                return find(s_[x]);
//        }

        // Perform a find with path compression.
        // Return the set containing x.
        // 使用路径压缩实时优化的find
        const Object& find(const Object& x)
        {
            // 判断存在性
            auto iter = s_.find(x); // &{x, s_[x]}
            if(iter == s_.end())
                throw IllegalArgumentException{};
            // 主要流程
            if(iter->second.flag_) // x为集合的根，返回x本身
                return x;
            else // 找到x所在集合的根，s[x]指向根
            {
                return iter->second.value_.root_ = find(iter->second.value_.root_);
            }
        }

        // Union two disjoint sets.
        // For simplicity, we assume root1 and root2
        // are distinct and represent set names.
        // root1 is the root of set 1.
        // root2 is the root of set 2.
        void unionSets(const Object& root1, const Object& root2)
        {
            auto iter1 = s_.find(root1);
            auto iter2 = s_.find(root2);
            if(iter1 == s_.end() || iter2 == s_.end())
                return;
            // 不是根
            if(false == iter1->second.flag_ || false == iter2->second.flag_)
                throw IllegalArgumentException{};
            // 主要流程
            // root2 is deeper, make root2 new root
            if(iter1->second.value_.height_ < iter2->second.value_.height_)
            {
                iter1->second.flag_ = false;
                iter1->second.value_.root_ = root2;
            } // root1 is deeper or equal height, make root1 new root
            else
            {
                // 如果高度相等高度，root1增高
                if(iter1->second.value_.height_ == iter2->second.value_.height_)
                    ++(iter1->second.value_.height_);
                iter2->second.flag_ = false;
                iter2->second.value_.root_ = root1;
            }
        }
    private:
        union Value
        {
            int height_;
            Object root_;
        };
        struct Element
        {
            bool flag_; // 标记是否是根
            Value value_;
        };
        std::unordered_map<Object, Element> s_;
    };
}

#endif //DISJOINT_SET_HPP
