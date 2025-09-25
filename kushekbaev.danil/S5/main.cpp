#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <UBST.hpp>
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Wrong number of elements!\n";
    return 1;
  }
  std::ifstream in(argv[1]);
  kushekbaev::UBST< std::string, std::function< void() > > commands;
  commands["ascending"] = std::bind(kushekbaev::ascending);
  commands["descending"] = std::bind(kushekbaev::descending);
  commands["breadth"] = std::bind(kushekbaev::breadth);
}