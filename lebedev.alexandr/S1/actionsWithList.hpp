#ifndef ACTIONSWITHLIST_HPP
#define ACTIONSWITHLIST_HPP
#include <list>
#include <sstream>
#include <string>

namespace lebedev
{
  void printList(const std::list< std::string >& list, std::ostream& output);
  void printList(const std::list< int >& list, std::ostream& output);
  std::list< std::list< int > > createReorderedList(const std::list< std::pair< std::string, std::list< int > > >& List);
  std::pair< std::list< int >, bool > createListOfSum(const std::list< std::list< int > >& list);
}
#endif
