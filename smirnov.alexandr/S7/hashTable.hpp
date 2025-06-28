#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <functional>
#include <stdexcept>
#include <vector>
#include "bucket.hpp"
#include "constIteratorHash.hpp"
#include "iteratorHash.hpp"

namespace smirnov
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    using iterator = IteratorHash< Key, Value, Hash, Equal >;
    using const_iterator = ConstIteratorHash< Key, Value, Hash, Equal >;

    HashTable();
    ~HashTable() = default;
    HashTable(const HashTable & rhs) = default;
    HashTable(HashTable && rhs) noexcept = default;
    HashTable & operator=(const HashTable & rhs) = default;
    HashTable & operator=(HashTable &&) noexcept = default;

    Value & operator[](const Key & key);
    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    iterator begin();
    iterator end();
    iterator find(const Key & key);

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator find(const Key & key) const;

    size_t size() const noexcept;
    bool empty() const noexcept;
    std::pair< iterator, bool > insert(const Key & key, const Value & value);
    size_t erase(const Key & key);
    float load_factor() const noexcept;
  private:
    std::vector< Bucket< Key, Value > > buckets_;
    size_t size_;
    Hash hasher_;
    Equal key_equal_;
    static constexpr float max_load_factor = 0.75f;
    size_t probe(size_t hash_value, size_t attempt) const noexcept;
    void rehash(size_t new_capacity);
  };

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    buckets_(8),
    size_(0),
    hasher_(),
    key_equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  float HashTable< Key, Value, Hash, Equal >::load_factor() const noexcept
  {
    if (buckets_.empty())
    {
      return 0.0f;
    }
    return static_cast< float >(size_) / buckets_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin()
  {
    return iterator(std::addressof(buckets_), 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end()
  {
    return iterator(std::addressof(buckets_), buckets_.size());
  }

  template< class Key, class Value, class Hash, class Equal >
  IteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key & key)
  {
    if (empty())
    {
      return end();
    }
    size_t hash_value = hasher_(key) % buckets_.size();
    size_t attempt = 0;
    size_t index = hash_value;
    while (attempt < buckets_.size())
    {
      index = probe(hash_value, attempt);
      auto & bucket = buckets_[index];
      if (bucket.occupied && key_equal_(bucket.data.first, key))
      {
        return iterator(std::addressof(buckets_), index);
      }
      if (!bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++attempt;
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    return const_iterator(std::addressof(buckets_), 0);
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return const_iterator(std::addressof(buckets_), buckets_.size());
  }

  template< class Key, class Value, class Hash, class Equal >
  ConstIteratorHash< Key, Value, Hash, Equal > HashTable< Key, Value, Hash, Equal >::find(const Key & key) const
  {
    if (empty())
    {
      return end();
    }
    size_t hash_value = hasher_(key) % buckets_.size();
    size_t attempt = 0;
    size_t index = hash_value;
    while (attempt < buckets_.size())
    {
      index = probe(hash_value, attempt);
      const auto & bucket = buckets_[index];
      if (bucket.occupied && key_equal_(bucket.data.first, key))
      {
        return const_iterator(std::addressof(buckets_), index);
      }
      if (!bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++attempt;
    }
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::operator[](const Key & key)
  {
    auto it = find(key);
    if (it != end())
    {
      return it->second;
    }
    auto result = insert(key, Value());
    return result.first->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key)
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found\n");
    }
    return it->second;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::at(const Key & key) const
  {
    auto it = find(key);
    if (it == end())
    {
      throw std::out_of_range("Key not found\n");
    }
    return it->second;
  }

  template < class Key, class Value, class Hash, class Equal >
  std::pair< IteratorHash< Key, Value, Hash, Equal >, bool >
      HashTable< Key, Value, Hash, Equal >::insert(const Key & key, const Value & value)
  {
    if (load_factor() >= max_load_factor)
    {
      rehash(buckets_.size() * 2);
    }
    size_t hash_value = hasher_(key) % buckets_.size();
    size_t attempt = 0;
    size_t index = 0;
    size_t first_deleted = buckets_.size();
    while (attempt < buckets_.size())
    {
      index = probe(hash_value, attempt);
      auto & bucket = buckets_[index];
      if (bucket.occupied && !bucket.deleted && key_equal_(bucket.data.first, key))
      {
        return {iterator(std::addressof(buckets_), index), false};
      }
      if (bucket.deleted && first_deleted == buckets_.size())
      {
        first_deleted = index;
      }
      if (!bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++attempt;
    }
    if (first_deleted != buckets_.size())
    {
      index = first_deleted;
    }
    buckets_[index].data = {key, value};
    buckets_[index].occupied = true;
    buckets_[index].deleted = false;
    ++size_;
    return {iterator(std::addressof(buckets_), index), true};
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::erase(const Key & key)
  {
    size_t hash_value = hasher_(key) % buckets_.size();
    size_t attempt = 0;
    size_t index = 0;
    while (attempt < buckets_.size())
    {
      index = probe(hash_value, attempt);
      auto & bucket = buckets_[index];
      if (bucket.occupied &&  !bucket.deleted && key_equal_(bucket.data.first, key))
      {
        bucket.occupied = false;
        bucket.deleted = true;
        size_--;
        return 1;
      }
      if (!bucket.occupied && !bucket.deleted)
      {
        break;
      }
      ++attempt;
    }
    return 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t hash_value, size_t attempt) const noexcept
  {
    return (hash_value + attempt * attempt) % buckets_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_capacity)
  {
    std::vector< Bucket< Key, Value > > old_buckets = std::move(buckets_);
    buckets_.clear();
    buckets_.resize(new_capacity);
    size_ = 0;
    for (size_t i = 0; i < old_buckets.size(); ++i)
    {
      const auto & bucket = old_buckets[i];
      if (bucket.occupied)
      {
        insert(bucket.data.first, bucket.data.second);
      }
    }
  }
}
#endif
