#include <iostream>
#include <string>
#include <list>

int main()
{
  std::list< std::pair< std::string, std::list< unsigned int > > > listOfPairs;
  std::string nameList;
  while (std::cin >> nameList)
  {
    std::list< unsigned int > numList;
    unsigned int num = 0;
    while (std::cin >> num)
    {
      numList.push_back(num);
    }
    listOfPairs.push_back(std::make_pair(nameList, numList));
    std::cin.clear();
  }
}
