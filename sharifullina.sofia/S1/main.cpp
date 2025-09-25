#include <iostream>
#include <string>
#include <limits>
#include "list.hpp"

namespace
{
  using listOfPairs = sharifullina::List< std::pair< std::string, sharifullina::List< unsigned long long > > >;

  size_t getMaxListSize(const listOfPairs& lists)
  {
    size_t maxSize = 0;
    for (auto it = lists.begin(); it != lists.end(); ++it)
    {
      if (it->second.size() > maxSize)
      {
        maxSize = it->second.size();
      }
    }
    return maxSize;
  }

  void printNames(std::ostream& out, const listOfPairs& lists)
  {
    if (lists.empty()) return;
    auto it = lists.begin();
    out << it->first;
    ++it;
    for (; it != lists.end(); ++it)
    {
      out << " " << it->first;
    }
    out << "\n";
  }

  void printTransposed(std::ostream& out, const listOfPairs& lists)
  {
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      bool firstInRow = true;
      for (auto listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          auto numIt = listIt->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }
          if (!firstInRow)
          {
            out << " ";
          }
          out << *numIt;
          firstInRow = false;
        }
        else if (!firstInRow)
        {
          out << " ";
        }
      }
      out << "\n";
    }
  }

  void addWithCheck(unsigned long long& sum, unsigned long long value)
  {
    if (std::numeric_limits< unsigned long long >::max() - value < sum)
    {
      throw std::overflow_error("Overflow detected in sum calculation");
    }
    sum += value;
  }

  sharifullina::List< unsigned long long > calculateSums(const listOfPairs& lists)
  {
    sharifullina::List< unsigned long long > sums;
    size_t maxSize = getMaxListSize(lists);
    for (size_t i = 0; i < maxSize; ++i)
    {
      unsigned long long rowSum = 0;
      bool hasValue = false;
      for (auto listIt = lists.begin(); listIt != lists.end(); ++listIt)
      {
        if (i < listIt->second.size())
        {
          auto numIt = listIt->second.begin();
          for (size_t j = 0; j < i; ++j)
          {
            ++numIt;
          }
          addWithCheck(rowSum, *numIt);
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

  void printSums(std::ostream& out, const sharifullina::List< unsigned long long >& sums)
  {
    if (sums.empty())
    {
      out << "0\n";
      return;
    }
    auto it = sums.begin();
    out << *it;
    ++it;
    for (; it != sums.end(); ++it)
    {
      out << " " << *it;
    }
    out << "\n";
  }

  bool safeStringToULL(const std::string& str, unsigned long long& result)
  {
    result = 0;
    for (char c : str) {
      if (c < '0' || c > '9') return false;
      unsigned long long digit = c - '0';
      if (result > (std::numeric_limits<unsigned long long>::max() - digit) / 10) {
        return false;
      }
      result = result * 10 + digit;
    }
    return true;
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
      std::string numStr;
      while (std::cin >> numStr)
      {
        unsigned long long number;
        if (safeStringToULL(numStr, number)) {
          numbers.pushBack(number);
        } else {
          std::cin.putback(' ');
          for (auto it = numStr.rbegin(); it != numStr.rend(); ++it) {
            std::cin.putback(*it);
          }
          break;
        }
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
  catch (const std::overflow_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
