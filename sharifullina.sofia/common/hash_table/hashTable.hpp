#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <boost/hash2/xxhash.hpp>
#include "iterator.hpp"
#include "hashNode.hpp"

namespace sharifullina
{
  namespace detail
  {
    template< class Key >
    struct XXHash
    {
      size_t operator()(const Key & key) const
      {
        boost::hash2::xxhash_64 hasher;
        hasher.update(std::addressof(key), sizeof(Key));
        return hasher.result();
      }
    };
  }

  template< class Key, class T, class HS1 = std::hash< Key >, class HS2 = detail::XXHash< Key >, class EQ = std::equal_to< Key > >
  struct HashTable
  {
    using iterator = HashConstIterator< Key, T, HS1, HS2, EQ >;

    HashTable();
    HashTable(const HashTable & rhs);
    HashTable(HashTable && rhs) noexcept;
    ~HashTable();

    HashTable & operator=(const HashTable & rhs);
    HashTable & operator=(HashTable && rhs) noexcept;

    iterator begin() const noexcept;
    iterator end() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    T & at(const Key & key);
    const T & at(const Key & key) const;

    std::pair< iterator, bool > insert(const Key & key, const T & value);
    iterator erase(iterator pos);
    size_t erase(const Key & key);

    void swap(HashTable & rhs) noexcept;

    iterator find(const Key & key) const noexcept;

    float loadFactor() const noexcept;
    void rehash(size_t newCapacity);

  private:
    HashNode< Key, T > * slots_;
    size_t capacity_;
    size_t size_;
    float maxLoadFactor_ = 0.7f;

    std::pair< size_t, size_t > calculatePositions(const Key & key) const noexcept;
    std::pair< size_t, bool > findPosition(const Key & key) const noexcept;
    void rehashIfNeeded();
  };

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable():
    slots_(new HashNode< Key, T >[10]),
    capacity_(10),
    size_(0)
  {}

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::~HashTable()
  {
    delete[] slots_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(const HashTable & rhs):
    slots_(new HashNode< Key, T >[rhs.capacity_]),
    capacity_(rhs.capacity_),
    size_(rhs.size_)
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (rhs.slots_[i].occupied && !rhs.slots_[i].deleted)
      {
        slots_[i] = rhs.slots_[i];
      }
    }
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ >::HashTable(HashTable && rhs) noexcept:
    slots_(rhs.slots_),
    capacity_(rhs.capacity_),
    size_(rhs.size_)
  {
    rhs.slots_ = nullptr;
    rhs.capacity_ = 0;
    rhs.size_ = 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ > & HashTable< Key, T, HS1, HS2, EQ >::operator=(const HashTable & rhs)
  {
    if (this != std::addressof(rhs))
    {
      HashTable tmp(rhs);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashTable< Key, T, HS1, HS2, EQ > & HashTable< Key, T, HS1, HS2, EQ >::operator=(HashTable && rhs) noexcept
  {
    if (this != std::addressof(rhs))
    {
      delete[] slots_;
      slots_ = rhs.slots_;
      capacity_ = rhs.capacity_;
      size_ = rhs.size_;
      rhs.slots_ = nullptr;
      rhs.capacity_ = 0;
      rhs.size_ = 0;
    }
    return *this;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::begin() const noexcept
  {
    return iterator(slots_, capacity_, 0);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::swap(HashTable & rhs) noexcept
  {
    std::swap(slots_, rhs.slots_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(size_, rhs.size_);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::end() const noexcept
  {
    return iterator(slots_, capacity_, capacity_);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  size_t HashTable< Key, T, HS1, HS2, EQ >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  bool HashTable< Key, T, HS1, HS2, EQ >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< size_t, size_t > HashTable< Key, T, HS1, HS2, EQ >::calculatePositions(const Key & key) const noexcept
  {
    size_t h1 = HS1{}(key) % capacity_;
    size_t h2 = HS2{}(key) % (capacity_ - 1) + 1;
    return {h1, h2};
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< size_t, bool > HashTable< Key, T, HS1, HS2, EQ >::findPosition(const Key & key) const noexcept
  {
    auto positions = calculatePositions(key);
    size_t h1 = positions.first;
    size_t h2 = positions.second;
    size_t deletedSlot = capacity_;

    for (size_t i = 0; i < capacity_; ++i)
    {
      size_t index = (h1 + i * h2) % capacity_;

      if (!slots_[index].occupied)
      {
        if (deletedSlot != capacity_)
        {
          return {deletedSlot, true};
        }
        return {index, true};
      }
      else if (slots_[index].deleted && deletedSlot == capacity_)
      {
        deletedSlot = index;
      }
      else if (EQ{}(slots_[index].data.first, key))
      {
        return {index, false};
      }
    }

    return {capacity_, false};
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::rehashIfNeeded()
  {
    if (loadFactor() >= maxLoadFactor_)
    {
      rehash(capacity_ * 2);
    }
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  std::pair< HashConstIterator< Key, T, HS1, HS2, EQ >, bool > HashTable< Key, T, HS1, HS2, EQ >::insert(const Key & key, const T & value)
  {
    rehashIfNeeded();

    auto positionInfo = findPosition(key);
    size_t pos = positionInfo.first;
    bool isNew = positionInfo.second;

    if (pos == capacity_)
    {
      throw std::runtime_error("Hash table is full");
    }

    if (isNew)
    {
      slots_[pos].data = std::make_pair(key, value);
      slots_[pos].occupied = true;
      slots_[pos].deleted = false;
      ++size_;
      return {iterator(slots_, capacity_, pos), true};
    }

    return {iterator(slots_, capacity_, pos), false};
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::erase(iterator pos)
  {
    if (pos == end())
    {
      return end();
    }

    size_t index = pos.current_;
    slots_[index].occupied = false;
    slots_[index].deleted = true;
    --size_;

    ++pos;
    return pos;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  size_t HashTable< Key, T, HS1, HS2, EQ >::erase(const Key & key)
  {
    auto it = find(key);
    if (it != end())
    {
      erase(it);
      return 1;
    }
    return 0;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  HashConstIterator< Key, T, HS1, HS2, EQ > HashTable< Key, T, HS1, HS2, EQ >::find(const Key & key) const noexcept
  {
    auto positionInfo = findPosition(key);
    size_t pos = positionInfo.first;
    bool isNew = positionInfo.second;

    if (pos == capacity_ || isNew)
    {
      return end();
    }

    return iterator(slots_, capacity_, pos);
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  T & HashTable< Key, T, HS1, HS2, EQ >::at(const Key & key)
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return slots_[it.current_].data.second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  const T & HashTable< Key, T, HS1, HS2, EQ >::at(const Key & key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found");
    }
    return slots_[it.current_].data.second;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  float HashTable< Key, T, HS1, HS2, EQ >::loadFactor() const noexcept
  {
    return static_cast< float >(size_) / capacity_;
  }

  template< class Key, class T, class HS1, class HS2, class EQ >
  void HashTable< Key, T, HS1, HS2, EQ >::rehash(size_t newCapacity)
  {
    if (newCapacity <= capacity_)
    {
      return;
    }

    HashNode< Key, T > * newSlots = new HashNode< Key, T >[newCapacity];

    for (size_t i = 0; i < capacity_; ++i)
    {
      if (slots_[i].occupied && !slots_[i].deleted)
      {
        const Key & key = slots_[i].data.first;
        auto positions = calculatePositions(key);
        size_t h1 = positions.first;
        size_t h2 = positions.second;

        for (size_t j = 0; j < newCapacity; ++j)
        {
          size_t index = (h1 + j * h2) % newCapacity;
          if (!newSlots[index].occupied)
          {
            newSlots[index] = slots_[i];
            break;
          }
        }
      }
    }

    delete[] slots_;
    slots_ = newSlots;
    capacity_ = newCapacity;
  }
}
#endif
