#ifndef BINOMIAL_QUEUE_HPP
#define BINOMIAL_QUEUE_HPP

#include <iostream>
#include <vector>
#include "../lib/dsexceptions.h"

// Binomial queue class
// 二项队列
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// pop( )           --> Return and remove smallest item
// Comparable top( )  --> Return smallest item
// bool empty( )        --> Return true if empty; else false
// void clear( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    // 默认的比较函数
    template <typename Object>
    class IsLessBQ
    {
    public:
        bool operator() (const Object& lhs, const Object& rhs) const
        {
            return lhs < rhs;
        }
    };

    template <typename Object, class Compare = IsLessBQ<Object>>
    class BinomialQueue
    {
    public:
        explicit BinomialQueue(const Compare& comp = Compare())
        : trees_(DEFAULT_TREES), size_{0}, compare_{comp}
        {
            for(auto& root : trees_)
                root = nullptr;
        }

        explicit BinomialQueue(const Object& item, const Compare& comp = Compare())
        : trees_(1), size_{1}, compare_{comp}
        {
            trees_[0] = new BinomialNode(item, nullptr, nullptr);
        }

        explicit BinomialQueue(Object&& item, const Compare& comp = Compare())
        : trees_(1), size_{1}, compare_{comp}
        {
            trees_[0] = new BinomialNode(std::move(item), nullptr, nullptr);
        }

        BinomialQueue(const BinomialQueue& rhs)
        : trees_(rhs.trees_.size(), nullptr), size_{rhs.size_}, compare_{rhs.compare_}
        {
            for(int i = 0; i < rhs.trees_.size(); ++i)
                clone(trees_[i], rhs.trees_[i]);
        }

        BinomialQueue(BinomialQueue&& rhs) noexcept
        : trees_{std::move(rhs.trees_)}, size_{rhs.size_}, compare_{std::move(rhs.compare_)}
        {}

        ~BinomialQueue() noexcept
        { clear(); }

        BinomialQueue& operator=(const BinomialQueue& rhs)
        {
            BinomialQueue copy(rhs);
            std::swap(*this, copy);
            return *this;
        }

        BinomialQueue& operator=(BinomialQueue&& rhs) noexcept
        {
            std::swap(size_, rhs.size_);
            std::swap(trees_, rhs.trees_);
            std::swap(compare_, rhs.compare_);
            return *this;
        }

        bool empty() const
        { return size_ == 0; }

        int size() const
        { return size_; }

        void clear()
        {
            size_ = 0;
            for(auto& root : trees_)
                clear(root);
        }

        const Object& top() const
        {
            if(empty())
                throw UnderflowException{};
            return trees_[findTopIndex()]->element_;
        }

        // 允许重复
        // 插入即为特殊的合并
        void insert(const Object& x)
        {
            BinomialQueue new_tree(x, compare_);
            merge(new_tree);
        }

        void insert(Object&& x)
        {
            BinomialQueue new_tree(std::move(x), compare_);
            merge(new_tree);
        }

        void pop()
        {
            Object item;
            pop(item);
        }

        void pop(Object& item)
        {
            if(empty())
                throw UnderflowException{};
            int top_id = findTopIndex(); // 找到待删除的节点id
            item = trees_[top_id]->element_;
            BinomialNode* old = trees_[top_id];
            BinomialNode* rest_tree = old->first_child_;

            delete old; // 删除根
            // 构造树H''
            BinomialQueue rest_queue(compare_);
            rest_queue.trees_.resize(top_id);
            rest_queue.size_ = (1 << top_id) - 1;
            for(int j = top_id - 1; j >= 0; --j)
            {
                rest_queue.trees_[j] = rest_tree;
                rest_tree = rest_tree->next_sibling_;
                rest_queue.trees_[j]->next_sibling_ = nullptr;
            }
            // 将当前树this构造成H'
            trees_[top_id] = nullptr;
            size_ -= rest_queue.size_ + 1;

            // 合并h' h''
            merge(rest_queue);
        }

        // 合并
        void merge(BinomialQueue& rhs)
        {
            if(this == &rhs)
                return;
            size_ += rhs.size_;

            if(size_ > capacity()) // 表扩张
            {
                int old_n_trees = trees_.size();
                // 新的表长=合并项中表长大者+1
                int new_n_trees = std::max(trees_.size(), rhs.trees_.size()) + 1;
                trees_.resize(new_n_trees);
                for(int i = old_n_trees; i < new_n_trees; ++i)
                {
                    trees_[i] = nullptr; // 新的表节点初始化nullptr
                }
            }

            BinomialNode* carry = nullptr; // 从循环上一步得到的树
            BinomialNode* t1 = nullptr;
            BinomialNode* t2 = nullptr;
            for(int i = 0, j = 1; j <= size_; ++i, j *= 2)
            {
                // 用i遍历每棵树
                t1 = trees_[i];
                t2 = i < rhs.trees_.size() ? rhs.trees_[i] : nullptr;

                int which_case = t1 == nullptr ? 0 : 1;
                which_case += t2 == nullptr ? 0 : 2;
                which_case += carry == nullptr ? 0 : 4;

                if(which_case < 2)
                    continue;

                switch(which_case)
                {
                    // case 0, 1 do nothing
                    // case 0: // no trees
                    // case 1: // only this
                    case 2: /* Only rhs */
                        trees_[i] = t2;
                        rhs.trees_[i] = nullptr;
                        break;
                    case 4: /* Only carry */
                        trees_[i] = carry;
                        carry = nullptr;
                        break;
                    case 3: /* this and rhs */
                        carry = combineTrees(t1, t2);
                        trees_[i] = rhs.trees_[i] = nullptr;
                        break;
                    case 5: /* this and carry */
                        carry = combineTrees(t1, carry);
                        trees_[i] = nullptr;
                        break;
                    case 6: /* rhs and carry */
                        carry = combineTrees(t2, carry);
                        rhs.trees_[i] = nullptr;
                        break;
                    case 7: /* All three */
                        trees_[i] = carry;
                        carry = combineTrees(t1, t2);
                        rhs.trees_[i] = nullptr;
                        break;
                    default:
                        break;
                }
            }
            // 消除rhs
            for(auto& root : rhs.trees_)
                root = nullptr;
            rhs.size_ = 0;
        }


    private:
        class BinomialNode
        {
        public:
            Object element_;
            BinomialNode* first_child_;
            BinomialNode* next_sibling_; // 二项队列中的树不是二叉树，所有用孩子兄弟法表示

            explicit BinomialNode(const Object& e, BinomialNode* lt = nullptr, BinomialNode* rt = nullptr)
            : element_{e}, first_child_{lt}, next_sibling_{rt}
            {}

            explicit BinomialNode(Object&& e, BinomialNode* lt = nullptr, BinomialNode* rt = nullptr)
            : element_{std::move(e)}, first_child_{lt}, next_sibling_{rt}
            {}
        };

        const static int DEFAULT_TREES = 1;

        std::vector<BinomialNode*> trees_; // 二项树列表
        int size_; // 记录队列中项数
        Compare compare_; // 比较函数

        int findTopIndex() const
        {
            int i;
            int top_id;
            for(i = 0; trees_[i] == nullptr; ++i) // 找到树表中第一个不为null的根
                ;
            for(top_id = i; i < trees_.size(); ++i)
            {
                if(trees_[i] != nullptr && compare_(trees_[i]->element_, trees_[top_id]->element_))
                    top_id = i;
            }
            return top_id;
        }

        // 返回二项队列的容量 2^k - 1, k = 队列长度
        int capacity() const
        { return (1 << trees_.size()) - 1;}

        void clear(BinomialNode*& node)
        {
            if(nullptr == node)
                return;
            BinomialNode* lt = node->first_child_;
            BinomialNode* rt = node->next_sibling_;
            delete node;
            node = nullptr;
            clear(rt);
            clear(lt);
        }

        void clone(BinomialNode*& node, BinomialNode* rhs)
        {
            if(nullptr == rhs)
                return;
            node = new BinomialNode(rhs->element_, nullptr, nullptr);
            clone(node->next_sibling_, rhs->next_sibling_);
            clone(node->first_child_, rhs->first_child_);
        }

        // Return the result of merging equal-sized t1 and t2.
        // 合并两棵同样大小的树
        BinomialNode* combineTrees(BinomialNode* t1, BinomialNode* t2)
        {
            if(compare_(t2->element_, t1->element_))
                return combineTrees(t2, t1);
            t2->next_sibling_ = t1->first_child_;
            t1->first_child_ = t2;
            return t1;
        }
    };
}
#endif //BINOMIAL_QUEUE_HPP
