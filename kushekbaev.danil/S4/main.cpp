#include <fstream>
#include <limits>
#include <stdexcept>
#include "commands.hpp"

using dict_t = std::map< size_t, std::string >;
using dataset_t = std::map< std::string, dict_t >;

namespace
{
  dataset_t readInputFromFile(std::ifstream& in)
  {
    if (!in.is_open())
    {
      throw std::runtime_error("File couldn't be open!");
    }
    std::string name;
    dataset_t dataset;
    while (std::getline(in, name))
    {
      size_t key;
      std::string value;
      dict_t dictionary;
      while (in >> key >> value)
      {
        dictionary[key] = value;
      }
      dataset[name] = dictionary;
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    return dataset;
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
  dataset_t dataset = readInputFromFile(in);
  std::map< std::string, std::function< void() > > commands;
  commands["print"] = std::bind(kushekbaev::print, std::ref(std::cout), std::ref(std::cin), std::cref(dataset));
  commands["complement"] = std::bind(kushekbaev::complement, std::ref(std::cin), std::ref(dataset));
  commands["intersect"] = std::bind(kushekbaev::intersect, std::ref(std::cin), std::ref(dataset));
  commands["union"] = std::bind(kushekbaev::unification, std::ref(std::cin), std::ref(dataset));
  std::string command;
  while (!(std::cin >> command).eof())
  {
    try
    {
      commands.at(command)();
    }
    catch (const std::logic_error&)
    {
      std::cerr << "<INVALID COMMAND>";
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << "\n";
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}
