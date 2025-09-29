#ifndef FILES_HPP
#define FILES_HPP

#include <string>
#include <stdexcept>
#include <limits>

struct KeySumm {
  int result_ = 0;
  std::string values_;
  void operator()(const std::pair< const int, std::string >& key_value) {
    if (result_ > 0 && key_value.first > std::numeric_limits<int>::max() - result_) {
      throw std::overflow_error("Integer overflow detected");
    }
    if (result_ < 0 && key_value.first < std::numeric_limits<int>::min() - result_) {
      throw std::underflow_error("Integer underflow detected");
    }
    result_ += key_value.first;
    if (!values_.empty()) {
      values_ += " ";
    }
    values_ += key_value.second;
  }
};

#endif
