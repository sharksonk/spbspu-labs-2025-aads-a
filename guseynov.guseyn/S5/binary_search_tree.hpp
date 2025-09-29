#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <stack>
#include <queue>
#include "tree.hpp"

namespace guseynov {
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree : public Tree< Key, Value, Compare > {
  public:
    using Base = Tree< Key, Value, Compare >;
    using Node_t = Node< Key, Value >;
    using Tree< Key, Value, Compare >::Tree;

    template< typename F >
    F traverse_lnr(F f) {
      if (this->empty()) {
        return f;
      }
      std::stack< Node_t* > stack;
      Node_t* current = this->getRoot();
      while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
          stack.push(current);
          current = current->left;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->right;
      }
      return f;
    }

    template< typename F >
    F traverse_lnr(F f) const {
      if (this->empty()) {
        return f;
      }
      std::stack< const Node_t* > stack;
      const Node_t* current = this->getRoot();
      while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
          stack.push(current);
          current = current->left;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->right;
      }
      return f;
    }
    template< typename F >
    F traverse_rnl(F f) {
      if (this->empty()) {
        return f;
      }
      std::stack< Node_t* > stack;
      Node_t* current = this->getRoot();
      while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
          stack.push(current);
          current = current->right;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->left;
      }
      return f;
    }

    template< typename F >
    F traverse_rnl(F f) const {
      if (this->empty()) {
        return f;
      }
      std::stack< const Node_t* > stack;
      const Node_t* current = this->getRoot();
      while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
          stack.push(current);
          current = current->right;
        }
        current = stack.top();
        stack.pop();
        f(current->data);
        current = current->left;
      }
      return f;
    }
    template< typename F >
    F traverse_breadth(F f) {
      if (this->empty()) {
        return f;
      }
      std::queue< Node_t* > queue;
      queue.push(this->getRoot());
      while (!queue.empty()) {
        Node_t* current = queue.front();
        queue.pop();
        f(current->data);
        if (current->left != nullptr) {
          queue.push(current->left);
        }
        if (current->right != nullptr) {
          queue.push(current->right);
        }
      }
      return f;
    }

    template< typename F >
    F traverse_breadth(F f) const {
      if (this->empty()) {
        return f;
      }
      std::queue< const Node_t* > queue;
      queue.push(this->getRoot());
      while (!queue.empty()) {
        const Node_t* current = queue.front();
        queue.pop();
        f(current->data);
        if (current->left != nullptr) {
          queue.push(current->left);
        }
        if (current->right != nullptr) {
          queue.push(current->right);
        }
      }
      return f;
    }
  };
}

#endif
