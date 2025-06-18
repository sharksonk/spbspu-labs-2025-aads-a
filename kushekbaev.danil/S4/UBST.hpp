#ifndef UBST_HPP
#define UBST_HPP

#include <functional>
#include "constiterator.hpp"

namespace kushekbaev
{
  template< typename Key, typename Value >
  struct UBST
  {
    UBST();
    ~UBST();

    ConstIterator< Key, Value > cbegin() const noexcept;
    ConstIterator< Key, Value > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;
    void swap(UBST& other);

    ConstIterator< Key, Value > find(const Key& key) const;
    private:
      using node_t = kushekbaev::TreeNode< Key, Value >;
      node_t* fakeroot_;
      node_t* root_;
      size_t size_;
      size_t height(node_t* node) const noexcept;
      void killChildrenOf(node_t* node);
  };

  template< typename Key, typename Value >
  UBST< Key, Value>::UBST():
    fakeroot_(reinterpret_cast< node_t* >(new char[sizeof(node_t)])),
    root_(fakeroot_),
    size_(0)
  {
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->parent = nullptr;
    fakeroot_->height = -1;
  }

  template< typename Key, typename Value >
  UBST< Key, Value >::~UBST()
  {
    clear();
    std::free(fakeroot_);
  }

  template< typename Key, typename Value >
  ConstIterator< Key, Value > UBST< Key, Value >::cbegin() const noexcept
  {
    node_t* current = root_;
    while (current->left)
    {
      current->left;
    }
    return ConstIterator< Key, Value >(current);
  }

  template< typename Key, typename Value >
  ConstIterator< Key, Value > UBST< Key, Value >::cend() const noexcept
  {
    return ConstIterator< Key, Value >(fakeroot_);
  }

  template< typename Key, typename Value >
  size_t UBST< Key, Value >::size() const noexcept
  {
    return size_;
  }

  template< typename Key, typename Value >
  bool UBST< Key, Value >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename Key, typename Value >
  void UBST< Key, Value >::clear() noexcept
  {
    killChildrenOf(root_);
    root_ = fakeroot_;
    size_ = 0;
  }

  template< typename Key, typename Value >
  void UBST< Key, Value >::killChildrenOf(node_t* node)
  {
    if (node)
    {
      --size_;
      killChildrenOf(node->left);
      killChildrenOf(node->right);
      delete node;
    }
  }

  template< typename Key, typename Value >
  void UBST< Key, Value >::swap(UBST& other)
  {
    std::swap(fakeroot_, other.fakeroot_);
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< typename Key, typename Value >
  size_t UBST< Key, Value >::height(node_t* node) const noexcept
  {
    return (node == fakeroot_ || node == nullptr) ? -1 : node->height;
  }
}

#endif