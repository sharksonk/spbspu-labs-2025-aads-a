#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <initializer_list>
#include "list.hpp"

int main()
{
  using namespace hismatova;
  std::vector< std::pair< std::string, List< int > > > sequences;
  std::string name;
  int num = 0;
  while (std::cin >> name)
  {
    List< int > numbers;
    while (std::cin.peek() != '\n' && std::cin >> num)
    {
      numbers.push_back(num);
    }
    sequences.emplace_back(name, numbers);
  }
  if (sequences.empty())
  {
    std::cout << "0\n";
    return 0;
  }
  for (const auto& seq: sequences)
  {
    std::cout << seq.first << " ";
  }
  std::cout << "\n";
  size_t max_length = 0;
  for (const auto& seq: sequences)
  {
    max_length = std::max(max_length, seq.second.size());
  }
  std::vector< std::vector< int > > vertical_sequences(max_length);
  for (size_t i = 0; i < max_length; ++i)
  {
    for (const auto& seq: sequences)
    {
      if (i < seq.second.size())
      {
        auto it = seq.second.begin();
        for (size_t j = 0; j < i; ++j)
        {
          ++it;
        }
        vertical_sequences[i].push_back(*it);
      }
    }
  }
  for (const auto& v_seq: vertical_sequences)
  {
    for (int num: v_seq)
    {
      std::cout << num << " ";
    }
    std::cout << "\n";
  }
  std::vector< int > sums;
  for (const auto& v_seq: vertical_sequences)
  {
    if (!v_seq.empty())
    {
      int sum = std::accumulate(v_seq.begin(), v_seq.end(), 0);
      sums.push_back(sum);
    }
  }
  if (sums.size() != vertical_sequences.size())
  {
    std::cerr << "ERROR: empty sequence\n";
    return 1;
  }
  for (int sum: sums)
  {
    std::cout << sum << " ";
  }
  std::cout << "\n";
  return 0;
}
