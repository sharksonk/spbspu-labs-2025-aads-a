#include "commandProcessor.hpp"
#include "commands.hpp"
#include <map>
#include <functional>

void shramko::processCommand(TreeOfTrees & trees, const std::string & cmd, std::istream & in, std::ostream & out)
{
  std::map< std::string, std::function< void(TreeOfTrees&, std::istream&, std::ostream&) > > commandMap;
  commandMap["print"] = print;
  commandMap["complement"] = complement;
  commandMap["intersect"] = intersect;
  commandMap["union"] = unite;
  commandMap.at(cmd)(trees, in, out);
}
