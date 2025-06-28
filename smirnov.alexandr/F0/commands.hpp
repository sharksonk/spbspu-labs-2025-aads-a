#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "dictionary.hpp"

namespace smirnov
{
  void printCommand(Dicts & dicts, std::istream & in, std::ostream & out);
}
#endif
