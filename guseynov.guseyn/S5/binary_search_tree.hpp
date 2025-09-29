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
      std::vector< std::pair< Key, Value > > all_elements;
      for (auto it = this->begin(); it != this->end(); ++it) {
        all_elements.push_back(*it);
      }
      std::vector< std::pair< Key, Value > > breadth_order;
      if (!all_elements.empty()) {
        std::vector< Key > keys;
        for (const auto& element : all_elements) {
          keys.push_back(element.first);
        }
        std::sort(keys.begin(), keys.end());
        Key root_key = keys[keys.size() / 2];
        std::queue< Key > key_queue;
        key_queue.push(root_key);
        std::set< Key > visited;
        while (!key_queue.empty()) {
          Key current_key = key_queue.front();
          key_queue.pop();
          if (visited.find(current_key) != visited.end()) {
            continue;
          }
          visited.insert(current_key);
          auto element_it = std::find_if(all_elements.begin(), all_elements.end(),
            [current_key](const auto& elem) { return elem.first == current_key; });
          if (element_it != all_elements.end()) {
            breadth_order.push_back(*element_it);
            f(*element_it);
          }
          for (const auto& element : all_elements) {
            if (visited.find(element.first) == visited.end()) {
              key_queue.push(element.first);
            }
          }
        }
      }
      return f;
    }

    template< typename F >
    F traverse_breadth(F f) const {
      if (this->empty()) {
        return f;
      }
      std::vector< std::pair< Key, Value > > all_elements;
      for (auto it = this->cbegin(); it != this->cend(); ++it) {
        all_elements.push_back(*it);
      }
      if (!all_elements.empty()) {
        std::vector< Key > keys;
        for (const auto& element : all_elements) {
          keys.push_back(element.first);
        }
        std::sort(keys.begin(), keys.end());
        for (const auto& key : keys) {
          auto element_it = std::find_if(all_elements.begin(), all_elements.end(),
            [key](const auto& elem) { return elem.first == key; });
          if (element_it != all_elements.end()) {
            f(*element_it);
          }
        }
      }
      return f;
    }
  };
}

#endif
