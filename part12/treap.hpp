//
// Created by DDRHb on 2019/11/5.
//

#ifndef TREAP_HPP
#define TREAP_HPP

#include <iostream>
#include <climits>
#include "../lib/uniform_random.h"
#include "../lib/dsexceptions.h"

// Treap class
// 树堆
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
    class Treap
    {
    public:
        Treap()
        {
            null_node_ = new TreeNode{};
            null_node_->left_ = null_node_->right_ = null_node_;
            null_node_->priority_ = INT32_MAX;
            root_ = null_node_;
        }

        Treap(const Treap& rhs)
        {
            null_node_ = new TreeNode{};
            null_node_->left_ = null_node_->right_ = null_node_;
            null_node_->priority_ = INT32_MAX;
            clone(root_, rhs.root_);
        }

        Treap(Treap&& rhs) noexcept
        : root_{rhs.root_}, null_node_{rhs.null_node_}
        {
            rhs.root_ = nullptr;
            rhs.null_node_ = nullptr;
        }

        ~Treap()
        {
            clear();
            delete null_node_;
        }

        Treap& operator=(const Treap& rhs)
        {
            Treap copy{rhs};
            std::swap(*this, copy);
            return *this;
        }

        Treap& operator=(Treap&& rhs) noexcept
        {
            std::swap(root_, rhs.root_);
            std::swap(null_node_, rhs.null_node_);
            return *this;
        }

        bool empty() const
        {
            return root_ == null_node_;
        }

        const Object& findMin() const
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->left_ != null_node_)
                ptr = ptr->left_;
            return ptr->element_;
        }

        const Object& findMax() const
        {
            if(empty())
                throw UnderflowException{};
            TreeNode* ptr = root_;
            while(ptr->right_ != null_node_)
                ptr = ptr->right_;
            return ptr->element_;
        }

        bool contains(const Object& x) const
        {
            TreeNode* current = root_;
            null_node_->element_ = x;
            while(true)
            {
                if(x < current->element_)
                    current = current->left_;
                else if(x > current->element_)
                    current = current->right_;
                else
                    return current != null_node_;
            }
        }

        void clear()
        {
            clear(root_);
        }

        void print() const
        {
            if(empty())
                std::cout << "Empty tree" << std::endl;
            else
                print(root_);
        }

        void insert(const Object& x)
        {
            insert(x, root_);
        }

        void insert(Object&& x)
        {
            insert(std::move(x), root_);
        }

        void remove(const Object& x)
        {
            remove(x, root_);
        }


    private:
        class TreeNode
        {
        public:
            Object element_;
            TreeNode* left_;
            TreeNode* right_;
            int priority_; // 优先级

            TreeNode()
            : element_{Object()}, left_{nullptr}, right_{nullptr}, priority_{INT32_MAX}
            {}

            TreeNode(const Object& e, int pr, TreeNode* lt = nullptr, TreeNode* rt = nullptr)
            : element_{e}, left_{lt}, right_{rt}, priority_{pr}
            {}

            TreeNode(Object&& e, int pr, TreeNode* lt = nullptr, TreeNode* rt = nullptr)
            : element_{std::move(e)}, left_{lt}, right_{rt}, priority_{pr}
            {}
        };

        TreeNode* root_;
        TreeNode* null_node_;
        UniformRandom random_;

        void clone(TreeNode*& root, TreeNode* node)
        {
            if(node == node->left_)
                root = null_node_;
            else
            {
                root = new TreeNode(node->element_, node->priority_);
                clone(root->left_, node->left_);
                clone(root->right_, node->right_);
            }
        }

        void clear(TreeNode*& root)
        {
            if(root != null_node_)
            {
                TreeNode* lt = root->left_;
                TreeNode* rt = root->right_;
                delete root;
                root = null_node_;
                clear(lt);
                clear(rt);
            }
        }

        void print(TreeNode* node) const
        {
            if(node != null_node_)
            {
                print(node->left_);
                std::cout << node->element_ << std::endl;
                print(node->right_);
            }
        }

        void insert(const Object& x, TreeNode*& node)
        {
            // 以随机数作为优先级分配
            if(node == null_node_)
                node = new TreeNode(x, random_.nextInt(), null_node_, null_node_);
            else if(x < node->element_)
            {
                insert(x, node->left_);
                // 优先级调整
                if(node->left_->priority_ < node->priority_)
                    rotateWithLeftChild(node);
            } else if (x > node->element_)
            {
                insert(x, node->right_);
                // 优先级调整
                if(node->right_->priority_ < node->priority_)
                    rotateWithRightChild(node);
            }
            // else duplicate: do nothing
        }

        void insert(Object&& x, TreeNode*& node)
        {
            // 以随机数作为优先级分配
            if(node == null_node_)
                node = new TreeNode(std::move(x), random_.nextInt(), null_node_, null_node_);
            else if(x < node->element_)
            {
                insert(std::move(x), node->left_);
                // 优先级调整
                if(node->left_->priority_ < node->priority_)
                    rotateWithLeftChild(node);
            } else if (x > node->element_)
            {
                insert(std::move(x), node->right_);
                // 优先级调整
                if(node->right_->priority_ < node->priority_)
                    rotateWithRightChild(node);
            }
            // else duplicate: do nothing
        }

        void remove(const Object& x, TreeNode*& node)
        {
            if(node != null_node_)
            {
                if(x < node->element_)
                    remove(x, node->left_);
                else if(x > node->element_)
                    remove(x, node->right_);
                else // match
                {
                    if(node->left_->priority_ < node->right_->priority_)
                        rotateWithLeftChild(node);
                    else
                        rotateWithRightChild(node);
                    if(node != null_node_) // 不是叶子节点
                        remove(x, node);
                    else // 是叶子节点
                    {
                        delete node->left_;
                        node->left_ = null_node_;
                    }
                }
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
            TreeNode *k2 = k1->right_;
            k1->right_ = k2->left_;
            k2->left_ = k1;
            k1 = k2;
        }
    };
}
#endif //TREAP_HPP
