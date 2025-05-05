#include <boost/test/unit_test.hpp>
#include "list.hpp"

using namespace lebedev;

BOOST_AUTO_TEST_SUITE(S1)

BOOST_AUTO_TEST_CASE(DefaultConstructorTest)
{
  List<int> list;
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(FillConstructorTest)
{
  List<int> list(5, 10);
  BOOST_TEST(list.size() == 5);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 10);
}

BOOST_AUTO_TEST_CASE(PushBackTest)
{
  List<size_t> list;
  list.push_back(10);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 10);

  list.push_back(20);
  BOOST_TEST(list.front() == 10);
  BOOST_TEST(list.back() == 20);
}

BOOST_AUTO_TEST_CASE(PushFrontTest)
{
  List<size_t> list;
  list.push_front(10);
  BOOST_TEST(list.front() == 10);

  list.push_front(20);
  BOOST_TEST(list.front() == 20);
  BOOST_TEST(list.back() == 10);
}

BOOST_AUTO_TEST_CASE(PopBackTest)
{
  List<size_t> list;
  list.push_back(10);
  list.push_back(20);
  list.pop_back();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.back() == 10);
}

BOOST_AUTO_TEST_CASE(PopFrontTest)
{
  List<size_t> list;
  list.push_back(10);
  list.push_back(20);
  list.pop_front();
  BOOST_TEST(list.size() == 1);
  BOOST_TEST(list.front() == 20);
}

BOOST_AUTO_TEST_CASE(CopyConstructorTest)
{
  List<size_t> list1;
  list1.push_back(10);
  list1.push_back(20);

  List<size_t> list2(list1);
  BOOST_TEST(list2.size() == 2);
  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list2.back() == 20);
}

BOOST_AUTO_TEST_CASE(MoveConstructorTest)
{
  List<size_t> list1;
  list1.push_back(10);

  List<size_t> list2(std::move(list1));
  BOOST_TEST(list2.size() == 1);
  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(CopyAssignmentTest)
{
  List<size_t> list1;
  list1.push_back(10);

  List<size_t> list2;
  list2 = list1;
  BOOST_TEST(list2.size() == 1);
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(MoveAssignmentTest)
{
  List<size_t> list1;
  list1.push_back(10);

  List<size_t> list2;
  list2 = std::move(list1);
  BOOST_TEST(list2.size() == 1);
  BOOST_TEST(list2.front() == 10);
  BOOST_TEST(list1.empty());
}

BOOST_AUTO_TEST_CASE(IteratorsTest)
{
  List<size_t> list;
  list.push_back(10);
  list.push_back(20);

  auto it = list.begin();
  BOOST_TEST(*it == 10);
  ++it;
  BOOST_TEST(*it == 20);
  ++it;
  BOOST_TEST(it == list.end());
}

BOOST_AUTO_TEST_CASE(ConstIteratorsTest)
{
  List<size_t> list;
  list.push_back(10);

  const List<size_t>& clist = list;
  auto cit = clist.constBegin();
  BOOST_TEST(*cit == 10);
  ++cit;
  BOOST_TEST(cit == clist.constEnd());
}

BOOST_AUTO_TEST_CASE(ClearTest)
{
  List<size_t> list;
  list.push_back(10);
  list.push_back(20);
  list.clear();
  BOOST_TEST(list.empty());
  BOOST_TEST(list.size() == 0);
}

BOOST_AUTO_TEST_CASE(SwapTest)
{
  List<size_t> list1;
  list1.push_back(10);

  List<size_t> list2;
  list2.push_back(20);

  list1.swap(list2);
  BOOST_TEST(list1.front() == 20);
  BOOST_TEST(list2.front() == 10);
}

BOOST_AUTO_TEST_CASE(EdgeCasesTest)
{
  List<size_t> list;
  BOOST_CHECK_NO_THROW(list.pop_back());
  BOOST_CHECK_NO_THROW(list.pop_front());

  list.push_back(42);
  BOOST_TEST(list.front() == 42);
  BOOST_TEST(list.back() == 42);
  list.pop_back();
  BOOST_TEST(list.empty());
}

BOOST_AUTO_TEST_SUITE_END()
