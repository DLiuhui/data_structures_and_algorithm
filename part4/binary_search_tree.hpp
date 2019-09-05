//
// Created by DDRHb on 2019/9/5.
//
#ifndef __BINARY_SEARCH_TREE_HPP__
#define __BINARY_SEARCH_TREE_HPP__

#include "../lib/dsexceptions.h"
#include <iostream>
#include <algorithm>

// BinarySearchTree ADT
// 二叉搜索（排序）树
// 需要实现的功能
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Object findMin( )      --> Return smallest item
// Object findMax( )      --> Return largest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted
// 插入方案：
//  插入的x > 节点值, 插入到右子树，x < 节点值, 插入到左子树，不考虑x = 节点值的情况
// 删除节点方案：
//  寻找x，如果没找到，什么都不做
//  如果找到x
//  如果x是叶节点，直接删除
//  如果x不是叶节点，将x换成左子树的最大点（或者右子树的最小点）
//  递归删除左子树最大点（右子树最小点）

namespace DS
{
    template <typename Object>
    class BinarySearchTree
    {
    private:
        class TreeNode
        {
        public:
            Object element_;
            TreeNode* left_;
            TreeNode* right_;

            explicit TreeNode(const Object& element, TreeNode* lt = nullptr, TreeNode* rt = nullptr)
                : element_{element}, left_{lt}, right_{rt}
            {}

            explicit TreeNode(Object&& element, TreeNode* lt = nullptr, TreeNode* rt = nullptr)
                : element_{std::move(element)}, left_{lt}, right_{rt}
            {}
        };

        TreeNode* cloneTree(TreeNode* root) const
        {
            if(root)  // 树深拷贝，递归复制树的左子树，右子树
                return new TreeNode(root->element_, cloneTree(root->left_), cloneTree(root->right_));
            else
                return nullptr;
        }

        void clearTree(TreeNode*& node)  // 删除树，递归删除树左右子树
        {
            if(node)
            {
                clearTree(node->left_);
                clearTree(node->right_);
                delete node;
            }
            node = nullptr;  // 安全处理，防止脏内存
        }

        TreeNode* findMinProcess(TreeNode* node) const
        {
            if(node == nullptr)
                return nullptr;
            while(node->left_)
                node = node->left_;
            return node;
        }

        TreeNode* findMaxProcess(TreeNode* node) const
        {
            if(node == nullptr)
                return nullptr;
            while(node->right_)
                node = node->right_;
            return node;
        }

        bool containsProcess(const Object& x, TreeNode* node) const
        {
            if(node == nullptr)
                return false;
            else if(node->element_ > x)
                return containsProcess(x, node->left_);
            else if(node->element_ < x)
                return containsProcess(x, node->right_);
            else
                return true;
        }

        void printTreeReverseProcess(TreeNode* node, std::ostream& out = std::cout) const
        {
            if(node == nullptr)
                return;
            printTreeReverseProcess(node->right_);
            out << node->element_ << std::endl;
            printTreeReverseProcess(node->left_);
        }

        void printTreeProcess(TreeNode* node, std::ostream& out = std::cout) const
        {
            if(node == nullptr)
                return;
            printTreeProcess(node->left_);
            out << node->element_ << std::endl;
            printTreeProcess(node->right_);
        }

        // 由于需要永久修改节点指针指向的元素
        // 所以采用TreeNode*&，指针的引用 / 或者 TreeNode**
        void insertProcess(const Object& x, TreeNode*& node)
        {
            if(node == nullptr)
                node = new TreeNode(x);
            else if(x > node->element_)
                insertProcess(x, node->right_);
            else if(x < node->element_)
                insertProcess(x, node->left_);
        }

        // 由于需要永久修改节点指针指向的元素
        // 所以采用TreeNode*&，指针的引用 / 或者 TreeNode**
        void insertProcess(Object&& x, TreeNode*& node)
        {
            if(node == nullptr)
                node = new TreeNode(std::move(x));
            else if(x > node->element_)
                insertProcess(std::move(x), node->right_);
            else if(x < node->element_)
                insertProcess(std::move(x), node->left_);
        }

        // 由于需要永久修改节点指针指向的元素
        // 所以采用TreeNode*&，指针的引用 / 或者 TreeNode**
        void removeProcess(const Object& x, TreeNode*& node)
        {
            if(node == nullptr)
                return;
            if(node->element_ > x)
                removeProcess(x, node->left_);
            else if(node->element_ < x)
                removeProcess(x, node->right_);
            else
            {
                if(node->left_ == nullptr || node->right_ == nullptr)
                {
                    TreeNode* old_node = node;
                    // 设置成非空子树的根，或者置空
                    node = (node->left_ == nullptr) ? node->right_ : node->left_;
                    delete old_node;
                }
                else
                {
                    // 如果x不是叶节点，将x换成左子树的最大点（或者右子树的最小点）
                    //  递归删除左子树最大点（右子树最小点）
                    node->element_ = findMaxProcess(node->left_)->element_;
                    removeProcess(node->element_, node->left_);
                    // node->element_ = findMinProcess(node->right_)->element_;
                    // removeProcess(node->element_, node->right_);
                }
            }
        }

        TreeNode* root_;

    public:
        BinarySearchTree()
            : root_{nullptr}
        {}

        // 拷贝，由于并没有使用智能指针，需要进行完全复制深拷贝clone
        BinarySearchTree(const BinarySearchTree& rhs)
            : root_{nullptr}
        { root_ = cloneTree(rhs.root_); }

        BinarySearchTree(BinarySearchTree&& rhs) noexcept
            : root_{rhs.root_}
        { rhs.root_ = nullptr; }

        ~BinarySearchTree()
        { clear(); }

        BinarySearchTree& operator=(const BinarySearchTree& rhs)
        {
            BinarySearchTree copy(rhs);
            std::swap(*this, copy);
            return *this;
        }

        BinarySearchTree& operator=(BinarySearchTree&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            return *this;
        }

        // 判断空树
        bool empty() const
        { return root_ == nullptr; }

        // 判断树是否包含x
        bool contains(const Object& x) const
        {
            return containsProcess(x, root_);
        }

        const Object& findMin() const
        {
            if(empty())
                throw UnderflowException{};
            return findMinProcess(root_)->element_;
        }

        const Object& findMax() const
        {
            if(empty())
                throw UnderflowException{};
            return findMaxProcess(root_)->element_;
        }

        // 按照排序顺序打印所有节点值，默认升序
        void printTree(std::ostream& out = std::cout, bool reverse = false) const
        {
            if(empty())
                out << "Empty tree" << std::endl;
            else if(reverse)
                printTreeReverseProcess(root_, out);
            else
                printTreeProcess(root_, out);
        }

        void clear()
        { clearTree(root_); }

        // 插入，重复项不考虑
        void insert(const Object& x)
        {
            insertProcess(x, root_);
        }

        // 插入，重复项不考虑
        void insert(Object&& x)
        {
            insertProcess(std::move(x), root_);
        }

        // 删除x，存在则删除，不存在则不进行操作
        void remove(const Object& x)
        {
            removeProcess(x, root_);
        }

    };
}
#endif //__BINARY_SEARCH_TREE_HPP__
