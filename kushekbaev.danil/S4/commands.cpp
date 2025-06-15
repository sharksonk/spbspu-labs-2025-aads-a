#include "commands.hpp"

using dict_t = std::map< size_t, std::string >;
using dataset_t = std::map< std::string, dict_t >;

void kushekbaev::executeCommand(std::istream& in, std::ostream& out, dataset_t& dataset)
{
  std::map< std::string, std::function< void() > > commands;
  commands["print"] = PrintCommand{ out, in, dataset };
  commands["complement"] = ComplementCommand{ in, dataset };
  commands["intersect"] = IntersectCommand{ in, dataset };
  commands["union"] = UnionCommand{ in, dataset };
  std::string command;
  in >> command;
  commands.at(command)();
}

void kushekbaev::print(std::ostream& out, std::istream& in, const dataset_t& dataset)
{
  std::string name;
  in >> name;
  if (dataset.empty())
  {
    throw std::logic_error("<EMPTY>");
  }
  auto it = dataset.find(name);
  if (it == dataset.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  out << name;
  for (const auto& element: it->second)
  {
    out << " " << element.first << " " << element.second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, dataset_t& dataset)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dataset.find(name1);
  auto it2 = dataset.find(name2);
  if (it1 == dataset.end() || it2 == dataset.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  dict_t resultDict;
  for (const auto& element: it2->second)
  {
    if (it1->second.find(element.first) == it1->second.end())
    {
      resultDict[element.first] = element.second;
    }
  }
  dataset[newName] = resultDict;
}

void kushekbaev::intersect(std::istream& in, dataset_t& dataset)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dataset.find(name1);
  auto it2 = dataset.find(name2);
  if (it1 == dataset.end() || it2 == dataset.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  dict_t resultDict;
  for (const auto& element: it1->second)
  {
    if (it2->second.find(element.first) != it2->second.end())
    {
      resultDict[element.first] = element.second;
    }
  }
  dataset[newName] = resultDict;
}

void kushekbaev::unification(std::istream& in, dataset_t& dataset)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dataset.find(name1);
  auto it2 = dataset.find(name2);
  if (it1 == dataset.end() || it2 == dataset.end())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  dict_t resultDict = it1->second;
  for (const auto& element: it2->second)
  {
    resultDict[element.first] = element.second;
  }
  dataset[newName] = resultDict;
}
