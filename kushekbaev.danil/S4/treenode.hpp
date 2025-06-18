#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <cstddef>
#include <utility>

namespace kushekbaev
{
  template< typename Key, typename Value >
  struct TreeNode
  {
    template< typename... Args >
    explicit TreeNode(Args&&... args) noexcept;

    std::pair< Key, Value > data;
    TreeNode< Key, Value >* parent;
    TreeNode< Key, Value >* left;
    TreeNode< Key, Value >* right;
    size_t height;
  };

  template< typename Key, typename Value >
  template< typename... Args >
  TreeNode< Key, Value >::TreeNode(Args&&... args) noexcept:
    data{std::pair< Key, Value >(std::forward< Args >(args)...)},
    parent(nullptr),
    right(nullptr),
    left(nullptr),
    height(0)
  {}
}

#endif