#include <boost/test/unit_test.hpp>
#include "bitree.hpp"
#include "avl_tree.hpp"

using namespace karnauhova;

BOOST_AUTO_TEST_SUITE(avl_tree)

BOOST_AUTO_TEST_CASE(default_constructor)
{
  AvlTree< int, std::string > tree;
  BOOST_TEST(tree.size() == 0);
  BOOST_TEST(tree.empty());
  BOOST_TEST(tree.begin() == tree.end());
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  AvlTree< int, std::string > original;
  original.insert({1, "one"});
  original.insert({2, "two"});
  original.insert({3, "three"});

  AvlTree< int, std::string > copy(original);

  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy.find(1) != copy.end());
  BOOST_TEST(copy.find(2) != copy.end());
  BOOST_TEST(copy.find(3) != copy.end());
  BOOST_TEST(copy[1] == "one");
  BOOST_TEST(copy[2] == "two");
  BOOST_TEST(copy[3] == "three");

  // Проверяем, что оригинал не изменился
  BOOST_TEST(original.size() == 3);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  AvlTree< int, std::string > original;
  original.insert({1, "one"});
  original.insert({2, "two"});
  original.insert({3, "three"});

  AvlTree< int, std::string > moved(std::move(original));

  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(moved.find(1) != moved.end());
  BOOST_TEST(moved.find(2) != moved.end());
  BOOST_TEST(moved.find(3) != moved.end());
  BOOST_TEST(moved[1] == "one");
  BOOST_TEST(moved[2] == "two");
  BOOST_TEST(moved[3] == "three");

  // Оригинал должен быть пустым после перемещения
  BOOST_TEST(original.size() == 0);
  BOOST_TEST(original.empty());
}

BOOST_AUTO_TEST_CASE(copy_assignment)
{
  AvlTree< int, std::string > original;
  original.insert({1, "one"});
  original.insert({2, "two"});
  original.insert({3, "three"});

  AvlTree< int, std::string > copy;
  copy = original;

  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy[1] == "one");
  BOOST_TEST(copy[2] == "two");
  BOOST_TEST(copy[3] == "three");

  // Проверяем self-assignment
  copy = copy;
  BOOST_TEST(copy.size() == 3);
  BOOST_TEST(copy[1] == "one");
}

BOOST_AUTO_TEST_CASE(move_assignment)
{
  AvlTree< int, std::string > original;
  original.insert({1, "one"});
  original.insert({2, "two"});
  original.insert({3, "three"});

  AvlTree< int, std::string > moved;
  moved = std::move(original);

  BOOST_TEST(moved.size() == 3);
  BOOST_TEST(moved[1] == "one");
  BOOST_TEST(moved[2] == "two");
  BOOST_TEST(moved[3] == "three");

  // Оригинал должен быть пустым после перемещения
  BOOST_TEST(original.size() == 0);
  BOOST_TEST(original.empty());

  // Проверяем self-assignment
  moved = std::move(moved);
  BOOST_TEST(moved.size() == 3);
}

BOOST_AUTO_TEST_CASE(subscript_operator)
{
  AvlTree< int, std::string > tree;

  // Оператор [] для вставки
  tree[1] = "one";
  tree[2] = "two";
  tree[3] = "three";

  BOOST_TEST(tree.size() == 3);
  BOOST_TEST(tree[1] == "one");
  BOOST_TEST(tree[2] == "two");
  BOOST_TEST(tree[3] == "three");

  // Оператор [] для модификации
  tree[2] = "modified";
  BOOST_TEST(tree[2] == "modified");

  // Оператор [] для создания нового элемента
  tree[4] = "four";
  BOOST_TEST(tree.size() == 4);
  BOOST_TEST(tree[4] == "four");
}

BOOST_AUTO_TEST_CASE(const_subscript_operator)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  tree.insert({3, "three"});

  const AvlTree< int, std::string >& const_tree = tree;

  BOOST_TEST(const_tree[1] == "one");
  BOOST_TEST(const_tree[2] == "two");
  BOOST_TEST(const_tree[3] == "three");
}

BOOST_AUTO_TEST_CASE(at_method)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  tree.insert({3, "three"});

  BOOST_TEST(tree.at(1) == "one");
  BOOST_TEST(tree.at(2) == "two");
  BOOST_TEST(tree.at(3) == "three");

  // Метод at должен бросать исключение для несуществующего ключа
  BOOST_CHECK_THROW(tree.at(4), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(const_at_method)
{
  AvlTree< int, std::string > tree;
  tree.insert({1, "one"});
  tree.insert({2, "two"});
  tree.insert({3, "three"});

  const AvlTree< int, std::string >& const_tree = tree;

  BOOST_TEST(const_tree.at(1) == "one");
  BOOST_TEST(const_tree.at(2) == "two");
  BOOST_TEST(const_tree.at(3) == "three");

  // Const метод at также должен бросать исключение
  BOOST_CHECK_THROW(const_tree.at(4), std::out_of_range);
}

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
