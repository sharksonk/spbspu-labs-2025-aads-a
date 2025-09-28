#include "commands.hpp"
#include <string>
#include "array.hpp"

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
    out << it->first << " -";
    const Array & translations = it->second;
    for (std::size_t i = 0; i < translations.size(); ++i)
    {
      out << " " << translations[i];
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
  auto wordIt = dict.find(word);
  if (wordIt != dict.end())
  {
    Array & translations = wordIt->second;
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
  Array translations;
  translations.push_back(translation);
  dict.insert(word, translations);
}

void smirnov::translateCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
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
  auto wordIt = dict.find(word);
  if (wordIt == dict.cend())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  const Array & translations = wordIt->second;
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

void smirnov::removeCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string dictName, word;
  in >> dictName >> word;
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
  auto wordIt = dict.find(word);
  if (wordIt == dict.end())
  {
    out << "The word " << word << " doesn't exist in " << dictName << "\n";
    return;
  }
  dict.erase(wordIt);
  out << "The word " << word << " successfully deleted from " << dictName << "\n";
}

void smirnov::createCommand(Dicts & dicts, std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (dicts.find(name) != dicts.end())
  {
    out << "The dictionary with name " << name << " already exists.\n";
    return;
  }
  Dict dict;
  dicts.insert(name, dict);
  out << name << " is successfully created.\n";
}
