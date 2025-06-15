#include "commands.hpp"

using dict_t = std::map< size_t, std::string >;
using dataset_t = std::map< std::string, dict_t >;

namespace
{
  void checkRepeats(std::istream& in, const dataset_t& dataset, dataset_t& ds1, dataset_t& ds2)
  {
    std::string name1, name2;
    in >> name1 >> name2;

    auto it1 = dataset.find(name1);
    auto it2 = dataset.find(name2);

    if (it1 == dataset.end() || it2 == dataset.end())
    {
      throw std::logic_error("<INVALID COMMAND>");
    }
    // Очищаем и заполняем ds1 и ds2, сохраняя структуру
    ds1.clear();
    ds2.clear();
    ds1[name1] = it1->second;  // Сохраняем с тем же ключом
    ds2[name2] = it2->second;
  }
}

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
  for (auto dict_it = it->second.begin(); dict_it != it->second.end(); ++dict_it)
  {
    out << " " << dict_it->first << " " << dict_it->second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, const dataset_t& dataset)
{
  dict_t resultDict;
  dataset_t ds1;
  dataset_t ds2;
  checkRepeats(in, dataset, ds1, ds2);
  const dict_t& dict1 = ds1.begin()->second;
  const dict_t& dict2 = ds2.begin()->second;

  for (auto it = dict1.begin(); it != dict1.end(); ++it)
  {
    if (dict2.find(it->first) == dict2.end())
    {
      resultDict.insert(*it);
    }
  }
}

void kushekbaev::intersect(std::istream& in, const dataset_t& dataset)
{
  dict_t resultDict;
  dataset_t ds1;
  dataset_t ds2;
  checkRepeats(in, dataset, ds1, ds2);
  const dict_t& dict1 = ds1.begin()->second;
  const dict_t& dict2 = ds2.begin()->second;

  auto it1 = dict1.begin();
  auto it2 = dict2.begin();

  while (it1 != dict1.end() && it2 != dict2.end())
  {
    if (it1->first < it2->first)
    {
      ++it1;
    }
    else if (it2->first < it1->first)
    {
      ++it2;
    }
    else
    {
      resultDict.insert(*it1);
      ++it1;
      ++it2;
    }
  }
}

void kushekbaev::unification(std::istream& in, const dataset_t& dataset)
{
  dataset_t ds1;
  dataset_t ds2;
  checkRepeats(in, dataset, ds1, ds2);
  dict_t resultDict = ds1.begin()->second;
  const dict_t& dict2 = ds2.begin()->second;
  for (auto it = dict2.begin(); it != dict2.end(); ++it)
  {
    resultDict[it->first] = it->second;
  }
}
