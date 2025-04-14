#ifndef TREE_NODE_HPP
#define TREE_NODE_HPP

namespace karnauhova
{
  enum Color 
  {
	Black,
	Red
  };

  template< typename Key, typename Value >
  struct TreeNode
  {
    Color color;
    std::pair< Key, Value > data;
    TreeNode< Key, Value >* left, * right, * parent;
  };
}

#endif
