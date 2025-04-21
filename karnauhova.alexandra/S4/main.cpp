#include <iostream>
#include <functional>
#include <cstring>
#include <fstream>
#include "bitree.hpp"
#include "dataset_work.hpp"

namespace 
{
  using TreeKey = karnauhova::BiTree< size_t, std::string >;
  using DataTree = karnauhova::BiTree< std::string, TreeKey >;
  void input_trees(std::ifstream& in, DataTree& map_trees)
  {
    std::string name_map;
    while (!in.eof())
    {
      TreeKey tree_value;
      in >> name_map;
      size_t key = 0;
      while (in >> key)
      {
        std::string value;
        in >> value;
        tree_value.push(key, value);
        std::cout << "1";
      }
      map_trees.push(name_map, tree_value);
      in.clear();
    }
  }
}

int main(int argc, char* argv[])
{
  using TreeKey = karnauhova::BiTree< size_t, std::string >;
  using DataTree = karnauhova::BiTree< std::string, TreeKey >;
  if (argc != 2)
  {
    std::cerr << "Incorrect file\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  DataTree set_trees;
  try
  {
    input_trees(file, set_trees);
  }
  catch(const std::exception& e)
  {
    std::cerr << "1" << e.what() << '\n';
    return 1;
  }
  while (!std::cin.eof())
  {
    std::string name_operat;
    std::cin >> name_operat;
    if (name_operat == "print")
    {
      std::string name_tree;
      karnauhova::print_dataset(std::cout, name_tree, set_trees);
    }
  }

}


/* karnauhova::BiTree< size_t, std::string, std::less< std::pair< size_t, std::string > > > meow;
  meow.push(1, "meow");
  meow.push(2, "meow meow");
  meow.push(3, "meow meow meow");
  meow.push(4, "lol");
  std::cout  << meow.get_value(4) << "\n"; */