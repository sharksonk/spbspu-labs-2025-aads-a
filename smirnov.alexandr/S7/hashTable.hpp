#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <vector>
#include <functional>
#include <cstddef>
#include "bucket.hpp"

namespace smirnov
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    HashTable();
    ~HashTable() = default;
    HashTable(const HashTable & rhs) = default;
    HashTable(HashTable && rhs) noexcept = default;
    HashTable & operator=(const HashTable & rhs) = default;
    HashTable & operator=(HashTable &&) noexcept = default;

    size_t size() const noexcept;
    bool empty() const noexcept;
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
    if (size_ == 0)
    {
      return 0.0;
    }
    return static_cast< float >(size_) / buckets_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
    buckets_(8),
    size_(0),
    hasher_(),
    key_equal_()
  {}

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::probe(size_t hash_value, size_t attempt) const noexcept
  {
    return (hash_value + attempt * attempt) % buckets_.size();
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t new_capacity)
  {
    for (size_t i = 0; i < buckets_.size(); i++)
    {
      if (!buckets_[i].occupied)
      {
        continue;
      }
    }
  }
}
#endif
