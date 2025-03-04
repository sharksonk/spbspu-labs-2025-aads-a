#ifndef NODE_HPP
#define NODE_HPP

namespace hismatova
{
  template < typename T >
  struct Node
  {
    T data;
    Node< T >* next;
  };
}

#endif
