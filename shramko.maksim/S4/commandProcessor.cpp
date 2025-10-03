#include "commandProcessor.hpp"
#include "commands.hpp"
#include <stdexcept>

namespace shramko
{
  void processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out)
  {
    if (cmd == "print")
    {
      print(trees, in, out);
      return;
    }
    using CommandFunc = void(*)(TreeOfTrees&, std::istream&, std::ostream&);
    static UBstTree< std::string, CommandFunc > commandMap;
    if (commandMap.empty())
    {
      commandMap["complement"] = complement;
      commandMap["intersect"] = intersect;
      commandMap["union"] = unite;
    }
    auto map_it = commandMap.find(cmd);
    if (map_it == commandMap.cend())
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    try
    {
      map_it->second(trees, in, out);
    }
    catch (const std::exception&)
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}
