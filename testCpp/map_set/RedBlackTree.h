#pragma once

#include <iostream>
#include <assert.h>

using namespace std;

enum Colour
{
    RED,
    BLACK
};

template<class T>
struct RBTreeNode
{
    RBTreeNode<T>* _left;
    RBTreeNode<T>* _right;
    RBTreeNode<T>* _parent;

    T _data;
    Colour _col;

    RBTreeNode(const T& data)
        :_left(nullptr)
         ,_right(nullptr)
         ,_parent(nullptr)
         ,_data(data)
    {}
};

template<class T, class Ref, class Ptr>
struct __RBTreeIterator
{
    typedef RBTreeNode<T> Node;
    typedef __RBTreeIterator<T, Ref, Ptr> Self;
    Node* _node;

    __RBTreeIterator(Node* node)
        :_node(node)
    {}

    Ref operator*()
    {
        return _node->_data;
    }

    Ptr operator->()
    {
        return &_node->_data;
    }

    bool operator!=(const Self& s) const 
    {
        return _node != s._node;
    }

    bool operator==(const Self& s) const 
    {
        return _node == s._node;
    }

    Self& operator++()  // 中序遍历
    {
        if (_node->_right)
        {
            // 下一个就是右子树的最左节点
            Node* left = _node->_right;
            while (left->_left)
            {
                left = left->_left;
            }
            _node = left;
        }
        else 
        {
            // 找祖先里面，孩子不是祖先的右子树那个
            Node* parent = _node->_parent;
            Node* cur = _node;
            while (parent && cur == parent->_right)
            {
                cur = cur->_parent;
                parent = cur->_parent;
            }

            _node = parent;
        }

        return *this;
    }

    Self& operator--()
    {
        return *this;
    }
};

template<class K, class T, class KeyOfT>
struct RBTree
{
    typedef RBTreeNode<T> Node;
public:
    typedef __RBTreeIterator<T, T&, T*> iterator;

    iterator begin()
    {
        Node* left = _root;
        while (left && left->_left)
        {
            left = left->_left;
        }

        return iterator(left);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    bool Insert(const T& data)
    {
        KeyOfT kot;

        if (_root == nullptr)
        {
            _root = new Node(data);
            _root->_col = BLACK;
            return true;
        }
        
        Node* parent = nullptr;
        Node* cur = _root;

        while (cur)
        {
            if (kot(cur->_data) < kot(data))
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (kot(cur->_data) > kot(data))
            {
                parent = cur;
                cur = cur->_left;
            }
            else 
            {
                return false;
            }
        }

        // 走到空，新建+链接
        cur = new Node(data);
        cur->_col = RED;

        if (kot(parent->_data) < kot(data))
        {
            parent->_right = cur;
        }
        else 
        {
            parent->_left = cur;
        }
        cur->_parent = parent;

        // 更新树平衡
        while (parent && parent->_col == RED)
        {
            Node* grandfater = parent->_parent;
            assert (grandfater);
            assert (grandfater->_col == BLACK);

            // 关键看叔叔
            if (parent == grandfater->_left)
            {
                Node* uncle = grandfater->_right;
                // 情况一：uncle存在且为红，变色+继续往上处理
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = uncle->_col = BLACK;
                    grandfater->_col = RED;
                    // 继续往上处理
                    cur = grandfater;
                    parent = cur->_parent;
                }
                // 情况二+三：uncle不存在 + 存在且为黑
                else 
                {
                    // 情况二：右单旋+变色
                    //    g
                    //  p   u
                    //c 
                    if (cur == parent->_left)
                    {
                        RotateR(grandfater);
                        parent->_col = BLACK;
                        grandfater->_col = RED;
                    }
                    else 
                    {
                        // 情况三：左右单旋+变色 
                        //    g
                        //  p   u
                        //   c
                        RotateL(parent);
                        RotateR(grandfater);
                        cur->_col = BLACK;
                        grandfater->_col = RED;
                    }

                    break;
                }
            }
            else  // (parent == grandfater->_right)
            {
                Node* uncle = grandfater->_left;
                // 情况一
                if (uncle && uncle->_col == RED)
                {
                    parent->_col = uncle->_col = BLACK; 
                    grandfater->_col = RED;
                    // 继续往上处理
                    cur = grandfater;
                    parent = cur->_parent;
                }
                else 
                {
                    // 情况二：左单旋+变色
                    //    g
                    //  u   p
                    //       c
                    if (cur == parent->_right)
                    {
                        RotateL(grandfater);
                        parent->_col = BLACK;
                        grandfater->_col = RED;
                    }
                    else 
                    {
                        // 情况三：右左单旋+变色
                        //    g
                        //  u   p
                        //     c
                        RotateR(parent);
                        RotateL(grandfater);
                        cur->_col = BLACK;
                        grandfater->_col = RED;
                    }

                    break;
                }
            }
        }

        _root->_col = BLACK;
        return true;
    }

    void InOrder()
    {
        _InOrder(_root);
        cout << endl;
    }

    bool IsBalance()
    {
        if (_root == nullptr)
        {
            return true;
        }

        if (_root->_col == RED)
        {
            cout << "根节点不是黑色" << endl;
            return false;
        }

        // 黑色节点数量基准值
        int benchmark = 0;
        // Node* cur = _root;
        // while (cur)
        // {
        //     if (cur->_col == BLACK;)
        //     ++benchmark;
        //     cur = cur->_left;
        // }
        
        return PrevCheck(_root, 0, benchmark);
    }

private:
bool PrevCheck(Node* root, int blackNum, int& benchmark)
{
    if (root == nullptr)
    {
        if (benchmark == 0)
        {
            benchmark = blackNum;
            return true;
        }

        if (blackNum != benchmark)
        {
            cout << "某条线路黑色节点的数量不相等" << endl;
            return false;
        }
        else 
        {
            return true;
        }
    }

    if (root->_col == BLACK)
    {
        ++blackNum;
    }
    
    if (root->_col == RED && root->_parent->_col == RED)
    {
        cout << "存在连续的红色节点" << endl;
        return false;
    }

    return PrevCheck(root->_left, blackNum, benchmark)
        && PrevCheck(root->_right, blackNum, benchmark);
}

void _InOrder(Node* root)
{
    if (root == nullptr)
    {
        return;
    }
    _InOrder(root->_left);
    cout << root->_kv.first << ":" << root->_kv.second << endl;
    _InOrder(root->_right);
}

void RotateL(Node* parent)
{
    Node* subR = parent->_right;
    Node* subRL = subR->_left;

    parent->_right = subRL;
    if (subRL)
    {
        subRL->_parent = parent;
    }

    Node* ppNode = parent->_parent;


    subR->_left = parent;
    parent->_parent = subR;

    if (_root == parent)
    {
        _root = subR;
        subR->_parent = nullptr;
    }
    else 
    {
        if (ppNode->_left == parent) 
        {
            ppNode->_left = subR;
        }
        else 
        {
            ppNode->_right = subR;
        }
        
        subR->_parent = ppNode;
    }

}

void RotateR(Node* parent)
{
    Node* subL = parent->_left;
    Node* subLR = subL->_right;

    parent->_left = subLR;
    if (subLR)
    {
        subLR->_parent = parent;

    }

    Node* ppNode = parent->_parent;
    
    
    subL->_right = parent;
    parent->_parent = subL;

    if (_root == parent)
    {
        _root = subL;
        subL->_parent = nullptr;

    }
    else 
    {
        if (ppNode->_left == parent)
        {
            ppNode->_left = subL;
        }
        else 
        {
            ppNode->_right = subL;
        }
    
        subL->_parent = ppNode;

    }
}

private:
        Node* _root = nullptr;
};





