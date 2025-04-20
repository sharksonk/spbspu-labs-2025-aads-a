#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <limits>
#include "list.hpp"

int main()
{
  using namespace hismatova;
  std::vector< std::pair< std::string, List< int > > > sequences;
  std::string name;
  bool overflow_occurred = false;
  while (std::cin >> name)
  {
    List< int > numbers;
    int summa = 0;
    std::string line;
    std::getline(std::cin, line);
    if (line.empty())
    {
      line = "";
    }
    std::istringstream iss(line);
    unsigned long long num = 0;
    while (iss >> num)
    {
      if (num > static_cast<unsigned long long>(std::numeric_limits<int>::max()) ||
          summa > std::numeric_limits<int>::max() - static_cast<int>(num))
      {
        overflow_occurred = true;
        break;
      }
      summa += static_cast< int >(num);
      numbers.push_back(static_cast< int >(num));
    }
    sequences.emplace_back(name, numbers);
    if (overflow_occurred)
    {
      break;
    }
  }
  if (overflow_occurred)
  {
    std::cout << "Formed lists with exit code 1 and error message in standard error because of overflow";
    return 1;
  }
  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  for (size_t i = 0; i < sequences.size(); ++i)
  {
    std::cout << sequences[i].first;
    if (i + 1 < sequences.size())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  size_t max_length = 0;
  for (const auto& seq : sequences)
  {
    max_length = std::max(max_length, seq.second.size());
  }
  std::vector< std::vector< int > > vertical_sequences(max_length);
  for (size_t i = 0; i < max_length; ++i)
  {
    for (const auto& seq : sequences)
    {
      if (i < seq.second.size())
      {
        auto it = seq.second.begin();
        for (size_t j = 0; j < i; ++j) ++it;
        vertical_sequences[i].push_back(*it);
      }
    }
  }
  for (const auto& v_seq : vertical_sequences)
  {
    for (size_t j = 0; j < v_seq.size(); ++j)
    {
      std::cout << v_seq[j];
      if (j + 1 < v_seq.size())
      {
        std::cout << " ";
      }
    }
    std::cout << "\n";
  }

  std::vector< int > sums;
  for (const auto& v_seq : vertical_sequences)
  {
    if (!v_seq.empty())
    {
      int sum = std::accumulate(v_seq.begin(), v_seq.end(), 0);
      sums.push_back(sum);
    }
  }

  if (sums.empty())
  {
    std::cout << "0\n";
    return 0;
  }

  for (size_t i = 0; i < sums.size(); ++i)
  {
    std::cout << sums[i];
    if (i +1 < sums.size())
    {
      std::cout << " ";
    }
  }
  std::cout << "\n";
  return 0;
}

