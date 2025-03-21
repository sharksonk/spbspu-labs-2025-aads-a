#ifndef NODE_HPP
#define NODE_HPP

namespace hismatova
{
  template < typename T >
  struct Node
  {
    T data;
    Node* next;
    Node(const T& value, Node* nextNode = nullptr):
      data(value),
      next(nextNode)
    {}
  };
}

#endif
