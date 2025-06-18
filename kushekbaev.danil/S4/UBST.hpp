#ifndef UBST_HPP
#define UBST_HPP

#include <functional>
#include "constiterator.hpp"
#include "iterator.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value, typename Cmp = std::less< Key > >
  struct UBST
  {
    UBST();
    ~UBST();

    Iterator< Key, Value, Cmp > begin() noexcept;
    Iterator< Key, Value, Cmp > end() noexcept;

    ConstIterator< Key, Value, Cmp > cbegin() const noexcept;
    ConstIterator< Key, Value, Cmp > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;
    void swap(UBST& other);

    Iterator< Key, Value, Cmp > find(const Key& key) noexcept;

    ConstIterator< Key, Value, Cmp > find(const Key& key) const noexcept;

    private:
      using node_t = kushekbaev::TreeNode< Key, Value, Cmp >;
      node_t* fakeroot_;
      node_t* root_;
      size_t size_;
      Cmp cmp_;
      size_t height(node_t* node) const noexcept;
      void killChildrenOf(node_t* node);
  };

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp>::UBST():
    fakeroot_(reinterpret_cast< node_t* >(new char[sizeof(node_t)])),
    root_(fakeroot_),
    size_(0)
  {
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->parent = nullptr;
    fakeroot_->height = -1;
  }

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp >::~UBST()
  {
    clear();
    std::free(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::begin() noexcept
  {
    if (empty())
    {
      return end();
    }
    node_t* current = root_;
    while (current->left)
    {
      current->left;
    }
    return Iterator< Key, Value, Cmp >(current);
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::end() noexcept
  {
    return Iterator< Key, Value, Cmp >(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::cbegin() const noexcept
  {
    if (empty())
    {
      return cend();
    }
    node_t* current = root_;
    while (current->left)
    {
      current->left;
    }
    return ConstIterator< Key, Value, Cmp >(current);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::cend() const noexcept
  {
    return ConstIterator< Key, Value, Cmp >(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  size_t UBST< Key, Value, Cmp >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value, typename Cmp >
  bool UBST< Key, Value, Cmp >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value, typename Cmp >
  void UBST< Key, Value, Cmp >::clear() noexcept
  {
    killChildrenOf(root_);
    root_ = fakeroot_;
    size_ = 0;
  }

  template< typename Key, typename Value, typename Cmp >
  void UBST< Key, Value, Cmp >::killChildrenOf(node_t* node)
  {
    if (node)
    {
      --size_;
      killChildrenOf(node->left);
      killChildrenOf(node->right);
      delete node;
    }
  }

  template< typename Key, typename Value, typename Cmp >
  void UBST< Key, Value, Cmp >::swap(UBST& other)
  {
    std::swap(fakeroot_, other.fakeroot_);
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< typename Key, typename Value, typename Cmp >
  size_t UBST< Key, Value, Cmp >::height(node_t* node) const noexcept
  {
    return (node == fakeroot_ || node == nullptr) ? -1 : node->height;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::find(const Key& key) noexcept
  {
    node_t* current = root_;
    while (current && current != fakeroot_)
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
        return Iterator< Key, Value, Cmp >(current);
      }
    }
    return end();
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::find(const Key& key) const noexcept
  {
    node_t* current = root_;
    while (current && current != fakeroot_)
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
        return ConstIterator< Key, Value, Cmp >(current);
      }
    }
    return cend();
  }
}

#endif
