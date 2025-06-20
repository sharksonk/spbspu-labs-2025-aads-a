#include "commands.hpp"
#include <algorithm>
#include <vector.hpp>

using dataset_t = kushekbaev::UBST< size_t, std::string >; //ЭТО СТРОКА!!
using dict_t = kushekbaev::UBST< std::string, dataset_t >; //ЭТО СЛОВАРЬ ИЗ СТРОК!!

void kushekbaev::print(std::ostream& out, std::istream& in, const dict_t& dictionary)
{
  if (dictionary.empty())
  {
    out << "<EMPTY>";
    return;
  }
  std::string name;
  in >> name;
  dataset_t result = dictionary.at(name);
  if (result.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  kushekbaev::Vector< std::pair < size_t, std::string > > sorted;
  for (const auto& elem: result)
  {
    sorted.push_back(elem);
  }
  std::sort(sorted.begin(), sorted.end());
  out << name;
  for (const auto& elem: sorted)
  {
    out << " " << elem.first << " " << elem.second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dictionary.find(name1);
  auto it2 = dictionary.find(name2);
  if (it1 == dictionary.end() || it2 == dictionary.end())
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  dataset_t result;
  if (name1 == name2)
  {
    auto target = dictionary.find(newName);
    if (target != dictionary.end())
    {
      target->second = result;
    }
    else
    {
      dictionary.insert({newName, result});
    }
    return;
  }
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it)
  {
    if (ds2.find(it->first) == ds2.cend())
    {
      result.insert(*it);
    }
  }
  auto target = dictionary.find(newName);
  if (target != dictionary.end())
  {
    target->second = result;
  }
  else
  {
    dictionary.insert({newName, result});
  }
}

void kushekbaev::intersect(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dictionary.find(name1);
  auto it2 = dictionary.find(name2);
  if (it1 == dictionary.end() || it2 == dictionary.end())
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  dataset_t result;
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  if (ds1.empty() || ds2.empty())
  {
    auto target = dictionary.find(newName);
    if (target != dictionary.end())
    {
      target->second = result;
    }
    else
    {
      dictionary.insert({newName, result});
    }
    return;
  }
  if (name1 == name2)
  {
    result = ds1;
  }
  else
  {
    const dataset_t& smaller = (ds1.size() < ds2.size()) ? ds1 : ds2;
    const dataset_t& larger = (ds1.size() < ds2.size()) ? ds2 : ds1;

    for (const auto& elem : smaller)
    {
      if (larger.find(elem.first) != larger.end())
      {
        result.insert(elem);
      }
    }
  }
  auto target = dictionary.find(newName);
  if (target != dictionary.end())
  {
    target->second = result;
  }
  else
  {
    dictionary.insert({newName, result});
  }
}

void kushekbaev::unification(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  auto it1 = dictionary.find(name1);
  auto it2 = dictionary.find(name2);
  if (it1 == dictionary.end() || it2 == dictionary.end())
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  const dataset_t ds1 = dictionary.at(name1);
  const dataset_t ds2 = dictionary.at(name2);
  dataset_t result;
  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it)
  {
    result.insert(*it);
  }
  for (auto it = ds2.cbegin(); it != ds2.cend(); ++it)
  {
    if (result.find(it->first) == result.end())
    {
      result.insert(*it);
    }
  }
  auto target = dictionary.find(newName);
  if (target != dictionary.end())
  {
    target->second = result;
  }
  else
  {
    dictionary.insert({newName, result});
  }
}
