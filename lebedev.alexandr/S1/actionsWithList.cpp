#include "actionsWithList.hpp"
#include <ostream>

void lebedev::printList(const lebedev::List< std::string >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.constBegin();
  output << *current;
  ++current;

  for (; current != list.constEnd(); current++)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

void lebedev::printList(const lebedev::List< int >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.constBegin();
  output << *current;
  ++current;

  for (; current != list.constEnd(); ++current)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

using namespace lebedev;
List< List< int > > createReorderedList(const List< std::pair< std::string, List< int > > >& List)
{
  lebedev::List< lebedev::List< int > > reordered;
  size_t maxSize = 0;
  for (auto seq = List.constBegin(); seq != List.constEnd(); seq++)
  {
    if ((*seq).second.size() > maxSize)
    {
      maxSize = (*seq).second.size();
    }
  }
  for (size_t i = 0; i < maxSize; i++)
  {
    lebedev::List< int > seq;
    reordered.push_back(seq);
  }
  for (auto pair = List.constBegin(); pair != List.constEnd(); pair++)
  {
    auto it = reordered.begin();
    for (auto i = (*pair).second.constBegin(); i != (*pair).second.constEnd(); i++)
    {
      (*it).push_back(*i);
      if (it != reordered.end())
      {
        ++it;
      }
    }
  }
  return reordered;
}

std::pair< lebedev::List< int >, bool > lebedev::createListOfSum(const lebedev::List< lebedev::List< int > >& list)
{
  bool isOverflow = false;
  lebedev::List< int > listOfSum;
  for (auto seq = list.constBegin(); seq != list.constEnd(); seq++)
  {
    int res = 0;
    for (auto current = seq->constBegin(); current != seq->constEnd(); current++)
    {
      if ((res + (*current)) < res)
      {
        isOverflow = true;
      }
      res += (*current);
    }
    listOfSum.push_back(res);
  }
  return { listOfSum, isOverflow };
}
