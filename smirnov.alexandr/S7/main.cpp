#include <iostream>
#include <string>
#include "hashTable.hpp"

int main()
{
  smirnov::HashTable< int, std::string > table;

    // Вставка элементов с проверкой результата
    if (!table.insert(1, "one").second) {
        std::cout << "Failed to insert key 1\n";
    }
    if (!table.insert(2, "two").second) {
        std::cout << "Failed to insert key 2\n";
    }

    // Проверка find
    auto it = table.find(1);
    if (it != table.end()) {
        std::cout << "Found: " << it->first << " => " << it->second << "\n";
    }

    // Проверка повторной вставки
    auto insert_result = table.insert(1, "new_one");
    if (!insert_result.second) {
        std::cout << "Key 1 already exists with value: "
                  << insert_result.first->second << "\n";
    }

    // Проверка поиска несуществующего ключа
    if (table.find(42) == table.end()) {
        std::cout << "Key 42 not found (correct behavior)\n";
    }

    // Вывод размера
    std::cout << "Size: " << table.size() << "\n";
    std::cout << "Load factor: " << table.load_factor() << "\n";
}
