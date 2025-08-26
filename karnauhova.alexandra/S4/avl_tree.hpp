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

    void clear() noexcept;
    void swap(AvlTree< Key, Value, Compare >& oth) noexcept;

    Iter find(const Key& key) noexcept;
    CIter find(const Key& key) const noexcept;

  private:
    Node* fake_;
    size_t size_;
    Compare comp_;

    void clearRecursive(Node* node) noexcept;
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
    
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare > AvlTree< Key, Value, Compare >::erase(CIter) noexcept
  {
    
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
    std::swap(root_, oth.root_);
    std::swap(fake_, oth.fake_);
    std::swap(size_, oth.size_);
    std::swap(comp_, oth.comp_);
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
}

#endif
