#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include <map>

using dataset_t = std::map< size_t, std::string >;
using dict_t = std::map< std::string, dataset_t >;

namespace kushekbaev
{
  void print(std::ostream& out, std::istream& in, const dict_t& dataset);

  void complement(std::istream& in, dict_t& dataset);

  void intersect(std::istream& in, dict_t& dataset);

  void unification(std::istream& in, dict_t& dataset);
}

#endif
