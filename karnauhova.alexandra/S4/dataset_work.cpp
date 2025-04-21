#include "dataset_work.hpp"

void karnauhova::print_dataset(std::ostream& out, std::string named, DataTree* dataset)
{
  TreeKey tree;
  tree = dataset->get_value(named);
  out << "meow";

}