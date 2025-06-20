#include "commands.hpp"

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
  auto it = dictionary.find(name);
  if (it == dictionary.cend())
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  const auto& dataset = it->second;
  if (dataset.empty())
  {
    out << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto jt = dataset.cbegin(); jt != dataset.cend(); ++jt)
  {
    out << " " << jt->first << " " << jt->second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  if (dictionary.count(name1) == 0 || dictionary.count(name2) == 0)
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  dataset_t dataset;
  const dataset_t& it1 = dictionary.at(name1);
  const dataset_t& it2 = dictionary.at(name2);
  for (auto it = it1.cbegin(); it != it1.cend(); ++it)
  {
    if (it2.find(it->first) == it2.cend())
    {
      dataset.insert({ it->first, it->second });
    }
  }
  for (auto it = it2.cbegin(); it != it2.cend(); ++it)
  {
    if (it1.find(it->first) == it1.cend())
    {
      dataset.insert({ it->first, it->second });
    }
  }
  try
  {
    dictionary.at(newName) = dataset;
  }
  catch (const std::out_of_range&)
  {
    dictionary.insert(std::make_pair(newName, dataset));
  }
}

void kushekbaev::intersect(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  if (dictionary.count(name1) == 0 || dictionary.count(name2) == 0)
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  dataset_t dataset;
  const dataset_t& it1 = dictionary.at(name1);
  const dataset_t& it2 = dictionary.at(name2);
  for (auto it = it1.cbegin(); it != it1.cend(); ++it)
  {
    if (it2.find(it->first) != it2.cend())
    {
      dataset.insert({ it->first, it->second });
    }
  }
  try
  {
    dictionary.at(newName) = dataset;
  }
  catch (const std::out_of_range&)
  {
    dictionary.insert(std::make_pair(newName, dataset));
  }
}

void kushekbaev::unification(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  if (dictionary.count(name1) == 0 || dictionary.count(name2) == 0)
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  const dataset_t& it1 = dictionary.at(name1);
  const dataset_t& it2 = dictionary.at(name2);
  dataset_t dataset;
  for (auto it = it1.cbegin(); it != it1.cend(); ++it)
  {
    dataset.insert({ it->first, it->second });
  }
  for (auto it = it2.cbegin(); it != it2.cend(); ++it)
  {
    if (dataset.find(it->first) == dataset.end())
    {
      dataset.insert({ it->first, it->second });
    }
  }
  try
  {
    dictionary.at(newName) = dataset;
  }
  catch (const std::out_of_range&)
  {
    dictionary.insert(std::make_pair(newName, dataset));
  }
}
