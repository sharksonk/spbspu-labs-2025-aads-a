#include <boost/test/unit_test.hpp>
#include <sstream>
#include <algorithm>
#include "binary_search_tree.hpp"
#include "files.hpp"

BOOST_AUTO_TEST_CASE(simple_ascending_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "name first surname");
}

BOOST_AUTO_TEST_CASE(simple_descending_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "surname first name");
}

BOOST_AUTO_TEST_CASE(simple_breadth_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({2, "first"});
  tree.insert({1, "name"});
  tree.insert({3, "surname"});
  KeySumm collector;
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.result_ == 6);
  std::vector< std::string > expected_words = {"first", "name", "surname"};
  std::istringstream iss(collector.values_);
  std::vector< std::string > actual_words;
  std::string word;
  while (iss >> word) {
    actual_words.push_back(word);
  }
  BOOST_TEST(actual_words.size() == 3);
  for (const auto& expected : expected_words) {
    BOOST_TEST(std::find(actual_words.begin(), actual_words.end(), expected) != actual_words.end());
  }
}

BOOST_AUTO_TEST_CASE(empty_tree_traversal)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  KeySumm lnr_collector;
  lnr_collector = tree.traverse_lnr(lnr_collector);
  BOOST_TEST(lnr_collector.result_ == 0);
  BOOST_TEST(lnr_collector.values_.empty());
  KeySumm rnl_collector;
  rnl_collector = tree.traverse_rnl(rnl_collector);
  BOOST_TEST(rnl_collector.result_ == 0);
  BOOST_TEST(rnl_collector.values_.empty());
  KeySumm breadth_collector;
  breadth_collector = tree.traverse_breadth(breadth_collector);
  BOOST_TEST(breadth_collector.result_ == 0);
  BOOST_TEST(breadth_collector.values_.empty());
}

BOOST_AUTO_TEST_CASE(overflow_protection_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({std::numeric_limits<int>::max(), "max"});
  tree.insert({1, "one"});
  KeySumm collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(underflow_protection_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({std::numeric_limits<int>::min(), "min"});
  tree.insert({-1, "negative"});
  KeySumm collector;
  BOOST_CHECK_THROW(tree.traverse_lnr(collector), std::underflow_error);
}
