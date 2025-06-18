#ifndef CONSTITERATOR_HPP
#define CONSTITERATOR_HPP

#include <iterator>
#include "treenode.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value, typename Cmp >
  struct UBST;

  template< typename Key, typename Value, typename Cmp >
  struct ConstIterator
  {
    using this_t = ConstIterator< Key, Value, Cmp >;

    ConstIterator() noexcept;
    ConstIterator(const this_t&) = default;
    ~ConstIterator() = default;

    this_t& operator=(const this_t& other) = default;
    this_t& operator++() noexcept;
    this_t operator++(int) noexcept;
    this_t& operator--() noexcept;
    this_t operator--(int) noexcept;

    const std::pair< Key, Value >& operator*() const noexcept;
    const std::pair< Key, Value >& operator->() const noexcept;
    bool operator!=(const this_t& other) const noexcept;
    bool operator==(const this_t& other) const noexcept;

    private:
      friend struct UBST< Key, Value, Cmp >;
      TreeNode< Key, Value, Cmp >* node_;
      explicit ConstIterator(TreeNode< Key, Value, Cmp >* node) noexcept;
  };

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp >::ConstIterator() noexcept:
    node_(nullptr)
  {}

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp >::ConstIterator(TreeNode< Key, Value, Cmp >* node) noexcept:
    node_(node)
  {}

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp >& ConstIterator< Key, Value, Cmp >::operator++() noexcept
  {
    if (node_->right)
    {
      node_ = node_->right;
      while (node_->left)
      {
        node_ = node_->left;
      }
    }
    else
    {
      while (node_->parent && node_->parent->right == node_ && node_->parent->height != -1)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > ConstIterator< Key, Value, Cmp >::operator++(int) noexcept
  {
    this_t tmp(*this);
    ++(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp >& ConstIterator< Key, Value, Cmp >::operator--() noexcept
  {
    if (node_->left)
    {
      node_ = node_->left;
      while (node_->right)
      {
        node_ = node_->right;
      }
    }
    else
    {
      while (node_->parent && node_->parent->left == node_ && node_->parent->height != -1)
      {
        node_ = node_->parent;
      }
      node_ = node_->parent;
    }
    return (*this);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > ConstIterator< Key, Value, Cmp >::operator--(int) noexcept
  {
    this_t tmp(*this);
    --(*this);
    return tmp;
  }

  template< typename Key, typename Value, typename Cmp >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Cmp >::operator*() const noexcept
  {
    return node_->data;
  }

  template< typename Key, typename Value, typename Cmp >
  const std::pair< Key, Value >& ConstIterator< Key, Value, Cmp >::operator->() const noexcept
  {
    return std::addressof(node_->data);
  }

  template< typename Key, typename Value, typename Cmp >
  bool ConstIterator< Key, Value, Cmp >::operator!=(const this_t& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< typename Key, typename Value, typename Cmp >
  bool ConstIterator< Key, Value, Cmp >::operator==(const this_t& other) const noexcept
  {
    return !(*this == other);
  }
}

#endif