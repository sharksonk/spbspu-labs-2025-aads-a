#ifndef RBTREE_HPP
#define RBTREE_HPP
#include "tree_node.hpp"
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare >
  class RBTree
  {
  public:
    using Node = TreeNode< Key, Value >;
    RBTree();
    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);
  private:
    Node fake_;
  };

  template< typename Key, typename Value, typename Compare >
  RBTree< Key, Value, Compare >::RBTree():
    fake_(reinterpret_cast< Node* >(new char[sizeof(Node)])),
  {
    //фейк листья
  } 
}

#endif
