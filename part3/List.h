//
// Created by DDRHb on 2019/8/30.
//

#ifndef __LIST_H__
#define __LIST_H__

#include <iostream>

// 数据结构与算法分析 第四版 第三章 list
// 双向链表 包含头指针 尾指针

namespace DS
{
    template <typename Object>
    class List
    {

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

    public:
        // 常量迭代器
        class const_iterator
        {
            friend class List<Object>;
        public:
            const_iterator():
                current_{nullptr}
            {}

            const Object& operator*() const
            { return retrieve(); }

            const_iterator& operator++()
            {
                current_ = current_->next_;
                return *this;
            }

            const_iterator operator++(int)
            {
                const_iterator old = *this;
                ++(*this);
                return old;
            }

            const_iterator& operator--()
            {
                current_ = current_->prev_;
                return *this;
            }

            const_iterator operator--(int)
            {
                const_iterator old = *this;
                --(*this);
                return old;
            }

            bool operator==(const const_iterator& rhs) const
            { return current_ == rhs.current_; }

            bool operator!=(const const_iterator& rhs) const
            { return current_ != rhs.current_; }

        protected:
            Node* current_;

            Object& retrieve() const
            { return current_->data_; }

            const_iterator(Node* p):
                current_{p}
            {}
        };
        // 迭代器
        class iterator: public const_iterator
        {
            friend class List<Object>;
        public:
            iterator() = default;

            Object& operator*()
            { return const_iterator::retrieve(); }

            const Object& operator*() const
            { return const_iterator::operator*(); }

            iterator& operator++()
            {
                this->current_ = this->current_->next_;
                return *this;
            }

            iterator operator++(int)
            {
                iterator old = *this;
                ++(*this);
                return old;
            }

            iterator& operator--()
            {
                this->current_ = this->current_->prev_;
                return *this;
            }

            iterator operator--(int)
            {
                iterator old = *this;
                --(*this);
                return old;
            }

            bool operator== (const iterator & rhs) const
            { return this->current_ == rhs.current_; }

            bool operator!= (const iterator & rhs) const
            { return !(*this == rhs); }

        protected:
            iterator(Node* p):
                const_iterator{p}
            {}
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
            // 拷贝构造函数需要对头尾指针进行初处理
            head_->next_ = tail_;
            tail_->prev_ = head_;
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
            // 将rhs的head和tail置空，使得rhs链表中的表节点不被delete
            rhs.head_ = nullptr;
            rhs.tail_ = nullptr;
        }

        List& operator= (List&& rhs) noexcept
        {
            // 同Vector类似，交换后不置空，由rhs调用析构函数
            std::swap(the_size_, rhs.the_size_);
            std::swap(head_, rhs.head_);
            std::swap(tail_, rhs.tail_);
            return *this;
        }

        iterator begin()
        { return iterator(head_->next_); }

        const_iterator begin() const
        { return const_iterator(head_->next_); }

        // 返回末尾元素后一个
        iterator end()
        { return iterator(tail_); }

        const_iterator end() const
        { return const_iterator(tail_); }

        int size() const
        { return the_size_; }

        bool empty() const
        { return size() == 0; }

        void clear()
        {
            if(!empty())
                erase(begin(), end());
        }

        Object& front()
        { return *begin(); }

        const Object& front() const
        { return *begin(); }

        Object& back()
        { return *--end(); }

        const Object& back() const
        { return *--end(); }

        void push_front(const Object& x)
        { insert(begin(), x); }

        void push_back(const Object& x)
        { insert(end(), x); }

        void push_front(Object&& x)
        { insert(begin(), std::move(x)); }

        void push_back(Object&& x)
        { insert(end(), std::move(x)); }

        void pop_front()
        { erase(begin()); }

        void pop_back()
        { erase(--end()); }

        // 在目标点之前插入节点，返回插入点
        iterator insert(iterator iter, const Object& x)
        {
            Node* p = iter.current_;
            ++the_size_;
            return iterator(p->prev_ = p->prev_->next_ = new Node{x, p->prev_, p});
        }
        // 在目标点之前插入节点，返回插入点
        iterator insert(iterator iter, Object&& x)
        {
            Node* p = iter.current_;
            ++the_size_;
            return iterator(p->prev_ = p->prev_->next_ = new Node{std::move(x), p->prev, p});
        }

        // 清除目标点，返回目标点的下一个点
        iterator erase(iterator iter)
        {
            if(iter == end()) // 如果是end()，则什么也不做，返回end()
                return end();
            Node* p = iter.current_;
            iterator ret_val(p->next_); // 返回值
            p->prev_->next_ = p->next_;
            p->next_->prev_ = p->prev_;
            delete p;
            --the_size_;

            return ret_val;
        }

        // 删除到to之前一个node，返回to(或者list.end())
        iterator erase(iterator from, iterator to)
        {
            iterator itr = from;
            for(; itr != to && itr != end();)
                itr = erase(itr); // itr更新为删除点的下一个节点
            return itr;
        }
    };
}

#endif //__LIST_H__