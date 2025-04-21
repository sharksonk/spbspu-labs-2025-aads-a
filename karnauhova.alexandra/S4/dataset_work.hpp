#ifndef DATASET_WORK_HPP
#define DATASET_WORK_HPP
#include <cstring>
#include <iostream>
#include "bitree.hpp"
namespace karnauhova
{
  using TreeKey = karnauhova::BiTree< size_t, std::string >;
  using DataTree = karnauhova::BiTree< std::string, TreeKey >;
  void print_dataset(std::ostream& out, std::string named, DataTree& dataset);
}
#endif
