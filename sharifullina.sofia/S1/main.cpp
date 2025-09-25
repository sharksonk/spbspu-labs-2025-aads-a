#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include <iterator>
#include "list.hpp"

namespace {
  using listOfPairs = sharifullina::List< std::pair< std::string, sharifullina::List< unsigned long long > > >;

  size_t getMaxListSize(const listOfPairs & lists)
  {
    size_t maxSize = 0;
    for (const auto & pair : lists)
    {
      if (pair.second.size() > maxSize)
      {
        maxSize = pair.second.size();
      }
    }
    return maxSize;
  }

  void printNames(std::ostream& out, const listOfPairs & lists)
  {
    if (lists.empty())
    {
      return;
    }
    bool isFirst = true;
    for (const auto & pair : lists)
    {
      if (!isFirst)
      {
        out << " ";
      }
      out << pair.first;
      isFirst = false;
    }
    out << "\n";
  }

  void printTransposed(std::ostream & out, const listOfPairs & lists)
  {
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      bool isFirstInRow = true;
      for (const auto & pair : lists)
      {
        if (i < pair.second.size())
        {
          auto it = pair.second.cbegin();
          std::advance(it, i);
          if (!isFirstInRow)
          {
            out << " ";
          }
          out << *it;
          isFirstInRow = false;
        }
      }
      out << "\n";
    }
  }

  void addWithCheck(unsigned long long & sum, unsigned long long value)
  {
    if (std::numeric_limits< unsigned long long >::max() - value < sum)
    {
      throw std::overflow_error("overflow");
    }
    sum += value;
  }

  sharifullina::List< unsigned long long > calculateSums(const listOfPairs & lists)
  {
    sharifullina::List< unsigned long long > sums;
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      unsigned long long rowSum = 0;
      bool hasValue = false;
      for (const auto& pair : lists)
      {
        if (i < pair.second.size())
        {
          auto it = pair.second.cbegin();
          std::advance(it, i);
          addWithCheck(rowSum, *it);
          hasValue = true;
        }
      }
      if (hasValue)
      {
        sums.pushBack(rowSum);
      }
    }
    return sums;
  }

  void printSums(std::ostream & out, const sharifullina::List< unsigned long long > & sums)
  {
    if (sums.empty())
    {
      return;
    }
    bool isFirst = true;
    for (const auto & sum : sums)
    {
      if (!isFirst)
      {
        out << " ";
      }
      out << sum;
      isFirst = false;
    }
    out << "\n";
  }
}

int main()
{
  using namespace sharifullina;

  try
  {
    listOfPairs sequences;
    std::string name;
    while (std::cin >> name)
    {
      List< unsigned long long > numbers;
      unsigned long long number;
      while (std::cin >> number)
      {
        numbers.pushBack(number);
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      sequences.pushBack(std::make_pair(name, numbers));
    }

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    printNames(std::cout, sequences);
    printTransposed(std::cout, sequences);

    List< unsigned long long > sums = calculateSums(sequences);
    printSums(std::cout, sums);

    return 0;
  }
  catch (const std::overflow_error & e)
  {
    std::cerr << "Formed lists with exit code 1 and error message in standard error because of " << e.what() << "\n";
    return 1;
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
