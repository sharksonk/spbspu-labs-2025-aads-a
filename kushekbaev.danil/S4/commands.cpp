#include "commands.hpp"

using dataset_t = std::map< size_t, std::string >;
using dict_t = std::map< std::string, dataset_t >;

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
  if (it == dictionary.end())
  {
    throw std::out_of_range("<INVALID COMMAND>");
  }
  out << name;
  const auto& dataset = it->second;
  if (dataset.empty())
  {
    out << "<EMPTY>";
    return;
  }
  for (auto jt = dataset.begin(); jt != dataset.end(); ++jt)
  {
    out << " " << jt->first << " " << jt->second;
  }
  out << "\n";
}

void kushekbaev::complement(std::istream& in, dict_t& dictionary)
{
  std::string newName, name1, name2;
  in >> newName >> name1 >> name2;
  dataset_t dataset;
  const dataset_t it1 = dictionary.at(name1);
  const dataset_t it2 = dictionary.at(name2);
  for (auto it = it1.begin(); it != it1.end(); ++it)
  {
    if (it2.find(it->first) == it2.end())
    {
      dataset.insert({ it->first, it->second });
    }
  }
  for (auto it = it2.begin(); it != it2.end(); ++it)
  {
    if (it1.find(it->first) == it1.end())
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
  dataset_t dataset;
  const dataset_t it1 = dictionary.at(name1);
  const dataset_t it2 = dictionary.at(name2);
  for (auto it = it1.begin(); it != it1.end(); ++it)
  {
    if (it2.find(it->first) != it2.end())
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
  const dataset_t it1 = dictionary.at(name1);
  const dataset_t it2 = dictionary.at(name2);
  dataset_t dataset;
  for (auto it = it1.begin(); it != it1.end(); ++it)
  {
    dataset.insert({ it->first, it->second });
  }
  for (auto it = it2.begin(); it != it2.end(); ++it)
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
