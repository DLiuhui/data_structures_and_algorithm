#ifndef KD_TREE_HPP
#define KD_TREE_HPP

// k-d tree d维搜索树
// 此处实现的是二维搜索树
// 如果在建树时采用中值优化（保证根值是分割维度集合的中值）会建立一棵更加平衡的树

#include <vector>
#include "../lib/dsexceptions.h"

namespace DS
{
    template <typename Object>
    class KdTree
    {
    private:
        class TreeNode
        {
        public:
            std::vector<Object> data_;
            TreeNode* left_;
            TreeNode* right_;

            explicit TreeNode(const std::vector<Object>& item)
            : data_{item}, left_{nullptr}, right_{nullptr}
            {}
        };

        TreeNode* root_;

        void clear(TreeNode*& root)
        {
            if(root)
            {
                TreeNode* lp = root->left_;
                TreeNode* rp = root->right_;
                delete root;
                root = nullptr;
                clear(lp);
                clear(rp);
            }
        }

        void clone(TreeNode*& node, TreeNode* rhs)
        {
            if(rhs)
            {
                node = new TreeNode{rhs->data_};
                clone(node->left_, rhs->left_);
                clone(node->right_, rhs->right_);
            }
        }

        void printRange(const std::vector<Object>& low, const std::vector<Object>& high,
                TreeNode* node, int level) const
        {
            if(node)
            {
                if(low[0] <= node->data_[0] && high[0] >= node->data_[0] && \
                low[1] <= node->data_[1] && high[1] >= node->data_[1])
                    std::cout << "(" << node->data_[0] << "," << node->data_[1] << ")" << std::endl;
                if(low[level] <= node->data_[level])
                    printRange(low, high, node->left_, 1 - level);
                if(high[level] >= node->data_[level])
                    printRange(low, high, node->right_, 1 - level);
            }
        }

        void insert(const std::vector<Object>& x, TreeNode*& node, int level)
        {
            if(node == nullptr)
                node = new TreeNode(x);
            else if(x[level] > node->data_[level])
                insert(x, node->right_, 1 - level);
            else if(x[level] < node->data_[level] || x[1 - level] != node->data_[1 - level])
                insert(x, node->left_, 1 - level);
        }

        bool contain(const std::vector<Object>& item, TreeNode* node, int level)
        {
            if(item == nullptr)
                return false;
            else if(item[level] > node->data_[level])
                return contain(item, node->right_, 1 - level);
            else if(item[level] < node->data_[level] || item[1 - level] != node->data_[1 - level])
                contain(item, node->left_, 1 - level);
            else
                return true;
        }

    public:
        KdTree()
        : root_(nullptr)
        {}

        KdTree(const KdTree& rhs)
        : root_(nullptr)
        {
            clone(root_, rhs.root_);
        }

        KdTree(KdTree&& rhs) noexcept
        : root_{rhs.root_}
        {
            rhs.root_ = nullptr;
        }

        ~KdTree()
        {
            clear();
        }

        KdTree& operator=(const KdTree& rhs)
        {
            KdTree copy{rhs};
            std::swap(*this, copy);
            return *this;
        }

        KdTree& operator=(KdTree&& rhs) noexcept
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
            if(!empty())
                clear(root_);
        }

        // 打印low~high范围内的点
        void printRange(const std::vector<Object>& low, const std::vector<Object>& high) const
        {
            printRange(low, high, root_, 0);
        }

        void insert(const std::vector<Object>& x)
        {
            insert(x, root_, 0);
        }

        bool contain(const std::vector<Object>& item) const
        {
            if(empty())
                return false;
            return contain(item, root_, 0);
        }
    };
}

#endif //KD_TREE_HPP
