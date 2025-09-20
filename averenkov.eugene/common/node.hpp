#ifndef NODE_HPP
#define NODE_HPP
#include <utility>
#include <cstddef>

namespace averenkovDetail
{

  template< class Key, class Value >
  struct Node
  {
    std::pair< const Key, Value > data;
    Node* left;
    Node* right;
    Node* parent;
    size_t height;
    Node(const Key& k, const Value& v, Node* p);
  };

}

template < class Key, class Value >
averenkovDetail::Node< Key, Value >::Node(const Key& k, const Value& v, Node* p):
  data(std::make_pair(k, v)),
  left(nullptr),
  right(nullptr),
  parent(p),
  height(1)
{
}

#endif
