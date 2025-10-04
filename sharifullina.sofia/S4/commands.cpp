#include "commands.hpp"
#include <string>
#include <iostream>

void sharifullina::printCMD(std::ostream& out, const mainDict& dict)
{
  std::string name = "";
  std::cin >> name;
  const sharifullina::AVLtree< int, std::string >& current = dict.at(name);
  if (current.empty())
  {
    std::cout << "<EMPTY>\n";
    return;
  }
  out << name;
  for (auto i = current.cbegin(); i != current.cend(); ++i)
  {
    out << " " << (*i).first << " " << (*i).second;
  }
  out << "\n";
}

void sharifullina::complementCMD(mainDict& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  sharifullina::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    int key = (*i).first;
    if (dict2.find(key) == dict2.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}

void sharifullina::intersectCMD(mainDict& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  sharifullina::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    int key = (*i).first;
    if (dict2.find(key) != dict2.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}

void sharifullina::unionCMD(mainDict& dict)
{
  std::string name, dictName1, dictName2;
  std::cin >> name >> dictName1 >> dictName2;
  auto dict1 = dict.at(dictName1);
  auto dict2 = dict.at(dictName2);
  sharifullina::AVLtree< int, std::string > temp;
  for (auto i = dict1.begin(); i != dict1.end(); ++i)
  {
    temp.insert(*i);
  }
  for (auto i = dict2.begin(); i != dict2.end(); ++i)
  {
    int key = (*i).first;
    if (dict1.find(key) == dict1.end())
    {
      temp.insert(*i);
    }
  }
  dict[name] = temp;
}
