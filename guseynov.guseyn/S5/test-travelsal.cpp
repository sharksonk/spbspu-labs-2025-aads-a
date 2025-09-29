#include <boost/test/unit_test.hpp>
#include <sstream>
#include <algorithm>
#include "binary_search_tree.hpp"
#include "files.hpp"

BOOST_AUTO_TEST_CASE(lnr_traversal_standard)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({5, "five"});
  tree.insert({3, "three"});
  tree.insert({7, "seven"});
  tree.insert({2, "two"});
  tree.insert({4, "four"});
  tree.insert({6, "six"});
  tree.insert({8, "eight"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 35);
  BOOST_TEST(collector.values_ == "two three four five six seven eight");
}

BOOST_AUTO_TEST_CASE(rnl_traversal_standard)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({5, "five"});
  tree.insert({3, "three"});
  tree.insert({7, "seven"});
  tree.insert({2, "two"});
  tree.insert({4, "four"});
  tree.insert({6, "six"});
  tree.insert({8, "eight"});
  KeySumm collector;
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result_ == 35);
  BOOST_TEST(collector.values_ == "eight seven six five four three two");
}

BOOST_AUTO_TEST_CASE(breadth_traversal_standard)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({5, "five"});
  tree.insert({3, "three"});
  tree.insert({7, "seven"});
  tree.insert({2, "two"});
  tree.insert({4, "four"});
  tree.insert({6, "six"});
  tree.insert({8, "eight"});
  KeySumm collector;
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.values_.find("two") != std::string::npos);
  BOOST_TEST(collector.values_.find("three") != std::string::npos);
  BOOST_TEST(collector.values_.find("four") != std::string::npos);
  BOOST_TEST(collector.values_.find("five") != std::string::npos);
  BOOST_TEST(collector.values_.find("six") != std::string::npos);
  BOOST_TEST(collector.values_.find("seven") != std::string::npos);
  BOOST_TEST(collector.values_.find("eight") != std::string::npos);
  size_t space_count = std::count(collector.values_.begin(), collector.values_.end(), ' ');
  BOOST_TEST(space_count == 6);
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

BOOST_AUTO_TEST_CASE(single_node_traversal)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({1, "one"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 1);
  BOOST_TEST(collector.values_ == "one");
  collector.result_ = 0;
  collector.values_.clear();
  collector = tree.traverse_rnl(collector);
  BOOST_TEST(collector.result_ == 1);
  BOOST_TEST(collector.values_ == "one");
  collector.result_ = 0;
  collector.values_.clear();
  collector = tree.traverse_breadth(collector);
  BOOST_TEST(collector.result_ == 1);
  BOOST_TEST(collector.values_ == "one");
}

BOOST_AUTO_TEST_CASE(multiple_traversals_consistency)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({3, "three"});
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  tree.insert({4, "four"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 10);
  BOOST_TEST(collector.values_ == "one two three four");
  collector.result_ = 0;
  collector.values_.clear();
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 10);
  BOOST_TEST(collector.values_ == "one two three four");
}

BOOST_AUTO_TEST_CASE(duplicate_values_traversal)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({1, "uno"});
  tree.insert({2, "two"});
  KeySumm collector;
  collector = tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 3);
  BOOST_TEST(collector.values_ == "uno two");
}

BOOST_AUTO_TEST_CASE(const_traversal_test)
{
  guseynov::BinarySearchTree< int, std::string > tree;
  tree.insert({3, "three"});
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  const auto& const_tree = tree;
  KeySumm collector;
  collector = const_tree.traverse_lnr(collector);
  BOOST_TEST(collector.result_ == 6);
  BOOST_TEST(collector.values_ == "one two three");
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
  BOOST_TEST(collector.values_ == "first name surname");
}
