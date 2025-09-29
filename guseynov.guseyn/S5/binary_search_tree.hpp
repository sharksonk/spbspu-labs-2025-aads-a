#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <stack>
#include <queue>
#include <vector>
#include <algorithm>
#include <tree.hpp>

namespace guseynov {
  template < typename Key, typename Value, typename Compare = std::less< Key > >
  class BinarySearchTree : public Tree< Key, Value, Compare > {
  public:
    using Base = Tree< Key, Value, Compare >;
    using Tree< Key, Value, Compare >::Tree;

    template< typename F >
    F traverse_lnr(F f) {
      if (this->empty()) {
        return f;
      }
      auto it = this->begin();
      auto end_it = this->end();
      while (it != end_it) {
        f(*it);
        ++it;
      }
      return f;
    }

    template< typename F >
    F traverse_lnr(F f) const {
      if (this->empty()) {
        return f;
      }
      auto it = this->cbegin();
      auto end_it = this->cend();
      while (it != end_it) {
        f(*it);
        ++it;
      }
      return f;
    }
    template< typename F >
    F traverse_rnl(F f) {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      auto it = this->begin();
      auto end_it = this->end();
      while (it != end_it) {
        elements.push_back(*it);
        ++it;
      }
      for (auto rit = elements.rbegin(); rit != elements.rend(); ++rit) {
        f(*rit);
      }
      return f;
    }

    template< typename F >
    F traverse_rnl(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      auto it = this->cbegin();
      auto end_it = this->cend();
      while (it != end_it) {
        elements.push_back(*it);
        ++it;
      }
      for (auto rit = elements.rbegin(); rit != elements.rend(); ++rit) {
        f(*rit);
      }
      return f;
    }
    template< typename F >
    F traverse_breadth(F f) {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->begin(); it != this->end(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
      });
      for (const auto& element : elements) {
        f(element);
      }
      return f;
    }

    template< typename F >
    F traverse_breadth(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > elements;
      for (auto it = this->cbegin(); it != this->cend(); ++it) {
        elements.push_back(*it);
      }
      std::sort(elements.begin(), elements.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
      });
      for (const auto& element : elements) {
        f(element);
      }
      return f;
    }
  };
}

#endif
