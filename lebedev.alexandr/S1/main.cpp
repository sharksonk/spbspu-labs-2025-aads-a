#include <iostream>
#include <string>
#include "actionsWithList.hpp"
#include "list.hpp"

int main()
{
  lebedev::List< std::pair< std::string, lebedev::List< int > > > sequences;
  std::string name;

  while (std::cin >> name)
  {
    lebedev::List< int > numbers;
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
  lebedev::List< std::string > names;
  for (auto seq : sequences)
  {
    names.push_back(seq.first);
  }
  lebedev::printList(names, std::cout);
  lebedev::List< lebedev::List< int > > reorderedList = lebedev::createReorderedList(sequences);
  for (const auto& seq : reorderedList)
  {
    lebedev::printList(seq, std::cout);
  }
  std::pair< lebedev::List< int >, bool > listOfSum = lebedev::createListOfSum(reorderedList);
  if (listOfSum.second)
  {
    std::cerr << "Is Overflow";
    return 1;
  }
  lebedev::printList(listOfSum.first, std::cout);
}
