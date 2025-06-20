#ifndef UBST_HPP
#define UBST_HPP

#include <functional>
#include "constiterator.hpp"
#include "iterator.hpp"
#include <stack.hpp>

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
    template< typename InputIterator >
    UBST(InputIterator first, InputIterator last);
    explicit UBST(std::initializer_list< std::pair< Key, Value > > il);
    ~UBST();

    UBST< Key, Value, Cmp >& operator=(const UBST< Key, Value, Cmp >& other);
    UBST< Key, Value, Cmp >& operator=(UBST< Key, Value, Cmp >&& other);
    bool operator==(const UBST< Key, Value, Cmp >& other) const noexcept;
    bool operator!=(const UBST< Key, Value, Cmp >& other) const noexcept;

    Iterator< Key, Value, Cmp > begin() noexcept;
    Iterator< Key, Value, Cmp > end() noexcept;
    ConstIterator< Key, Value, Cmp > begin() const noexcept;
    ConstIterator< Key, Value, Cmp > end() const noexcept;
    ConstIterator< Key, Value, Cmp > cbegin() const noexcept;
    ConstIterator< Key, Value, Cmp > cend() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;

    void clear() noexcept;

    void swap(UBST& other);

    std::pair< Iterator< Key, Value, Cmp >, bool > insert(const std::pair< Key, Value >& value);
    std::pair< Iterator< Key, Value, Cmp >, bool > insert(std::pair< Key, Value >&& value);
    template< typename InputIterator >
    void insert(InputIterator first, InputIterator last);
    template< typename... Args >
    Iterator< Key, Value, Cmp > insert(ConstIterator< Key, Value, Cmp > hint, Args&&... args);
    Iterator< Key, Value, Cmp > erase(Iterator< Key, Value, Cmp > position);
    Iterator< Key, Value, Cmp > erase(ConstIterator< Key, Value, Cmp > position);
    Iterator< Key, Value, Cmp > erase(Iterator< Key, Value, Cmp > first, Iterator< Key, Value, Cmp > last);
    Iterator< Key, Value, Cmp > erase(ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > last);
    size_t erase(const Key& key);
    template< typename... Args >
    std::pair< Iterator< Key, Value, Cmp >, bool > emplace(Args&&... args);
    template< typename... Args >
    Iterator< Key, Value, Cmp > emplace_hint(ConstIterator< Key, Value, Cmp > hint, Args&&... args);

    Iterator< Key, Value, Cmp > find(const Key& key) noexcept;
    ConstIterator< Key, Value, Cmp > find(const Key& key) const noexcept;

    size_t count(const Key& key) const noexcept;

    std::pair< Iterator< Key, Value, Cmp >, Iterator< Key, Value, Cmp > > equal_range(const Key& key);
    std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > > equal_range(const Key& key) const;
    Iterator< Key, Value, Cmp > lower_bound(const Key& key);
    ConstIterator< Key, Value, Cmp > lower_bound(const Key& key) const;
    Iterator< Key, Value, Cmp > upper_bound(const Key& key);
    ConstIterator< Key, Value, Cmp > upper_bound(const Key& key) const;

    private:
      using node_t = kushekbaev::TreeNode< Key, Value, Cmp >;
      node_t* fakeroot_;
      node_t* root_;
      long long int size_;
      Cmp cmp_;
      size_t height(node_t* node) const noexcept;
      void killChildrenOf(node_t* node);
      template< typename Pair >
      std::pair< Iterator < Key, Value, Cmp >, bool> insertImpl(Pair&& value);
      node_t* copySubtree(node_t* node, node_t* parent);
  };

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp>::UBST():
    fakeroot_(reinterpret_cast< node_t* >(std::malloc(sizeof(node_t)))),
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
    fakeroot_(reinterpret_cast<node_t*>(std::malloc(sizeof(node_t)))),
    root_(fakeroot_),
    size_(0)
  {
    if (other.root_ == other.fakeroot_)
    {
      return;
    }
    fakeroot_->left = fakeroot_;
    fakeroot_->right = fakeroot_;
    fakeroot_->parent = nullptr;
    fakeroot_->height = -1;
    if (!other.empty())
    {
      root_ = copySubtree(other.root_, fakeroot_);
      size_ = other.size_;
      node_t* minNode = root_;
      while (minNode->left)
      {
        minNode = minNode->left;
      }
      fakeroot_->left = minNode;
      node_t* maxNode = root_;
      while (maxNode->right)
      {
        maxNode = maxNode->right;
      }
      fakeroot_->right = maxNode;
    }
  }

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp>::UBST(UBST< Key, Value, Cmp >&& other):
    fakeroot_(std::move(other.fakeroot_)),
    root_(std::move(other.root_)),
    size_(std::move(other.size_)),
    cmp_(std::move(other.cmp_))
  {
    other.fakeroot_ = nullptr;
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename InputIterator >
  UBST< Key, Value, Cmp >::UBST(InputIterator first, InputIterator last):
    UBST()
  {
    for (auto it = first; it != last; ++it)
    {
      insert(*it);
    }
  }

  template< typename Key, typename Value, typename Cmp >
  UBST< Key, Value, Cmp >::UBST(std::initializer_list< std::pair< Key, Value > > il):
    UBST()
  {
    for (auto it = il.begin(); it != il.end(); ++it)
    {
      insert(*it);
    }
  }

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
  bool UBST< Key, Value, Cmp >::operator==(const UBST< Key, Value, Cmp >& other) const noexcept
  {
    if (size() != other.size())
    {
      return false;
    }
    auto it1 = begin();
    auto it2 = other.begin();
    while (it1 != end())
    {
      if (*it1 != *it2)
      {
        return false;
      }
      ++it1;
      ++it2;
    }
    return true;
  }

  template< typename Key, typename Value, typename Cmp >
  bool UBST< Key, Value, Cmp >::operator!=(const UBST< Key, Value, Cmp >& other) const noexcept
  {
    return !(*this == other);
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
      current = current->left;
    }
    return Iterator< Key, Value, Cmp >(current);
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::end() noexcept
  {
    return Iterator< Key, Value, Cmp >(fakeroot_);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::begin() const noexcept
  {
    if (empty())
    {
      return end();
    }
    node_t* current = root_;
    while (current->left)
    {
      current = current->left;
    }
    return ConstIterator< Key, Value, Cmp >(current);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::end() const noexcept
  {
    return ConstIterator< Key, Value, Cmp >(fakeroot_);
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
      current = current->left;
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
    return const_cast< Value& >(static_cast< const UBST* >(this)->at(key));
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
    auto it = find(key);
    if (it == end())
    {
      auto inserted = emplace(key, Value());
      return inserted.first->second;
    }
    return it->second;
  }

  template< typename Key, typename Value, typename Cmp >
  const Value& UBST< Key, Value, Cmp >::operator[](const Key& key) const
  {
    auto it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("Key not found");
    }
    return it->second;
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
    if (!node || node == fakeroot_)
    {
      return;
    }
    Stack< node_t* > stack;
    stack.push(node);
    while (!stack.empty())
    {
      node_t* current = stack.top();
      stack.pop();
      if (current->left)
      {
        stack.push(current->left);
      }
      if (current->right)
      {
        stack.push(current->right);
      }
      delete current;
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
  template< typename Pair >
  std::pair< Iterator< Key, Value, Cmp >, bool >
  UBST< Key, Value, Cmp >::insertImpl(Pair&& value)
  {
    return emplace(std::forward< Pair >(value));
  }

  template<typename Key, typename Value, typename Cmp>
  std::pair< Iterator< Key, Value, Cmp >, bool>
  UBST< Key, Value, Cmp >::insert(const std::pair< Key, Value >& value)
  {
    return insertImpl(value);
  }

  template<typename Key, typename Value, typename Cmp>
  std::pair< Iterator< Key, Value, Cmp >, bool>
  UBST< Key, Value, Cmp >::insert(std::pair< Key, Value >&& value)
  {
    return insertImpl(std::move(value));
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
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::insert(ConstIterator< Key, Value, Cmp > hint, Args&&... args)
  {
    return emplace_hint(hint, args...);
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::erase(Iterator< Key, Value, Cmp > position)
  {
    if (position == end())
    {
      return end();
    }
    node_t* todelete = position.node_;
    node_t* parent = todelete->parent;
    Iterator< Key, Value, Cmp > result = Iterator< Key, Value, Cmp >(parent);
    if (!(todelete->left) && !(todelete->right))
    {
      if (parent->left == todelete)
      {
        parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
      result = Iterator< Key, Value, Cmp >(parent);
    }
    else if (!(todelete->left) || !(todelete->right))
    {
      node_t* child = (todelete->left) ? todelete->left : todelete->right;
      if (parent->left == todelete)
      {
        parent->left = child;
      }
      else
      {
        parent->right = child;
      }
      child->parent = parent;
      result = Iterator< Key, Value, Cmp >(child);
    }
    else
    {
      node_t* next = todelete->right;
      while (next->left)
      {
        next = next->left;
      }
      todelete->data = next->data;
      return erase(Iterator< Key, Value, Cmp >(next));
    }
    if (todelete == root_)
    {
      root_ = (parent == fakeroot_) ? parent : fakeroot_;
      result = begin();
    }

    delete todelete;
    --size_;
    return result;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::erase(ConstIterator< Key, Value, Cmp > position)
  {
    if (position == cend())
    {
      return end();
    }
    node_t* todelete = position.node_;
    node_t* parent = todelete->parent;
    Iterator< Key, Value, Cmp > result = Iterator< Key, Value, Cmp >(parent);
    if (!(todelete->left) && !(todelete->right))
    {
      if (parent->left == todelete)
      {
        parent->left = nullptr;
      }
      else
      {
        parent->right = nullptr;
      }
      result = Iterator< Key, Value, Cmp >(parent);
    }
    else if (!(todelete->left) || !(todelete->right))
    {
      node_t* child = (todelete->left) ? todelete->left : todelete->right;
      if (parent->left == todelete)
      {
        parent->left = child;
      }
      else
      {
        parent->right = child;
      }
      child->parent = parent;
      result = Iterator< Key, Value, Cmp >(child);
    }
    else
    {
      node_t* next = todelete->right;
      while (next->left)
      {
        next = next->left;
      }
      todelete->data = next->data;
      return erase(Iterator< Key, Value, Cmp >(next));
    }
    if (todelete == root_)
    {
      root_ = (parent == fakeroot_) ? parent : fakeroot_;
      result = begin();
    }

    delete todelete;
    --size_;
    return result;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::erase(Iterator< Key, Value, Cmp > first, Iterator< Key, Value, Cmp > last)
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
    return last;
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::erase(ConstIterator< Key, Value, Cmp >first, ConstIterator< Key, Value, Cmp > last)
  {
    for (auto it = first; it != last;)
    {
      it = erase(it);
    }
    return last;
  }

  template< typename Key, typename Value, typename Cmp >
  size_t UBST< Key, Value, Cmp >::erase(const Key& key)
  {
    Iterator< Key, Value, Cmp > it = find(key);
    if (it == end())
    {
      return 0;
    }
    erase(it);
    return 1;
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
      fakeroot_->right = root_;
      ++size_;
      return{ Iterator< Key, Value, Cmp >(root_), true };
    }
    node_t* current = root_;
    node_t* parent = nullptr;
    bool isLeft = false;
    while(current && current != fakeroot_)
    {
      parent = current;
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
    if (!parent)
    {
      delete newNode;
      throw std::logic_error("Parent is nullptr!");
    }
    if (isLeft)
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    if (cmp_(key, fakeroot_->left->data.first))
    {
      fakeroot_->left = newNode;
    }
    if (cmp_(fakeroot_->right->data.first, key))
    {
      fakeroot_->right = newNode;
    }
    ++size_;
    return{ Iterator< Key, Value, Cmp >(newNode), true };
  }

  template< typename Key, typename Value, typename Cmp >
  template< typename... Args >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::emplace_hint(ConstIterator< Key, Value, Cmp > hint, Args&&... args)
  {
    if (empty())
    {
      return emplace(std::forward< Args >(args)...).first;
    }
    std::pair<Key, Value> newData(std::forward<Args>(args)...);
    auto newKey = newData.first;
    if (hint != cend())
    {
      auto hintKey = hint->first;
      auto nextHint = hint;
      ++hint;
      if (cmp_(hintKey, newKey) && (nextHint == cend()) || (cmp_(newKey, nextHint->first)))
      {
        node_t* newNode = new node_t(std::move(newData));
        node_t* hintNode = hint.node_;
        newNode->parent = hintNode;
        ++size_;
        return Iterator< Key, Value, Cmp >(newNode);
      }
    }
    return emplace(std::move(newData)).first;
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

  template< typename Key, typename Value, typename Cmp >
  size_t UBST< Key, Value, Cmp >::count(const Key& key) const noexcept
  {
    return (find(key) != cend());
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< Iterator< Key, Value, Cmp >, Iterator< Key, Value, Cmp > > UBST< Key, Value, Cmp >::equal_range(const Key& key)
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  std::pair< ConstIterator< Key, Value, Cmp >, ConstIterator< Key, Value, Cmp > > UBST< Key, Value, Cmp >::equal_range(const Key& key) const
  {
    return std::make_pair(lower_bound(key), upper_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::lower_bound(const Key& key)
  {
    node_t* current = root_;
    node_t* result = fakeroot_;
    while (current != fakeroot_ && current)
    {
      if (cmp_(current->data.first, key))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return Iterator< Key, Value, Cmp >(result);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::lower_bound(const Key& key) const
  {
    return ConstIterator< Key, Value, Cmp >(lower_bound(key));
  }

  template< typename Key, typename Value, typename Cmp >
  Iterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::upper_bound(const Key& key)
  {
    node_t* current = root_;
    node_t* result = fakeroot_;
    while (current != fakeroot_ && current)
    {
      if (cmp_(key, current->data.first))
      {
        result = current;
        current = current->left;
      }
      else
      {
        current = current->right;
      }
    }
    return Iterator< Key, Value, Cmp >(result);
  }

  template< typename Key, typename Value, typename Cmp >
  ConstIterator< Key, Value, Cmp > UBST< Key, Value, Cmp >::upper_bound(const Key& key) const
  {
    return ConstIterator< Key, Value, Cmp >(upper_bound(key));
  }

  template<typename Key, typename Value, typename Cmp>
  TreeNode< Key, Value, Cmp >* UBST<Key, Value, Cmp>::copySubtree(node_t* node, node_t* parent)
  {
    if (!node || node->height == -1)
    {
      return nullptr;
    }
    node_t* newNode = new node_t(node->data);
    newNode->parent = parent;
    newNode->height = node->height;
    newNode->left = copySubtree(node->left, newNode);
    newNode->right = copySubtree(node->right, newNode);
    return newNode;
  }
}

#endif
