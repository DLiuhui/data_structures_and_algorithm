#ifndef SPLAY_TREE_HPP
#define SPLAY_TREE_HPP

#include <iostream>
#include "../lib/dsexceptions.h"

// 区别于二叉搜索树BST和平衡二叉树AVL，伸展树本质在BST基础上还规定类每次访问之后将访问的节点变成根节点，
// 这样使得被访问过的节点尽可能靠近根节点，这样下次再次访问时更快定位。
// 因此不像AVL那样需要保存高度信息节省了存储空间，具有二叉查找树的所有性质，在性能上又比普通的二叉查找树有所改进。
// 普通的二叉查找树在最坏情况下的查找操作的时间复杂度为O(n)（当二叉树退化成一条链的时候）
// 而伸展树在任何情况下的平摊时间复杂度均为 O(logN)

// 变换形式，相比AVL增加一字型旋转

// SplayTree class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// bool empty( )        --> Return true if empty; else false
// void clear( )      --> Remove all items
// void print( )      --> Print tree in sorted order
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    template <typename Object>
    class SplayTree
    {
    public:
        SplayTree()
        {
            null_node_ = new TreeNode{};
            null_node_->left_ = null_node_->right_ = null_node_;
            root_ = null_node_;
        }

        SplayTree(const SplayTree& rhs)
        {
            null_node_ = new TreeNode{};
            null_node_->left_ = null_node_->right_ = null_node_;
            clone(root_, rhs.root_);
        }

        SplayTree(SplayTree&& rhs) noexcept
        : root_{rhs.root_}, null_node_{rhs.null_node_}
        {
            rhs.root_ = nullptr;
            rhs.null_node_ = nullptr;
        }

        ~SplayTree()
        {
            clear();
            delete null_node_;
            null_node_ = nullptr;
        }

        SplayTree& operator=(const SplayTree& rhs)
        {
            SplayTree copy{rhs};
            std::swap(*this, copy);
            return *this;
        }

        SplayTree& operator=(SplayTree&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            std::swap(null_node_, rhs.null_node_);
            return *this;
        }

        bool empty() const
        {
            return root_ == null_node_;
        }

        bool contains(const Object& x)
        {
            if(empty())
                return false;
            splay(x, root_);
            return root_->element_ == x;
        }

        void print() const
        {
            if(empty())
                std::cout << "Empty tree" << std::endl;
            else
                print(root_);
        }

        /* Not the most efficient implementation (uses two passes),
         * but has correct amortized behavior.
         * A good alternative is to first call find with parameter
         * smaller than any item in the tree, then call findMin.
         * Return the smallest item or throw UnderflowException if empty.
        */
        const Object& findMin()
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->left_ != null_node_)
                ptr = ptr->left_;

            splay(ptr->element_, root_);
            return ptr->element_;
        }

        const Object& findMax()
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->right_ != null_node_)
                ptr = ptr->right_;

            splay(ptr->element_, root_);
            return ptr->element_;
        }

        void clear()
        {
            while(!empty())
            {
                findMax(); // splay max item to root
                remove(root_->element_);
            }
        }

        void insert(const Object& x)
        {
            TreeNode* new_node = nullptr;
            new_node = new TreeNode{x};

            if(root_ == null_node_)
            {
                new_node->left_ = new_node->right_ = null_node_;
                root_ = new_node;
            } else{
                splay(x, root_);
                if(x < root_->element_)
                {
                    new_node->left_ = root_->left_;
                    new_node->right_ = root_;
                    root_->left_ = null_node_;
                    root_ = new_node;
                } else if(root_->element_ < x){
                    new_node->right_ = root_->right_;
                    new_node->left_ = root_;
                    root_->right_ = null_node_;
                    root_ = new_node;
                } else{
                    return;
                }
            }
        }

        void remove(const Object& x)
        {
            if(!contains(x)) // 调用contain的同时，如果x存在，会被放在root节点
                return;

            TreeNode* new_tree; // 保存root的子树

            if(root_->left_ == null_node_)
                new_tree = root_->right_;
            else
            {
                // 找左子树的最大节点
                // 将其splay到new_tree的根
                new_tree = root_->left_;
                splay(x, new_tree);
                new_tree->right_ = root_->right_;
            }
            delete root_; // 删除旧根
            root_ = new_tree;
        }

    private:
        class TreeNode
        {
        public:
            Object element_;
            TreeNode* left_;
            TreeNode* right_;

            TreeNode()
            :element_{Object()}, left_{nullptr}, right_{nullptr}
            {}

            explicit TreeNode(const Object& element, TreeNode* lt = nullptr, TreeNode* rt = nullptr)
            : element_{element}, left_{lt}, right_{rt}
            {}
        };

        TreeNode* root_;
        TreeNode* null_node_; // 空节点指示标志

        TreeNode* clone(TreeNode*& node, TreeNode* new_node) const
        {
            if(new_node == new_node->left_)
                node = null_node_;
            else
            {
                node = new TreeNode(new_node->element_, new_node->left_, new_node->right_);
                clone(node->left_, new_node->left_);
                clone(node->right_, new_node->right_);
            }
        }

        void print(TreeNode* node) const
        {
            if(node != node->left_)
            {
                print(node->left_);
                std::cout << node->element_ << std::endl;
                print(node->right_);
            }
        }

        void rotateWithLeftChild(TreeNode*& k2)
        {
            TreeNode* k1 = k2->left_;
            k2->left_ = k1->right_;
            k1->right_ = k2;
            k2 = k1;
        }

        void rotateWithRightChild(TreeNode*& k1)
        {
            TreeNode* k2 = k1->right_;
            k1->right_ = k2->left_;
            k2->left_ = k1;
            k1 = k2;
        }

        /*
         * Internal method to perform a top-down splay.
         * The last accessed node becomes the new root.
         * This method may be overridden to use a different
         * splaying algorithm, however, the splay tree code
         * depends on the accessed item going to the root.
         * x is the target item to splay around.
         * t is the root of the subtree to splay
         */
        void splay(const Object& x, TreeNode*& node)
        {
            // header/left_max/right_min的作用和变化结合下方while部分代码图解比较好理解
            TreeNode* left_max;
            TreeNode* right_min;
            TreeNode header;
            header.left_ = header.right_ = null_node_;
            left_max = right_min = &header;

            null_node_->element_ = x; // 作为退出条件

            while(true)
            {
                if(x < node->element_) // 含x的节点位于左子树
                {
                    if(x < node->left_->element_) // 含x的节点位于左子树的左子树
                        rotateWithLeftChild(node);
                    if(node->left_ == null_node_) // 含x的节点不存在
                        break;
                    right_min->left_ = node;
                    right_min = node;
                    node = node->left_;
                } else if(node->element_ < x) // 含x的节点位于右子树
                {
                    if(node->right_->element_ < x) // 含x的节点位于右子树的右子树
                        rotateWithRightChild(node);
                    if(node->right_ == null_node_) // 含x的节点不存在
                        break;
                    left_max->right_ = node;
                    left_max = node;
                    node = node->right_;
                } else{ // == x 或者是 == null_node_
                    break;
                }
            }
            // 经过while过程，找到了包含x的节点(如果存在的话)
            // node指向x
            left_max->right_ = node->left_;
            right_min->left_ = node->right_;
            node->left_ = header.right_;
            node->right_ = header.left_;
        }
    };
}
#endif //SPLAY_TREE_HPP
