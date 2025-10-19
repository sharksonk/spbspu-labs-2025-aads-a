
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <string>
#include <tree.hpp>
#include "key_summer.hpp"

namespace guseynov
{
  using CommandTree = Tree< std::string, std::function< void() >, std::less< std::string > >;
  using DataTree = Tree< long long, std::string, std::less< long long > >;

  template< class F >
  void ascending(DataTree& tree, F& f)
  {
    tree.traverse_lnr(f);
  }

  template< class F >
  void descending(DataTree& tree, F& f)
  {
    tree.traverse_rnl(f);
  }

  template< class F >
  void breadth(DataTree& tree, F& f)
  {
    tree.traverse_breadth(f);
  }

  template< class F >
  void getCommands(CommandTree& commands, DataTree& tree, F& f)
  {
    commands.insert(std::make_pair("ascending", [&tree, &f]() { ascending(tree, f); }));
    commands.insert(std::make_pair("descending", [&tree, &f]() { descending(tree, f); }));
    commands.insert(std::make_pair("breadth", [&tree, &f]() { breadth(tree, f); }));
  }
}

#endif
