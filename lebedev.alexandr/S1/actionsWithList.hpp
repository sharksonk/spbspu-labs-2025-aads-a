#ifndef ACTIONSWITHLIST_HPP
#define ACTIONSWITHLIST_HPP
#include <sstream>
#include <string>
#include "list.hpp"

namespace lebedev
{
  void printList(const List< std::string >& list, std::ostream& output);
  void printList(const List< int >& list, std::ostream& output);
  List< List< int > > createReorderedList(const List< std::pair< std::string, List< int > > >& List);
  std::pair< List< int >, bool > createListOfSum(const List< List< int > >& list);
}
#endif
