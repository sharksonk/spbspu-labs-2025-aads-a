#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <functional>
#include "inputDict.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "empty parameter\n";
    return 1;
  }
  std::string filename = argv[1];
  std::ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    std::cout << "error open file: " << filename << "\n";
    return 1;
  }
  sharifullina::AVLtree< std::string, sharifullina::AVLtree< int, std::string > > mainDict;
  try
  {
    sharifullina::inDictionaries(inputFile, mainDict);
  }
  catch (...)
  {
    std::cout << "error input!\n";
    return 1;
  }
  sharifullina::AVLtree< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(sharifullina::printCMD, std::ref(std::cout), std::cref(mainDict));
  commands["complement"] = std::bind(sharifullina::complementCMD, std::ref(mainDict));
  commands["intersect"] = std::bind(sharifullina::intersectCMD, std::ref(mainDict));
  commands["union"] = std::bind(sharifullina::unionCMD, std::ref(mainDict));
  std::string command = "";
  while (std::cin >> command)
  {
    try
    {
      auto func = commands.find(command);
      if (func != commands.end())
      {
        func->second();
      }
      else
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
