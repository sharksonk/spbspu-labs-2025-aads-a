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
    template< typename InputIterator >
    using enableIf = std::enable_if_t< std::is_convertible< decltype(*std::declval< InputIterator >()), Value >::value >;

    UBST();
    UBST(const UBST< Key, Value, Cmp >& other);
    UBST(UBST< Key, Value, Cmp >&& other);
    ~UBST();

    UBST<Key, Value, Cmp >& operator=(const UBST< Key, Value, Cmp >& other);
    UBST<Key, Value, Cmp >& operator=(UBST< Key, Value, Cmp >&& other);

    Iterator< Key, Value, Cmp > begin() noexcept;
    Iterator< Key, Value, Cmp > end() noexcept;
    ConstIterator< Key, Value, Cmp > cbegin() const noexcept;
    ConstIterator< Key, Value, Cmp > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    void clear() noexcept;
    void swap(UBST& other);
    std::pair< Iterator< Key, Value, Cmp >, bool > insert(const std::pair< Key, Value >& value);
    std::pair< Iterator< Key, Value, Cmp >, bool > insert(std::pair< Key, Value >&& value);
    template< typename InputIterator >
    void insert(InputIterator first, InputIterator last);
    template< typename... Args >
    std::pair< Iterator< Key, Value, Cmp >, bool > emplace(Args&&... args);

    Iterator< Key, Value, Cmp > find(const Key& key) noexcept;
    ConstIterator< Key, Value, Cmp > find(const Key& key) const noexcept;
    size_t count(const Key& key) const noexcept;
    std::pair< Iterator< Key, Value, Cmp >, Iterator< Key, Value, Cmp > > equal_range(const Key& key);
    std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > > equal_range(const Key& key) const;

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
  UBST< Key, Value, Cmp>::UBST(const UBST< Key, Value, Cmp >& other):
    fakeroot_(other.fakeroot_),
    root_(other.root_),
    size_(other.size_)
  {
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->height = -1;
    for (auto it = other.begin(); it != other.end(); ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp>::UBST(UBST< Key, Value, Cmp >&& other):
    fakeroot_(std::move(other.fakeroot)),
    root_(std::move(other.root_)),
    size_(std::move(other.size_)),
    cmp_(std::move(other.cmp))
  {}

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp >::~UBST()
  {
    clear();
    std::free(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  UBST<Key, Value, Cmp >& UBST< Key, Value, Cmp >::operator=(const UBST< Key, Value, Cmp >& other)
  {
    UBST< Key, Value, Cmp > tmp(other);
    swap(tmp);
    return *this;
  }

  template< typename Key, typename Value, typename Cmp >
  UBST<Key, Value, Cmp >& UBST< Key, Value, Cmp >::operator=(UBST< Key, Value, Cmp >&& other)
  {
    UBST< Key, Value, Cmp > tmp(other);
    swap(std::move(tmp));
    return *this;
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
  Value& UBST< Key, Value, Cmp >::at(const Key& key)
  {
    auto current = find(key);
    if (current == end())
    {
      throw std::out_of_range("Key hasn't been found!");
    }
    return current->second;
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& UBST< Key, Value, Cmp >::at(const Key& key) const
  {
    auto current = find(key);
    if (current == cend())
    {
      throw std::out_of_range("Key hasn't been found!");
    }
    return current->second;
  }

  template< typename Key, typename Value, typename Cmp >
  Value& UBST< Key, Value, Cmp >::operator[](const Key& key)
  {
    auto current = insert(std::make_pair(key, Value()));
    return current.first->second;
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
  std::pair< Iterator< Key, Value, Cmp >, bool > UBST< Key, Value, Cmp >::insert(const std::pair< Key, Value >& value)
  {
    return emplace(value);
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< Iterator< Key, Value, Cmp >, bool > insert(std::pair< Key, Value >&& value)
  {
    return insert(std::move(value));
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIterator >
  void UBST< Key, Value, Cmp >::insert(InputIterator first, InputIterator last)
  {
    UBST< Key, Value, Cmp > tmp(*this);
    while (first != last)
    {
      tmp.insert(*first);
      ++first;
    }
    swap(tmp);
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  std::pair< Iterator< Key, Value, Cmp >, bool > UBST< Key, Value, Cmp >::emplace(Args&&... args)
  {
    node_t* newNode = new node_t(std::forward< Args >(args)...);
    const Key& key = newNode->data.first;
    if (root_ == fakeroot_)
    {
      root_ = newNode;
      root_->parent = fakeroot_;
      fakeroot_->left = root_;
      fakeroot_->left = root_;
      ++size_;
      return{ Iterator< Key, Value, Cmp >(root_), true };
    }
    node_t* current = root_;
    bool isLeft = false;
    while(current && current != fakeroot_)
    {
      if (cmp_(key, current->data.first))
      {
        current = current->left;
        isLeft = true;
      }
      else if (cmp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        delete newNode;
        return{ Iterator< Key, Value, Cmp >(current), false };
      }
    }
    newNode->parent = current;
    if (isLeft)
    {
      current->left = newNode;
    }
    else
    {
      current->right = newNode;
    }
    newNode->left = nullptr;
    newNode->right = nullptr;
    ++size_;
    return{ Iterator< Key, Value, Cmp >(newNode), true };
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
  size_t UBST< Key, Value, Cmp >::count(const Key& key) const noexcept
  {
    return (find(key) != end());
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< Iterator< Key, Value, Cmp >, Iterator< Key, Value, Cmp > > UBST< Key, Value, Cmp >::equal_range(const Key& key)
  {

  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > > UBST< Key, Value, Cmp >::equal_range(const Key& key) const
  {

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
