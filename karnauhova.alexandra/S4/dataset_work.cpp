#include "dataset_work.hpp"

void karnauhova::print_dataset(std::ostream& out, std::string named, DataTree& dataset)
{
  TreeKey tree;
  tree = dataset.get_value(named);
  out << named;
  for (auto it = tree.begin(); it != tree.end(); it++)
  {
    std::pair< size_t, std::string > now = *it;
    out << " " << now.first << " " << now.second;
  }
  out << "\n";
}