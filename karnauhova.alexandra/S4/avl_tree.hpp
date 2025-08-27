#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "avltree_node.hpp"
#include "avltree_iterator.hpp"
#include "avltree_citerator.hpp"
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare = std::less< Key >>
  class AvlTree
  {
  public:
    using Node = AvlTreeNode< Key, Value >;
    using Iter = AvlTreeIterator< Key, Value, Compare >;
    using CIter = AvlTreeCIterator< Key, Value, Compare >;

    AvlTree();
    AvlTree(const AvlTree< Key, Value, Compare >& oth);
    AvlTree(AvlTree< Key, Value, Compare >&& oth) noexcept;
    ~AvlTree();

    std::pair< Iter, bool > insert(const std::pair< Key, Value >& val);

    Iter erase(Iter) noexcept;
    Iter erase(CIter) noexcept;
    
    Iter begin() const noexcept;
    Iter end() const noexcept;
    CIter cbegin() const noexcept;
    CIter cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t count(const Key& key) const;

    void clear() noexcept;
    void swap(AvlTree< Key, Value, Compare >& oth) noexcept;

    Iter find(const Key& key) noexcept;
    CIter find(const Key& key) const noexcept;

  private:
    Node* fake_;
    size_t size_;
    Compare comp_;

    void clearRecursive(Node*) noexcept;
    void balance(Node*) noexcept;
    Node* balanceNode(Node*) noexcept;
    int bfactor(Node*) const noexcept;
    Node* rotateRight(Node*) noexcept;
    Node* rotateLeft(Node*) noexcept;
    int height(Node*) const noexcept;
    void updateHeight(Node*) noexcept;
  };

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::AvlTree():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
    size_(0),
    comp_()
  {
    fake_->left = fake_->right = fake_;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTree< Key, Value, Compare >::~AvlTree()
  {
    clear();
    delete[] reinterpret_cast< char* >(fake_);
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< AvlTreeIterator< Key, Value, Compare >, bool > AvlTree< Key, Value, Compare >::insert(const std::pair< Key, Value >& val)
  {
    Node* newNode = nullptr;
    try
    {
      newNode = new Node{val, fake_, fake_, nullptr, 0};
      if (empty())
      {
        fake_->left = newNode;
        newNode->parent = newNode->left = newNode->right = fake_;
        size_ = 1;
        return {Iter(newNode), true};
      }
      Node* tmp = fake_->left;
      Node* parent = fake_;
      while (tmp != fake_)
      {
        parent = tmp;
        if (comp_(newNode->data1.first, tmp->data1.first))
        {
          tmp = tmp->left;
        }
        else if (comp_(tmp->data1.first, newNode->data1.first))
        {
          tmp = tmp->right;
        }
        else
        {
          delete newNode;
          return {Iter(tmp), false};
        }
      }
      newNode->parent = parent;
      if (cmp_(parent->data1.first, newNode->data1.first))
      {
        parent->right = newNode;
      }
      else
      {
        parent->left = newNode;
      }
      balance(fake_->left);
      size_++;
      return {Iter(newNode), true};
    }
    catch (const std::exception& e)
    {
      throw;
    }
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare > AvlTree< Key, Value, Compare >::erase(CIter it) noexcept
  {
    if (it == cend() || it.node_ == fake_)
    {
      return end();
    }
    Node* delet = it.node_;
    Iter res(delet);
    ++res;
    Node* new_node = nullptr;

    if (delet->left == fake_ && delet->right == fake_)
    {
      new_node = delet->parent;
      if (new_node != fake_)
      {
        if (new_node->left == delet)
        {
          new_node->left = fake_;
        }
        else
        {
          new_node->right = fake_;
        }
      }
      else
      {
        fake_->left = fake_;
      }
      delete delet;
      size_--;
    }
    else if (delet->left == fake_ || delet->right == fake_)
    {
      Node* child = delet->left != fake_ ? delet->left : delet->right;
      new_node = delet->parent;
      if (new_node != fake_)
      {
        if (new_node->left == delet)
        {
          new_node->left = child;
        }
        else
        {
          new_node->right = child;
        }
      }
      else
      {
        fake_->left = child;
      }
      child->parent = new_node;
      delete delet;
      size_--;
    }
    else
    {
      Node* tmp = delet->right;
      while (tmp->left != fake_)
      {
        tmp = tmp->left;
      }
      std::swap(delet->data1, tmp->data1);
      new_node = tmp->parent;
      if (new_node->left == tmp)
      {
        new_node->left = tmp->right;
      }
      else
      {
        new_node->right = tmp->right;
      }
      if (tmp->right != fake_)
      {
        tmp->right->parent = new_node;
      }
      delet tmp;
      size_--;
    }
    if (new_node != fake_)
    {
      balance(new_node);
    }
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::end() const noexcept
  {
    return Iter(fake_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::cend() const noexcept
  {
    return CIter(fake_);
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare > AvlTree< Key, Value, Compare >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    Node* tmp = fake_->left;
    while (tmp->left != fake_)
    {
      tmp = tmp->left;
    }
    return Iter(tmp);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    Node* tmp = fake_->left;
    while (tmp->left != fake_)
    {
      tmp = tmp->left;
    }
    return CIter(tmp);
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Compare >
  size_t AvlTree< Key, Value, Compare >::count(const Key& key) const
  {
    return find(key) != cend();
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::clear() noexcept
  {
    if (empty())
    {
      return;
    }
    clearRecursive(fake_->left);
    fake_->left = fake_;
    fake_->right = fake_;
    size_ = 0;
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::swap(AvlTree< Key, Value, Compare >& oth) noexcept
  {
    std::swap(fake_, oth.fake_);
    std::swap(size_, oth.size_);
    std::swap(comp_, oth.comp_);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Iter AvlTree< Key, Value, Compare >::find(const Key& key) noexcept
  {
    Node* current = fake_->left;
    while (current != fake_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return Iter(current);
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::CIter AvlTree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    Node* current = fake_->left;
    while (current != fake_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        return CIter(current);
      }
    }
    return cend();
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::clearRecursive(Node* node) noexcept
  {
    if (node == fake_)
    {
      return;
    }
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
  }

  template< typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::bfactor(Node* node) const noexcept
  {
    if (node == fake_ || node == nullptr)
    {
      return 0;
    }
    return height(node->left) - height(node->right);
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::balanceNode(Node* node) noexcept
  {
    updateHeight(node);
    int factor = bfactor(node);
    if (factor > 1)
    {
      if (bfactor(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }
    if (factor < -1)
    {
      if (bfactor(node->right) > 0)
      {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }
    return node;
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::balance(Node* node) noexcept
  {
    while (node != fake_)
    {
      Node* parent = node->parent;
      bool m = (node == parent->left);
      node = balance(node);
      if (m)
      {
        parent->left = node;
      }
      else
      {
        parent->right = node;
      }
      node = parent;
    }
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::rotateRight(Node* node) noexcept
  {
    if (!node || node->left == fake_ || node == fake_)
    {
      return node;
    }
    Node* it = node->left;
    node->left = it->right;
    if (it->right != fake_)
    {
      it->right->parent = node;
    }

    it->parent = node->parent;
    node->parent = it;
    it->right = node;
    if (it->parent != fake_)
    {
      if (it->parent->left == node)
      {
        it->parent->left = it;
      }
      else
      {
        it->parent->right = it;
      }
    }
    else
    {
      fake_->left = it;
    }

    updateHeight(node);
    updateHeight(it);

    return it;
  }

  template< typename Key, typename Value, typename Compare >
  typename AvlTree< Key, Value, Compare >::Node* AvlTree< Key, Value, Compare >::rotateLeft(Node* node) noexcept
  {
    if (!node || node->left == fake_ || node == fake_)
    {
      return node;
    }
    Node* it = node->right;
    node->right = it->left;
    if (it->left != fake_)
    {
      it->left->parent = node;
    }

    it->parent = node->parent;
    node->parent = it;
    it->left = node;
    if (it->parent != fake_)
    {
      if (it->parent->left == node)
      {
        it->parent->left = it;
      }
      else
      {
        it->parent->right = it;
      }
    }
    else
    {
      fake_->left = it;
    }

    updateHeight(node);
    updateHeight(it);

    return it;
  }

  template< typename Key, typename Value, typename Compare >
  int AvlTree< Key, Value, Compare >::height(Node* node) const noexcept
  {
    if (node == fake_ || node == nullptr)
    {
      return -1;
    }
    return node->height;
  }

  template< typename Key, typename Value, typename Compare >
  void AvlTree< Key, Value, Compare >::updateHeight(Node* node) noexcept
  {
    if (node != fake_ && node != nullptr)
    {
      node->height = std::max(height(node->left), height(node->right)) + 1;
    }
  }
}

#endif
