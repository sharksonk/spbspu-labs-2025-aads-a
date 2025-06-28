#include "commands.hpp"
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
    const std::vector< std::string > & translations = it->second;
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
