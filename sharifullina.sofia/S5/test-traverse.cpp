#include <boost/test/unit_test.hpp>
#include <string>
#include <../common/avl_tree/AVLtree.hpp>

using Tree = sharifullina::AVLtree< int, std::string >;

BOOST_AUTO_TEST_CASE(traverses_test)
{
  Tree tree;

  tree.insert(std::make_pair(2, "second"));
  tree.insert(std::make_pair(1, "first"));
  tree.insert(std::make_pair(3, "third"));

  const auto& constTree = tree;
  struct Collector {
    std::string values;
    void operator()(const std::pair< int, std::string >& kv) {
      if (!values.empty()) values += " ";
      values += kv.second;
    }
  } collector;

  collector.values = "";
  constTree.traverse_lnr(collector);
  BOOST_TEST(collector.values == "first second third");

  collector.values = "";
  constTree.traverse_rnl(collector);
  BOOST_TEST(collector.values == "third second first");

  collector.values = "";
  constTree.traverse_breadth(collector);
  BOOST_TEST(collector.values == "second first third");

  collector.values = "";
  tree.traverse_lnr(collector);
  BOOST_TEST(collector.values == "first second third");

  collector.values = "";
  tree.traverse_rnl(collector);
  BOOST_TEST(collector.values == "third second first");

  collector.values = "";
  tree.traverse_breadth(collector);
  BOOST_TEST(collector.values == "second first third");
}
