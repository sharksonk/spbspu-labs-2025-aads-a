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
  private:
    std::vector< Bucket< Key, Value > > buckets_;
    size_t size_;
    Hash hasher_;
    Equal key_equal_;
    static constexpr float max_load_factor = 0.75f;
    size_t probe(size_t hash_value, size_t attempt) const noexcept;
    void rehash(size_t new_capacity);
  };
}
#endif
