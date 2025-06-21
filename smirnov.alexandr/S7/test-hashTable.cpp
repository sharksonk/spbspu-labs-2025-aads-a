#include <boost/test/unit_test.hpp>
#include "hashTable.hpp"

BOOST_AUTO_TEST_CASE(test_insert_and_find)
{
  smirnov::HashTable< int, std::string > table;
  auto [it1, success1] = table.insert(1, "one");
  BOOST_TEST(success1);
  BOOST_TEST(it1->second == "one");
  auto [it2, success2] = table.insert(1, "uno");
  BOOST_TEST(!success2);
  BOOST_TEST(it2->second == "one");
  auto it3 = table.find(1);
  BOOST_TEST(it3 != table.end());
  BOOST_TEST(it3->second == "one");
  auto it4 = table.find(2);
  BOOST_TEST(it4 == table.end());
}

BOOST_AUTO_TEST_CASE(test_erase)
{
  smirnov::HashTable< int, std::string > table;
  table.insert(1, "one");
  table.insert(2, "two");
  size_t erasedCount1 = table.erase(1);
  BOOST_TEST(erasedCount1 == 1);
  auto it = table.find(1);
  BOOST_TEST(it == table.end());
  size_t erasedCount2 = table.erase(1);
  BOOST_TEST(erasedCount2 == 0);
  size_t erasedCount3 = table.erase(3);
  BOOST_TEST(erasedCount3 == 0);
}

BOOST_AUTO_TEST_CASE(test_load_factor_and_rehash)
{
  smirnov::HashTable< int, std::string > table;
  for (int i = 0; i < 100; ++i)
  {
    table.insert(i, "value");
  }
  float loadFactor = table.load_factor();
  BOOST_TEST(loadFactor <= 0.75);
  table.rehash(200);
  BOOST_TEST(table.size() <= 200);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
  smirnov::HashTable< int, std::string > table;
  table.insert(1, "one");
  table.insert(2, "two");
  table.insert(3, "three");
  size_t count = 0;
  for (auto it = table.begin(); it != table.end(); ++it)
  {
    BOOST_TEST(it->second == "one" || it->second == "two" || it->second == "three");
    ++count;
  }
  BOOST_TEST(count == 3);
}
