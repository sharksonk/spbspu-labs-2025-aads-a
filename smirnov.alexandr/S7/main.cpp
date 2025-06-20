#include <iostream>
#include <string>
#include "hashTable.hpp"

int main()
{
  smirnov::HashTable< int, std::string > table;
  std::cout << "Size: " << table.size() << "\n";
  std::cout << "Empty: " << (table.empty() ? "true" : "false") << "\n";
  std::cout << "Load factor: " << table.load_factor() << "\n";
}
