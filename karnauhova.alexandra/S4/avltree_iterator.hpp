#ifndef BITREE_ITERATOR_HPP
#define BITREE_ITERATOR_HPP
#include "avltree_node.hpp"
#include <iostream>
#include <cstddef>
#include <stdexcept>
namespace karnauhova
{
  template<typename Key, typename Value, typename Compare >
  class AvlTree;

  template< typename Key, typename Value, typename Compare = std::less< Key >>
  struct AvlTreeIterator
  {
    friend class AvlTree< Key, Value, Compare >;
  public:
    using this_t = AvlTreeIterator< Key, Value, Compare>;
    using Node = AvlTreeNode< Key, Value >;
    AvlTreeIterator() noexcept;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const noexcept;

    bool operator==(const this_t& rhs) const noexcept;
    bool operator!=(const this_t& rhs) const noexcept;;
  private:
    Node* node_;
    explicit AvlTreeIterator(const Node* node) noexcept;
  };

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >::AvlTreeIterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >::AvlTreeIterator(const Node* node) noexcept:
    node_(node)
  {}

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >&  AvlTreeIterator< Key, Value, Compare >::operator++() noexcept
  {
    if (node_->right)
    {
      Node* current = node_->right;
      while (current->left)
      {
        current = current->left;
      }
      this_t it{current};
      return it;
    }
    Node* parent = node_->parent;
    Node* last = node_;
    while (parent->left != last && parent->parent)
    {
      last = last->parent;
      parent = parent->parent;
    }
    this_t it{parent};
    return it;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >  AvlTreeIterator< Key, Value, Compare >::operator++(int) noexcept
  {
    this_t res(*this);
    ++(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >&  AvlTreeIterator< Key, Value, Compare >::operator--() noexcept
  {
    if (node_->left)
    {
      Node* current = node_->left;
      while (current->right)
      {
        current = current->right;
      }
      this_t it{current};
      return it;
      }
      Node* parent = node_->parent;
      Node* last = node_;
      while (parent->right != last && parent->parent)
      {
        last = last->parent;
        parent = parent->parent;
      }
    this_t it{parent};
    return it;
  }

  template< typename Key, typename Value, typename Compare >
  AvlTreeIterator< Key, Value, Compare >  AvlTreeIterator< Key, Value, Compare >::operator--(int) noexcept
  {
    this_t res(*this);
    --(*this);
    return res;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >& AvlTreeIterator< Key, Value, Compare >::operator*() const
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Compare >
  std::pair< Key, Value >* AvlTreeIterator< Key, Value, Compare >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeIterator< Key, Value, Compare >::operator==(const this_t& rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< typename Key, typename Value, typename Compare >
  bool AvlTreeIterator< Key, Value, Compare >::operator!=(const this_t& rhs) const noexcept
  {
    return !(*this == rhs);
  }
}
#endif
