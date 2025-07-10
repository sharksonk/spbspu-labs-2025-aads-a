#include "commands.hpp"
#include "array.hpp"
#include <string>

void smirnov::printCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName;
  in >> dictName;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }
  const Dict & dict = dictIt->second;
  if (dict.empty())
  {
    out << dictName << " is empty.\n";
    return;
  }
  for (auto it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << it->first << " - ";
    const Array & translations = it->second;
    for (std::size_t i = 0; i < translations.size(); ++i)
    {
      out << translations[i];
      if (i + 1 < translations.size())
      {
        out << " ";
      }
    }
    out << "\n";
  }
}

void smirnov::addCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word, translation;
  in >> dictName >> word >> translation;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  auto dictIt = dicts.find(dictName);
  if (dictIt == dicts.end())
  {
    out << "The dictionary with name " << dictName << " doesn't exist.\n";
    return;
  }

  Dict & dict = dictIt->second;
  auto it = dict.begin();
  for (; it != dict.end(); ++it)
  {
    if (it->first == word)
    {
      Array & translations = it->second;
      for (std::size_t i = 0; i < translations.size(); ++i)
      {
        if (translations[i] == translation)
        {
          out << "The word " << word << " already exists in " << dictName << "\n";
          return;
        }
      }
      translations.push_back(translation);
      return;
    }
  }
  Array arr;
  arr.push_back(translation);
  dict[word] = arr;
  // По ТЗ — если слово добавлено впервые, ничего не выводим
}
