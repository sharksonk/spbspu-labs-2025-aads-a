#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <vector>
#include <functional>

namespace smirnov
{
  template< class Key, class Value, class Hash = std::hash< Key >, class Equal = std::equal_to< Key > >
  class HashTable
  {
  public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable & rhs);
    HashTable(HashTable && rhs) noexcept;
    HashTable & operator=(const HashTable & rhs);
    HashTable & operator=(HashTable &&) noexcept;
  private:
  };
}
#endif
