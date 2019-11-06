#ifndef PAIRING_HEAP_HPP
#define PAIRING_HEAP_HPP

#include <iostream>
#include <stdexcept>
#include "../lib/dsexceptions.h"

// Pairing heap class
// 适合decreaseKey修改节点关键字值的场景
// 小根堆
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// PairNode & insert( x ) --> Insert x
// pop( minItem )   --> Remove (and optionally return) top item
// Comparable top( )  --> Return top item
// bool empty( )        --> Return true if empty; else false
// void clear( )      --> Remove all items
// void decreaseKey( Position p, newVal )
//                        --> Decrease value in Position p
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    template <typename Object>
    class PairingHeap
    {
    private:
        class PairNode
        {
        public:
            Object element_;
            PairNode* first_child_;
            PairNode* next_sibling_;
            PairNode* prev_;

            explicit PairNode(const Object& e, PairNode* fc = nullptr, PairNode* ns = nullptr, PairNode* p = nullptr)
            : element_{e}, first_child_{fc}, next_sibling_{ns}, prev_{p}
            {}

            explicit PairNode(Object&& e, PairNode* fc = nullptr, PairNode* ns = nullptr, PairNode* p = nullptr)
            : element_{std::move(e)}, first_child_{fc}, next_sibling_{ns}, prev_{p}
            {}
        };

        PairNode* root_;

        void clone(PairNode*& root, PairNode* other)
        {
            if(nullptr == other)
                return;
            root = new PairNode{other->element_};
            if(other->first_child_)
            {
                clone(root->first_child_, other->first_child_);
                root->first_child_->prev_ = root;
            }
            if(other->next_sibling_)
            {
                clone(root->next_sibling_, other->next_sibling_);
                root->next_sibling_->prev_ = root;
            }
        }

        void clear(PairNode* root)
        {
            if(nullptr == root)
                return;
            PairNode *fc = root->first_child_;
            PairNode *ns = root->next_sibling_;
            delete root;
            clear(fc);
            clear(ns);
        }

        // 将first和second连接在一起以满足堆序
        // first是树1的根，它可以不是nullptr
        // first->next_sibling_ 在接入时必须是nullptr
        // 所以root节点是唯一的，没有兄弟
        // second是树2的根，可以是nullptr
        // 合并结果到first上
        void compareAndLink(PairNode*& first, PairNode* second)
        {
            if(second == nullptr)
                return;
            if(second->element_ < first->element_)
            {
                // first 成为second的第一个儿子
                second->prev_ = first->prev_;
                first->next_sibling_ = second->first_child_;
                first->prev_ = second;
                if(first->next_sibling_ != nullptr)
                    first->next_sibling_->prev_ = first;
                second->first_child_ = first;
                first = second;
            } else {
                // second 变成 first的第一个儿子
                second->prev_ = first;
                first->next_sibling_ = second->next_sibling_;
                if(first->next_sibling_)
                    first->next_sibling_->prev_ = first;
                second->next_sibling_ = first->first_child_;
                if(second->next_sibling_)
                    second->next_sibling_->prev_ = second;
                first->first_child_ = second;
            }
        }

        // 实现两趟合并的方法
        // firstSibling是合并后树的根，假设不是nullptr
        PairNode* combineSiblings(PairNode* firstSibling)
        {
            if(firstSibling->next_sibling_ == nullptr)
                return firstSibling;
            // 保存子树用
            static std::vector<PairNode*> tree_array(5);
            // 遍历firstSibling和其所有兄弟，保存子树
            int n_siblings = 0;
            for(; firstSibling != nullptr; ++n_siblings)
            {
                if(n_siblings == tree_array.size())
                    tree_array.resize(n_siblings * 2);
                tree_array[n_siblings] = firstSibling;
                firstSibling = firstSibling->next_sibling_;
            }
            if(n_siblings == tree_array.size())
                tree_array.resize(n_siblings + 1);
            tree_array[n_siblings] = nullptr;

            // 由于在合并时，每个子树的根的兄弟会被舍弃(视为nullptr)
            // 所以用tree_array保存树根的兄弟，并用于后续合并
            // combine
            int i = 0;
            for(; i + 1 < n_siblings; i += 2) // 两两合并
                compareAndLink(tree_array[i], tree_array[i + 1]);
            int j = i - 2;
            if(j == n_siblings - 3) // n_siblings 为奇数，合并遗漏的一棵子树
                compareAndLink(tree_array[j], tree_array[j + 2]);
            for(; j >= 2; j -= 2) // 将上一步合并的树一一合并
                compareAndLink(tree_array[j - 2], tree_array[j]);
            return tree_array[0];
        }

    public:
        typedef PairNode* Position;

        explicit PairingHeap()
        : root_{nullptr}
        {}

        ~PairingHeap()
        {
            clear();
        }

        PairingHeap(const PairingHeap& rhs)
        : root_{nullptr}
        {
            if(rhs.root_)
                clone(root_, rhs.root_);
        }

        PairingHeap(PairingHeap&& rhs) noexcept
        : root_{rhs.root_}
        {
            rhs.root_ = nullptr;
        }

        PairingHeap& operator=(const PairingHeap& rhs)
        {
            PairingHeap copy{rhs};
            std::swap(*this, copy);
            return *this;
        }

        PairingHeap& operator=(PairingHeap&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            return *this;
        }

        bool empty() const
        {
            return root_ == nullptr;
        }

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
            PairNode* old_root = root_;
            if(root_->first_child_ == nullptr)
                root_ = nullptr;
            else
                root_ = combineSiblings(root_->first_child_);
            delete old_root;
        }

        void pop(Object& item)
        {
            item = top();
            pop();
        }

        void clear()
        {
            clear(root_);
            root_ = nullptr;
        }

        PairNode* insert(const Object& x)
        {
            PairNode* new_node = nullptr;
            new_node = new PairNode(x);
            if(nullptr == root_)
                root_ = new_node;
            else
                compareAndLink(root_, new_node);
            return new_node;
        }

        PairNode* insert(Object&& x)
        {
            PairNode* new_node = nullptr;
            new_node = new PairNode{std::move(x)};
            if(nullptr == root_)
                root_ = new_node;
            else
                compareAndLink(root_, new_node);
            return new_node;
        }

        // 减小key的值
        void decreaseKey(PairNode* p, const Object& new_val)
        {
            if(p->element_ < new_val)
                throw std::invalid_argument("new val should be not large than old val");
            if(new_val < p->element_)
            {
                p->element_ = new_val; // 点p改值后可能小于所属的父节点
                if (p != root_) // p不是根
                {
                    // p有兄弟的话，p兄弟的prev连接p的prev
                    if (p->next_sibling_ != nullptr)
                        p->next_sibling_->prev_ = p->prev_;
                    if (p->prev_->first_child_ == p) // 如果p是父节点的第一个儿子
                        p->prev_->first_child_ = p->next_sibling_;
                    else // 否则p的上一个(prev)兄弟直连p的下一个兄弟
                        p->prev_->next_sibling_ = p->next_sibling_;
                    p->next_sibling_ = nullptr;
                    compareAndLink(root_, p);
                }
            }
        }

        // 减小key的值
        void decreaseKey(PairNode* p, Object&& new_val)
        {
            if(p->element_ < new_val)
                throw std::invalid_argument("new val should be not large than old val");
            if(new_val < p->element_)
            {
                p->element_ = std::move(new_val); // 点p改值后可能小于所属的父节点
                if (p != root_) // p不是根
                {
                    // p有兄弟的话，p兄弟的prev连接p的prev
                    if (p->next_sibling_ != nullptr)
                        p->next_sibling_->prev_ = p->prev_;
                    if (p->prev_->first_child_ == p) // 如果p是父节点的第一个儿子
                        p->prev_->first_child_ = p->next_sibling_;
                    else // 否则p的上一个(prev)兄弟直连p的下一个兄弟
                        p->prev_->next_sibling_ = p->next_sibling_;
                    p->next_sibling_ = nullptr;
                    compareAndLink(root_, p);
                }
            }
        }
    };
}
#endif //PAIRING_HEAP_HPP
