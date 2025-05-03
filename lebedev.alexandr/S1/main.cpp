#include <list>
#include <iostream>
#include <string>
#include "actionsWithList.hpp"

int main()
{
  std::list< std::pair< std::string, std::list< int > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    std::list< int > numbers;
    int num = 0;
    while (std::cin >> num)
    {
      numbers.push_back(num);
    }
    sequences.push_back(std::make_pair(name, numbers));
    if (std::cin.eof())
    {
      break;
    }
    std::cin.clear();
  }

  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  std::list< std::string > names;
  for (auto seq : sequences)
  {
    names.push_back(seq.first);
  }
  lebedev::printList(names, std::cout);
  std::list< std::list< int > > reorderedList = lebedev::createReorderedList(sequences);
  for (const auto& seq : reorderedList)
  {
    lebedev::printList(seq, std::cout);
  }
  std::pair< std::list< int >, bool > listOfSum = lebedev::createListOfSum(reorderedList);
  if (listOfSum.second)
  {
    std::cerr << "Is Overflow";
    return 1;
  }
  lebedev::printList(listOfSum.first, std::cout);
}
