#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <iostream>
#include "../lib/dsexceptions.h"

// Red-black tree class
//
// CONSTRUCTION: with negative infinity object also
//               used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
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
    class RedBlackTree
    {
    private:
        enum Color {RED, BLACK};

        class TreeNode
        {
        public:
            Object element_;
            TreeNode* left_;
            TreeNode* right_;
            TreeNode* parent_;
            Color color_;

            explicit TreeNode(const Object& e = Object{}, Color c = BLACK,
                    TreeNode* lt = nullptr, TreeNode* rt = nullptr, TreeNode* p = nullptr)
            : element_{e}, left_{lt}, right_{rt}, parent_{p}, color_{c}
            {}

            explicit TreeNode(Object&& e, Color c = BLACK,
                    TreeNode* lt = nullptr, TreeNode* rt = nullptr, TreeNode* p = nullptr)
            : element_{std::move(e)}, left_{lt}, right_{rt}, parent_{p}, color_{c}
            {}
        };

        TreeNode* root_;

        void clone(TreeNode*& node, TreeNode* node_parent, TreeNode* new_node)
        {
            if(nullptr == new_node) // 空节点
                return;
            else
            {
                node = new TreeNode(new_node->element_, new_node->color_, nullptr, nullptr, node_parent);
                clone(node->left_, node, new_node->left_);
                clone(node->right_, node, new_node->right_);
            }
        }

        void clear(TreeNode* root)
        {
            if(nullptr == root)
                return;
            TreeNode *lt = root->left_;
            TreeNode *rt = root->right_;
            delete root;
            clear(lt);
            clear(rt);
        }

        void print(TreeNode* node) const
        {
            if(nullptr == node)
                return;
            print(node->left_);
            std::cout << node->element_ << " ";
            print(node->right_);
        }

        void insert(const Object& x, TreeNode*& current, TreeNode* parent)
        {
            if(nullptr == current)
            {
                current = new TreeNode(x, RED, nullptr, nullptr, parent);
                if(parent->color_ == RED)
                    insertFixUp(current);
            } else if(current->element_ == x)
            {
                return;
            } else if(x < current->element_)
            {
                insert(x, current->left_, current);
            } else {
                insert(x, current->right_, current);
            }
        }

        void insertFixUp(TreeNode*& node)
        {
            // current 默认是红色
            // 插入修复
            TreeNode* current = node;
            TreeNode* parent = current->parent_; // 父节点
            TreeNode* grand = nullptr; // 祖父节点
            TreeNode* uncle = nullptr; // 叔叔节点
            // 向上调整，直到根节点
            while(parent && parent->parent_ && parent->color_ == RED)
            {
                grand = parent->parent_;
                if(parent == grand->left_) // 父节点是祖父的左儿子
                {
                    uncle = grand->right_;
                    // 当前结点的父结点是红色且祖父结点的另一个子结点（叔叔结点）是黑色（空节点也可以视为黑色）
                    if(uncle == nullptr || uncle->color_ == BLACK)
                    {
                        // 情况1 当前结点的父结点是红色，叔叔结点是黑色，当前结点是其父结点的右子
                        // 进行一次左旋，转换成情况2
                        if(current == parent->right_)
                        {
                            rotateWithRightChild(parent);
                            current = parent; // current 指向旋转之后的parent节点
                            parent = current->parent_;
                        }
                        // 情况2 当前结点的父结点是红色，叔叔结点是黑色，当前结点是其父结点的左子
                        parent->color_ = BLACK;
                        grand->color_ = RED;
                        rotateWithLeftChild(grand);
                    }
                    else // 情况3 叔叔节点存在且是红色，重新着色
                    {
                        parent->color_ = BLACK;
                        uncle->color_ = BLACK;
                        grand->color_ = RED;

                        // 更新current位置到grand处
                        current = grand;
                        parent = current->parent_;
                    }
                } else { // 父节点是祖父的右儿子，过程与上面一致，左右方向调换
                    uncle = grand->left_;
                    // 当前结点的父结点是红色且祖父结点的另一个子结点（叔叔结点）是黑色（空节点也可以视为黑色）
                    if(uncle == nullptr || uncle->color_ == BLACK)
                    {
                        // 情况1 当前结点的父结点是红色，叔叔结点是黑色，当前结点是其父结点的左子
                        // 进行一次左旋，转换成情况3
                        if(current == parent->left_)
                        {
                            rotateWithLeftChild(parent);
                            current = parent; // current 指向旋转之后的parent节点
                            parent = current->parent_;
                        }
                        // 情况2 当前结点的父结点是红色，叔叔结点是黑色，当前结点是其父结点的右子
                        parent->color_ = BLACK;
                        grand->color_ = RED;
                        rotateWithRightChild(grand);
                    }
                    else // 情况3 叔叔节点存在且是红色，重新着色
                    {
                        parent->color_ = BLACK;
                        uncle->color_ = BLACK;
                        grand->color_ = RED;

                        // 更新current位置到grand处
                        current = grand;
                        parent = current->parent_;
                    }
                }
            }
            root_->color_ = BLACK;
        }

        TreeNode* rotateWithLeftChild(TreeNode*& k2)
        {
            TreeNode* k1 = k2->left_;
            k2->left_ = k1->right_;
            if(k1->right_)
                k1->right_->parent_ = k2;

            k1->parent_ = k2->parent_;
            if(nullptr == k2->parent_)
                root_ = k1;
            else if(k2->parent_->left_ == k2)
                k2->parent_->left_ = k1;
            else
                k2->parent_->right_ = k1;

            k1->right_ = k2;
            k2->parent_ = k1;
        }

        TreeNode* rotateWithRightChild(TreeNode*& k1)
        {
            TreeNode* k2 = k1->right_;
            k1->right_ = k2->left_;
            if(k2->left_)
                k2->left_->parent_ = k1;

            k2->parent_ = k1->parent_;
            if(nullptr == k1->parent_)
                root_ = k2;
            else if(k1->parent_->left_ == k1)
                k1->parent_->left_ = k2;
            else
                k1->parent_->right_ = k2;

            k2->left_ = k1;
            k1->parent_ = k2;
        }

        void remove(const Object& x, TreeNode*& node)
        {
            if(node == nullptr)
                return;
            if(x == node->element_)
            {
                TreeNode* remove_node = nullptr;
                TreeNode* adjust_node = nullptr;
                if(node->left_ != nullptr && node->right_ != nullptr)
                {
                    remove_node = findSuccessor(node);
                    node->element_ = remove_node->element_;
                } else {
                    remove_node = node;
                }

                if(remove_node->left_)
                    adjust_node = remove_node->left_;
                else
                    adjust_node = remove_node->right_;

                if(nullptr == remove_node->parent_)
                    root_ = adjust_node;
                else
                {
                    if(remove_node == remove_node->parent_->left_)
                        remove_node->parent_->left_ = adjust_node;
                    else
                        remove_node->parent_->right_ = adjust_node;
                }
                if(adjust_node)
                {
                    adjust_node->parent_ = remove_node->parent_;
                    if (remove_node->color_ == BLACK)
                        removeFixUp(adjust_node, adjust_node->parent_);
                }

                delete remove_node;
            } else if(x < node->element_)
            {
                remove(x, node->left_);
            } else {
                remove(x, node->right_);
            }
        }

        void removeFixUp(TreeNode*& current, TreeNode*& parent)
        {
            // 空节点也算作黑色节点
            while((current == nullptr || current->color_ == BLACK) && current != root_)
            {
                if(current == parent->left_) // 当前节点是左子节点
                {
                    TreeNode* sibling = parent->right_; // 当前节点的兄弟节点
                    if(sibling && sibling->color_ == RED) // 情况1 兄弟节点存在且是红色
                    {
                        sibling->color_ = BLACK;
                        parent->color_ = RED;
                        rotateWithRightChild(parent);
                        sibling = parent->right_;
                    }
                    // 情况2 兄弟节点是黑色的（不存在也算黑色），且兄弟的两个子节点也都是黑色的
                    if((nullptr == sibling) ||
                       ((nullptr == sibling->left_ || BLACK == sibling->left_->color_) &&
                        (nullptr == sibling->right_ || BLACK == sibling->right_->color_)))
                    {
                        sibling ? sibling->color_ = RED : 0;
                        current = parent;
                        parent = current->parent_;
                    }
                        // 情况3 兄弟节点是黑色的（不存在也算黑色），兄弟的左子节点红色，右子节点黑色
                    else {
                        if(sibling->right_ == nullptr || sibling->right_->color_ == BLACK)
                        {
                            sibling->color_ = RED;
                            if(sibling->left_)
                            {
                                sibling->left_->color_ = BLACK;
                                rotateWithLeftChild(sibling);
                            }
                            sibling = parent->right_;
                        }
                        //情况4 兄弟节点是黑色的，且兄弟的右子节点是红色，左子节点任意颜色
                        sibling->color_ = parent->color_;
                        parent->color_ = BLACK;
                        sibling->right_->color_ = BLACK;
                        rotateWithRightChild(parent);
                        current = root_;
                    }
                } else { // 与上部分对称
                    TreeNode* sibling = parent->left_; // 当前节点的兄弟节点
                    if(sibling && sibling->color_ == RED) // 情况1 兄弟节点存在且是红色
                    {
                        sibling->color_ = BLACK;
                        parent->color_ = RED;
                        rotateWithLeftChild(parent);
                        sibling = parent->left_;
                    }
                    // 情况2 兄弟节点是黑色的（不存在也算黑色），且兄弟的两个子节点也都是黑色的
                    if((nullptr == sibling) ||
                       ((nullptr == sibling->left_ || BLACK == sibling->left_->color_) &&
                        (nullptr == sibling->right_ || BLACK == sibling->right_->color_)))
                    {
                        sibling ? sibling->color_ = RED : 0;
                        current = parent;
                        parent = current->parent_;
                    }
                        // 情况3 兄弟节点是黑色的（不存在也算黑色），兄弟的右子节点红色，左子节点黑色
                    else
                    {
                        if (sibling->left_ == nullptr || sibling->left_->color_ == BLACK)
                        {
                            sibling->color_ = RED;
                            if (sibling->right_)
                            {
                                sibling->right_->color_ = BLACK;
                                rotateWithRightChild(sibling);
                            }
                            sibling = parent->left_;
                        }
                        //情况4 兄弟节点是黑色的，且兄弟的左子节点是红色，右子节点任意颜色
                        sibling->color_ = parent->color_;
                        parent->color_ = BLACK;
                        sibling->left_->color_ = BLACK;
                        rotateWithLeftChild(parent);
                        current = root_;
                    }
                }
            }
            if(current)
                current->color_ = BLACK;
        }
    public:
        RedBlackTree()
        : root_{nullptr}
        {}

        // 建立根，根为黑色
        explicit RedBlackTree(const Object& e)
        : root_{new TreeNode(e, BLACK, nullptr, nullptr, nullptr)}
        {}

        RedBlackTree(const RedBlackTree& rhs)
        : root_{nullptr}
        {
            clone(root_, nullptr, rhs.root_);
        }

        RedBlackTree(RedBlackTree&& rhs) noexcept
        : root_{rhs.root_}
        {
            rhs.root_ = nullptr;
        }

        ~RedBlackTree()
        {
            clear();
            root_ = nullptr;
        }

        RedBlackTree& operator=(const RedBlackTree& rhs)
        {
            RedBlackTree copy{rhs};
            std::swap(*this, copy);
            return *this;
        }

        RedBlackTree& operator=(RedBlackTree&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            return *this;
        }

        bool empty() const
        {
            return root_ == nullptr;
        }

        void clear()
        {
            if(empty())
                return;
            clear(root_);
            root_ = nullptr; // root重新指向null node，null node不删除
        }

        void print() const
        {
            if(empty())
                std::cout << "Empty tree" << std::endl;
            else
            {
                print(root_);
                std::cout << std::endl;
            }
        }

        const Object& findMin() const
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->left_)
                ptr = ptr->left_;
            return ptr->element_;
        }

        const Object& findMax() const
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->right_)
                ptr = ptr->right_;
            return ptr->element_;
        }

        bool contains(const Object& x) const
        {
            TreeNode* ptr = root_;
            while(ptr)
            {
                if(x < ptr->element_)
                    ptr = ptr->left_;
                else if(ptr->element_ < x)
                    ptr = ptr->right_;
                else
                    return true;
            }
            return false;
        }

        void insert(const Object& x)
        {
            if(empty())
            {
                root_ = new TreeNode(x, BLACK, nullptr, nullptr, nullptr);
                return;
            }
            insert(x, root_, nullptr);
        }

        void remove(const Object& x)
        {
            if(empty())
                return;
            remove(x, root_);
        }

        TreeNode* findSuccessor(TreeNode* node) const
        {
            TreeNode* ptr = node->right_;
            while(ptr->left_)
                ptr = ptr->left_;
            return ptr;
        }
    };
}
#endif //RB_TREE_HPP
