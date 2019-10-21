#ifndef LEFT_HEAP_HPP
#define LEFT_HEAP_HPP

#include <iostream>
#include "../lib/dsexceptions.h"

// Leftist heap class
// 支持快速合并
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// pop( minItem )   --> Remove (and optionally return) top item
// Comparable top( )  --> Return top item
// bool empty( )        --> Return true if empty; else false
// void clear( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    // 默认的比较函数
    template <typename Object>
    class IsLessLeft
    {
    public:
        bool operator() (const Object& lhs, const Object& rhs) const
        {
            return lhs < rhs;
        }
    };

    template <typename Object, class Compare = IsLessLeft<Object>>
    class LeftistHeap
    {
    public:
        explicit LeftistHeap(const Compare& cmp = Compare())
        : root_{nullptr}, compare_{cmp}
        {}

        LeftistHeap(const LeftistHeap& rhs)
        : root_{nullptr}, compare_{rhs.compare_}
        { clone(root_, rhs.root_); }

        LeftistHeap(LeftistHeap&& rhs) noexcept
        : root_{rhs.root_}, compare_{rhs.compare_}
        {
            rhs.root_ = nullptr; // 置空, rhs析构时不会删除已经隶属于this的子节点
        }

        ~LeftistHeap()
        { clear(); }

        LeftistHeap& operator=(const LeftistHeap& rhs)
        {
            LeftistHeap copy(rhs);
            std::swap(*this, copy);
            return *this;
        }

        LeftistHeap& operator=(LeftistHeap&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            std::swap(compare_, rhs.compare_);
            return *this;
        }

        bool empty() const
        { return root_ == nullptr; }

        // 获取堆顶
        const Object& top() const
        {
            if(empty())
                throw UnderflowException{};
            return root_->element_;
        }

        void pop()
        {
            if(empty())
                throw UnderflowException{};
            LeftistNode* old = root_;
            root_ = merge(root_->left_, root_->right_);
            delete old;
        }

        void pop(Object& item)
        {
            item = top();
            pop();
        }

        void clear()
        {
            clear(root_);
            root_ = nullptr; // 要置空
        }

        // 插入过程相当于把待插入的堆合并到插入节点上
        // Inserts x; duplicates allowed
        void insert(const Object& x)
        { root_ = merge(new LeftistNode(x), root_); }

        // Inserts x; duplicates allowed.
        void insert(Object&& x)
        { root_ = merge(new LeftistNode(std::move(x)), root_); }

        // 将新的堆合并到当前堆
        void merge(LeftistHeap& rhs)
        {
            if(this == &rhs) // 检查是不是本身
                return;
            root_ = merge(root_, rhs.root_);
            rhs.root_ = nullptr; // 置空, rhs析构时不会删除已经隶属于this的子节点
        }

    private:
        class LeftistNode
        {
        public:
            Object element_;
            LeftistNode* left_;
            LeftistNode* right_;
            int npl_; // 记录零路径长

            explicit LeftistNode(const Object& e, LeftistNode* lt = nullptr,
                    LeftistNode* rt = nullptr, int np = 0)
                    : element_{e}, left_{lt}, right_{rt}, npl_{np}
            {}

            explicit LeftistNode(Object&& e, LeftistNode* lt = nullptr,
                    LeftistNode* rt = nullptr, int np = 0)
                    : element_{std::move(e)}, left_{lt}, right_{rt}, npl_{np}
            {}
        };

        LeftistNode* root_; // 根节点
        Compare compare_;

        // 函数本身没有修改root_, 所以仍旧使用const修饰this
        // 采用尾递归消除以尽可能避免堆栈溢出可能
        void clone(LeftistNode*& node, LeftistNode* copy)
        {
            if(nullptr == copy)
                return;
            node = new LeftistNode(copy->element_, nullptr, nullptr, copy->npl_);
            // 优先拷贝高度小的右端
            clone(node->right_, copy->right_);
            // 拷贝高度大的左端，尾递归
            clone(node->left_, copy->left_);
        }

        // 采用尾递归消除以尽可能避免堆栈溢出可能
        void clear(LeftistNode* node)
        {
            if(nullptr == node)
                return;
            LeftistNode* rt = node->right_;
            LeftistNode* lt = node->left_;
            // 删除
            delete node;
            // 优先释放高度小的右端
            clear(rt);
            // 消除高度大的左端，尾递归
            clear(lt);
        }

        // 合并过程
        LeftistNode* merge(LeftistNode* h1, LeftistNode* h2)
        {
            if(h1 == nullptr)
                return h2;
            if(h2 == nullptr)
                return h1;
            if(compare_(h1->element_, h2->element_))
                return doMerge(h1, h2);
            else
                return doMerge(h2, h1);
        }

        // 合并h2到h1
        LeftistNode* doMerge(LeftistNode* h1, LeftistNode* h2)
        {
            if(h1->left_ == nullptr)
                h1->left_ = h2;
            else
            {
                // 递归的先将h2与h1的右子树合并
                h1->right_ = merge(h1->right_, h2);
                if(h1->left_->npl_ < h1->right_->npl_)
                    std::swap(h1->left_, h1->right_);
                // 零路径长是当前节点到一个不具有两个儿子的最短路径长
                h1->npl_ = h1->right_->npl_ + 1; // 更新零路径长
            }
            return h1;
        }
    };
}
#endif //LEFT_HEAP_HPP
