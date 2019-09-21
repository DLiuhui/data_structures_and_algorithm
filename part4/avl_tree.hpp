// 平衡树

#ifndef __AVL_TREE_HPP__
#define __AVL_TREE_HPP__

#include "../lib/dsexceptions.h"
#include <iostream>

// AVL Tree ADT
// 二叉平衡树
// 非优化平衡树，平衡调节通过旋转，每次插入删除均进行平衡调整
// 需要实现的功能
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x
// bool contains( x )     --> Return true if x is present
// Object findMin( )      --> Return smallest item
// Object findMax( )      --> Return largest item
// bool empty( )          --> Return true if empty; else false
// void clear( )          --> Remove all items
// void printTree( )      --> Print tree in sorted order
// int height()           --> the height of the tree
// ******************ERRORS********************************
// Throws UnderflowException as warranted

namespace DS
{
    template <typename Object>
    class AVLTree
    {
    private:
        class TreeNode
        {
        public:
            Object element_;
            TreeNode* left_;
            TreeNode* right_;
            int height_;

            explicit TreeNode(const Object& element, TreeNode* lt = nullptr, TreeNode* rt = nullptr, int h = 0)
                : element_{element}, left_{lt}, right_{rt}, height_{h}
            {}

            explicit TreeNode(Object&& element, TreeNode* lt = nullptr, TreeNode* rt = nullptr, int h = 0)
                : element_{std::move(element)}, left_{lt}, right_{rt}, height_{h}
            {}
        };

        // 返回节点node的高度
        int height(TreeNode* node) const
        { return (nullptr == node) ? -1 : node->height_; }

        void adjustHeight(TreeNode*& node)
        {
            int h_l = height(node->left_), h_r = height(node->right_);
            node->height_ = (h_l > h_r ? h_l : h_r) + 1;
        }

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

        // 在插入后进行平衡调整
        // 递归进行平衡调整
        void insertProcess(const Object& x, TreeNode*& node)
        {
            if(node == nullptr)
                node = new TreeNode(x);
            else if(x > node->element_)
                insertProcess(x, node->right_);
            else if(x < node->element_)
                insertProcess(x, node->left_);
            balance(node);
        }

        // 在插入后进行平衡调整
        // 递归进行平衡调整
        void insertProcess(Object&& x, TreeNode*& node)
        {
            if(node == nullptr)
                node = new TreeNode(std::move(x));
            else if(x > node->element_)
                insertProcess(std::move(x), node->right_);
            else if(x < node->element_)
                insertProcess(std::move(x), node->left_);
            balance(node);
        }

        // 在删除后进行平衡调整
        // 递归进行平衡调整
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
                    // x存在空子树
                    // 设置成非空子树的根，或者置空
                    node = (node->left_ == nullptr) ? node->right_ : node->left_;
                    delete old_node;
                }
                else
                {
                    // 如果x左右均非空，将x换成左子树的最大点（或者右子树的最小点）
                    //  递归删除左子树最大点（右子树最小点）
                    node->element_ = findMaxProcess(node->left_)->element_;
                    removeProcess(node->element_, node->left_);
                }
            }
            balance(node);
        }

        static const int ALLOWED_IMBALANCE = 1;

        void balance(TreeNode*& node)
        {
            if(node == nullptr)
                return;
            if(height(node->left_) - height(node->right_) > ALLOWED_IMBALANCE) // 左端失衡
            {
                if(height(node->left_->left_) >= height(node->left_->right_)) // 左左，向右单次旋转
                    rotateWithLeftChild(node);
                else // 左右，双旋转 左旋->右旋
                    doubleWithLeftChild(node);
            }
            else if(height(node->right_) - height(node->left_) > ALLOWED_IMBALANCE) // 右端失衡
            {
                if(height(node->right_->right_) >= height(node->right_->left_)) // 右右，向左单次旋转
                    rotateWithRightChild(node);
                else // 右左，双旋转 右旋->左旋
                    doubleWithRightChild(node);
            }
            adjustHeight(node);
        }

        void rotateWithLeftChild(TreeNode*& root)
        {
            // new_root 作为新的 root
            // root 作为 new_root->right
            TreeNode* new_root = root->left_;
            root->left_ = new_root->right_;
            new_root->right_ = root;
            // adjust height
            adjustHeight(root); // 先调整 root(new_root的子树)
            adjustHeight(new_root); // 再调整 new_root
            root = new_root; // 修改parent指针
        }

        void rotateWithRightChild(TreeNode*& root)
        {
            // new_root 作为新的 root
            // root 作为 new_root->left
            TreeNode* new_root = root->right_;
            root->right_ = new_root->left_;
            new_root->left_ = root;
            // adjust height
            adjustHeight(root); // 先调整 root(new_root的子树)
            adjustHeight(new_root); // 再调整 new_root
            root = new_root; // 修改parent指针
        }

        void doubleWithLeftChild(TreeNode*& root)
        {
            rotateWithRightChild(root->left_);
            rotateWithLeftChild(root);
        }

        void doubleWithRightChild(TreeNode*& root)
        {
            rotateWithLeftChild(root->right_);
            rotateWithRightChild(root);
        }

        TreeNode* root_;

    public:
        AVLTree()
            : root_{nullptr}
        {}

        // 拷贝，由于并没有使用智能指针，需要进行完全复制深拷贝clone
        AVLTree(const AVLTree& rhs)
            : root_{nullptr}
        { root_ = cloneTree(rhs.root_); }

        AVLTree(AVLTree&& rhs) noexcept
            : root_{rhs.root_}
        { rhs.root_ = nullptr; }

        ~AVLTree()
        { clear(); }

        AVLTree& operator=(const AVLTree& rhs)
        {
            AVLTree copy(rhs);
            std::swap(*this, copy);
            return *this;
        }

        AVLTree& operator=(AVLTree&& rhs) noexcept
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
#endif //__AVL_TREE_HPP__
