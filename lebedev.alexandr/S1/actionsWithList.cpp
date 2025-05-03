#include "actionsWithList.hpp"
#include <ostream>

void lebedev::printList(const std::list< std::string >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.begin();
  output << *current;
  ++current;

  for (; current != list.end(); current++)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

void lebedev::printList(const std::list< int >& list, std::ostream& output)
{
  if (list.empty())
  {
    return;
  }
  auto current = list.begin();
  output << *current;
  ++current;

  for (; current != list.end(); ++current)
  {
    output << ' ' << *current;
  }
  output << '\n';
}

std::list< std::list< int > > lebedev::createReorderedList(const std::list< std::pair< std::string, std::list< int > > >& List)
{
  std::list< std::list< int > > reordered;
  size_t maxSize = 0;
  for (const auto& seq: List)
  {
    maxSize = std::max(maxSize, seq.second.size());
  }
  for (size_t i = 0; i < maxSize; i++)
  {
    std::list< int > seq;
    reordered.push_back(seq);
  }
  for (const auto& pair : List)
  {
    auto it = reordered.begin();
    for (const auto& i : pair.second)
    {
      (*it).push_back(i);
      if (it != reordered.end())
      {
        ++it;
      }
    }
  }
  return reordered;
}

std::pair< std::list< int >, bool > lebedev::createListOfSum(const std::list< std::list< int > >& list)
{
  bool isOverflow = false;
  std::list< int > listOfSum;
  for (const auto& seq : list)
  {
    int res = 0;
    for (const auto& current : seq)
    {
      if ((res + current) < res)
      {
        isOverflow = true;
      }
      res += current;
    }
    listOfSum.push_back(res);
  }
  return { listOfSum, isOverflow };
}
