#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include <map>
#include "UBST.hpp"

using dataset_t = std::map< size_t, std::string >; //ЭТО СТРОКА!!
using dict_t = std::map< std::string, dataset_t >; //ЭТО СЛОВАРЬ ИЗ СТРОК!!

namespace kushekbaev
{
  void print(std::ostream& out, std::istream& in, const dict_t& dataset);

  void complement(std::istream& in, dict_t& dataset);

  void intersect(std::istream& in, dict_t& dataset);

  void unification(std::istream& in, dict_t& dataset);
}

#endif
