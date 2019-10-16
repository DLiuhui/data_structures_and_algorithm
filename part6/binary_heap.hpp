#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

#include "../lib/dsexceptions.h"
#include <vector>

// 基本二叉堆
// BinaryHeap class
// CONSTRUCTION: with an optional capacity (that defaults to 100)
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x, allowing duplicates
// pop( minItem )   --> Remove (and optionally return) top item
// Comparable top( )  --> Return the top item
// bool empty( )        --> Return true if empty; else false
// void clear( )      --> Remove all items
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    // 默认的比较函数
    template <typename Object>
    class IsLess
    {
    public:
        bool operator() (const Object& lhs, const Object& rhs) const
        {
            return lhs < rhs;
        }
    };

    template <typename Object, class Compare = IsLess<Object>>
    class BinaryHeap
    {
    public:
        // array额外流程一个空间(array[0])作为插入删除中介
        explicit BinaryHeap(const Compare& cmp = Compare(), std::size_t capacity = 100)
        : array_(capacity + 1), current_size_{0}, compare_{cmp}
        {}

        explicit BinaryHeap(const std::vector<Object>& items, const Compare& cmp = Compare())
        : array_(items.size() + 10), current_size_{items.size()}, compare_{cmp}
        {
            for(int i = 0; i < items.size(); ++i)
                array_[i + 1] = items[i];
            buildHeap();
        }

        bool empty() const
        { return current_size_ == 0; }

        std::size_t size() const
        { return current_size_; }

        // 找堆顶元素，如果空堆则抛出异常
        const Object& top() const
        {
            if(empty())
                throw UnderflowException{};
            return array_[1];
        }

        // 删除堆顶元素
        void pop()
        {
            if(empty())
                throw UnderflowException{};

            array_[1] = std::move(array_[current_size_--]);
            // 下滤过程
            percolateDown(1);
        }

        // 删除堆顶元素，并将其放在min_item中
        void pop(Object& min_item)
        {
            if(empty())
                throw UnderflowException{};

            min_item = std::move(array_[1]);
            array_[1] = std::move(array_[current_size_--]);
            // 下滤过程
            percolateDown(1);
        }

        void insert(const Object& x)
        {
            if(current_size_ == array_.size() - 1)
                array_.resize(array_.size() * 2);

            Object copy(x);
            array_[0] = std::move(copy);

            // 上滤过程
            // current_size += 1
            // 将新的元素放在最后一位
            percolateUp(++current_size_);
        }

        void insert(Object&& x)
        {
            if(current_size_ == array_.size() - 1)
                array_.resize(array_.size() * 2);

            array_[0] = std::move(x);

            // 上滤过程
            // current_size += 1
            // 将新的元素放在最后一位
            percolateUp(++current_size_);
        }

        // 惰性擦除
        void clear()
        { current_size_ = 0; }

    private:
        std::vector<Object> array_;
        std::size_t current_size_;
        Compare compare_;

        void percolateUp(std::size_t pos)
        {
            for(; compare_(array_[0], array_[pos / 2]); pos /= 2)
                array_[pos] = std::move(array_[pos / 2]);
            array_[pos] = std::move(array_[0]);
        }

        void percolateDown(std::size_t pos)
        {
            std::size_t next_pos = 0;
            Object tmp = std::move(array_[pos]);
            for(; pos * 2 <= current_size_; pos = next_pos)
            {
                next_pos = pos * 2;
                if(next_pos < current_size_ && compare_(array_[next_pos + 1], array_[next_pos]))
                    ++next_pos;
                if(compare_(tmp, array_[next_pos]))
                    break;
                else
                    array_[pos] = std::move(array_[next_pos]);
            }
            array_[pos] = std::move(tmp);
        }

        void buildHeap()
        {
            for(int i = current_size_ / 2; i > 0; --i)
                percolateDown(i);
        }
    };
}
#endif
