#include <fstream>
#include <limits>
#include <stdexcept>
#include <functional>
#include "commands.hpp"

using dataset_t = std::map< size_t, std::string >; //ЭТО СТРОКА!!
using dict_t = std::map< std::string, dataset_t >; //ЭТО СЛОВАРЬ ИЗ СТРОК!!

namespace
{
  dict_t readInputFromFile(std::ifstream& in)
  {
    if (!in.is_open())
    {
      throw std::runtime_error("File couldn't be open!");
    }
    dict_t inputdata;
    while (!in.eof())
    {
      std::string name;
      in >> name;
      size_t key = 0;
      std::string value;
      dataset_t tmp;
      char c = 0;
      while (in.get(c) && c != '\n')
      {
        in >> key >> value;
        tmp.insert(std::make_pair(key, value));
      }
      inputdata.insert(std::make_pair(name, std::move(tmp)));
    }
    return inputdata;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "<WRONG NUMBER OF ARGUMENTS>";
    return 1;
  }
  std::ifstream in(argv[1]);
  dict_t dictionary = readInputFromFile(in);
  std::map< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(kushekbaev::print, std::ref(std::cout), std::ref(std::cin), std::cref(dictionary));
  commands["complement"] = std::bind(kushekbaev::complement, std::ref(std::cin), std::ref(dictionary));
  commands["intersect"] = std::bind(kushekbaev::intersect, std::ref(std::cin), std::ref(dictionary));
  commands["union"] = std::bind(kushekbaev::unification, std::ref(std::cin), std::ref(dictionary));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>";
      return 1;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  return 0;
}
