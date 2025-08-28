#include <boost/test/unit_test.hpp>
#include "bitree.hpp"
#include "avl_tree.hpp"

using namespace karnauhova;

BOOST_AUTO_TEST_SUITE(avl_tree)

BOOST_AUTO_TEST_CASE(constructors)
{
  AvlTree< size_t, std::string > tree;
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.empty());
  
  AvlTree< size_t, std::string > testTree;
  testTree.insert({1, "1"});
  testTree.insert({2, "2"});
  testTree.insert({0, "0"});
  BOOST_TEST(testTree.size() == 3);
}

BOOST_AUTO_TEST_CASE(insert_element)
{
  AvlTree< size_t, std::string > tree;
  
  auto result = tree.insert({1, "1"});
  BOOST_TEST(result.second == true);
  BOOST_TEST((*result.first).first == 1);
  BOOST_TEST((*result.first).second == "1");
  
  result = tree.insert({1, "new1"});
  BOOST_TEST(result.second == false);
  BOOST_TEST(tree.size() == 1);
  
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  BOOST_TEST(tree.size() == 3);
}

BOOST_AUTO_TEST_CASE(erase_iterators)
{
  AvlTree< size_t, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  tree.insert({4, "4"});
  
  BOOST_TEST(tree.size() == 4);
  
  // Удаляем первый элемент
  auto cit = tree.cbegin();
  auto next_it = tree.erase(cit);
  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(next_it == tree.begin());
  
  // Проверяем, что удалился правильный элемент
  BOOST_TEST(tree.find(1) == tree.end());
  BOOST_TEST(tree.find(2) != tree.end());
  BOOST_TEST(tree.find(3) != tree.end());
  BOOST_TEST(tree.find(4) != tree.end());
}

BOOST_AUTO_TEST_CASE(iterators)
{
  AvlTree< size_t, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  
  auto it = tree.begin();
  BOOST_TEST((*it).first == 1);
  BOOST_TEST((*it).second == "1");
  
  ++it;
  BOOST_TEST((*it).first == 2);
  
  ++it;
  BOOST_TEST((*it).first == 3);
  
  ++it;
  BOOST_TEST(it == tree.end());
  
  auto cit = tree.cbegin();
  BOOST_TEST((*cit).first == 1);
  
  auto cend = tree.cend();
  BOOST_TEST(cit != cend);
}

BOOST_AUTO_TEST_CASE(size_and_empty_and_clear)
{
  AvlTree< size_t, std::string > tree;
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
  
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  tree.insert({4, "4"});
  
  BOOST_TEST(!tree.empty());
  BOOST_TEST(tree.size() == 4);
  
  tree.clear();
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.size() == 0);
}

BOOST_AUTO_TEST_CASE(count)
{
  AvlTree< size_t, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  
  BOOST_TEST(tree.count(1) == 1);
  BOOST_TEST(tree.count(2) == 1);
  BOOST_TEST(tree.count(3) == 1);
  BOOST_TEST(tree.count(4) == 0);
}

BOOST_AUTO_TEST_CASE(swap)
{
  AvlTree< size_t, std::string > tree1;
  tree1.insert({1, "1"});
  tree1.insert({2, "2"});
  
  AvlTree< size_t, std::string > tree2;
  tree2.insert({3, "3"});
  tree2.insert({4, "4"});
  
  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree2.size() == 2);
  
  tree1.swap(tree2);
  
  BOOST_TEST(tree1.size() == 2);
  BOOST_TEST(tree2.size() == 2);
  
  BOOST_TEST(tree1.find(3) != tree1.end());
  BOOST_TEST(tree2.find(1) != tree2.end());
}

BOOST_AUTO_TEST_CASE(find)
{
  AvlTree< size_t, std::string > tree;
  tree.insert({1, "1"});
  tree.insert({2, "2"});
  tree.insert({3, "3"});
  
  auto it = tree.find(2);
  BOOST_TEST(it != tree.end());
  BOOST_TEST((*it).first == 2);
  BOOST_TEST((*it).second == "2");
  
  it = tree.find(5);
  BOOST_TEST(it == tree.end());
  
  const auto& const_tree = tree;
  auto cit = const_tree.find(3);
  BOOST_TEST(cit != const_tree.cend());
  BOOST_TEST((*cit).first == 3);
  
  cit = const_tree.find(5);
  BOOST_TEST(cit == const_tree.cend());
}

BOOST_AUTO_TEST_SUITE_END()
