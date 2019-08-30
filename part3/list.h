//
// Created by DDRHb on 2019/8/30.
//

#ifndef __LIST_H__
#define __LIST_H__

#include <iostream>

// 数据结构与算法分析 第四版 第三章 list
// 双向链表 包含头指针 尾指针

template <typename Object>
class List
{
public:
    // 常量迭代器
    class const_iterator
    {

    };
    // 迭代器
    class iterator: public const_iterator
    {

    };

    List():
        the_size_{0}, head_{new Node}, tail_{new Node}
    {
        head_->next_ = tail_;
        tail_->prev_ = head_;
    }

    ~List()
    {
        clear();
        delete head_;
        delete tail_;
    }

    List(const List& rhs):
        the_size_{0}, head_{new Node}, tail_{new Node}
    {
        for(auto& x : rhs)
            push_back(x);
    }

    List& operator= (const List& rhs)
    {
        List copy(rhs);
        std::swap(*this, copy);
        return *this;
    }

    List(List&& rhs) noexcept:
        the_size_{rhs.the_size_}, head_{rhs.head_}, tail_{rhs.tail_}
    {
        rhs.the_size_ = 0;
        rhs.head_ = nullptr;
        rhs.tail_ = nullptr;
    }

    List& operator= (List&& rhs) noexcept
    {
        std::swap(the_size_, rhs.the_size_);
        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        return *this;
    }

private:
    // list 节点
    struct Node
    {
        Object data_;
        Node* prev_;
        Node* next_;

        explicit Node(const Object& data = Object{}, Node* prev = nullptr, Node* next = nullptr):
            data_{data}, prev_{prev}, next_{next}
        {}
        // 移动构造
        explicit Node(Object&& data, Node* prev = nullptr, Node* next = nullptr):
            data_{std::move(data)}, prev_{prev}, next_{next}
        {}
    };

    int the_size_;
    Node *head_;
    Node *tail_;
};

#endif //__LIST_H__
