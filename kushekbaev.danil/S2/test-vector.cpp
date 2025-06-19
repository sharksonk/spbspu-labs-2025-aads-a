#include <boost/test/unit_test.hpp>
#include "vector.hpp"

using namespace kushekbaev;

BOOST_AUTO_TEST_SUITE(VectorConstructors)

BOOST_AUTO_TEST_CASE(VectorDefaultConstructor)
{
  Vector< int > Vector;
  BOOST_TEST(Vector.empty());
}

BOOST_AUTO_TEST_CASE(VectorCopyConstructor)
{
  Vector< int > Vector;
  Vector.push_back(10);
  Vector.push_back(20);
  Vector< int > Vectorcopy(Vector);
  BOOST_TEST(Vector.front() == Vectorcopy.front());
  BOOST_TEST(Vector.back() == Vectorcopy.back());
  BOOST_TEST(Vector.size() == Vectorcopy.size());
  BOOST_TEST(Vectorcopy.front() == 10);
  BOOST_TEST(Vectorcopy.back() == 20);
  BOOST_TEST(Vectorcopy.size() == 2);
}

BOOST_AUTO_TEST_CASE(VectorMoveConstructor)
{
  Vector< int > Vector;
  Vector.push_back(10);
  Vector.push_back(20);
  Vector< int > Vectorcopy(std::move(Vector));
  BOOST_TEST(Vector.size() == 0);
  BOOST_TEST(Vectorcopy.front() == 10);
  BOOST_TEST(Vectorcopy.back() == 20);
  BOOST_TEST(Vectorcopy.size() == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(VectorAssignment)
{
  Vector< int > Vector1;
  Vector1.push_back(10);
  Vector< int > Vector2;
  Vector2 = Vector1;
  BOOST_TEST(Vector2.front() == 10);
}

BOOST_AUTO_TEST_SUITE(VectorElementAccess)

BOOST_AUTO_TEST_CASE(VectorFront)
{
  Vector< int > Vector;
  Vector.push_back(10);
  BOOST_TEST(Vector.front() == 10);
  Vector.push_back(20);
  BOOST_TEST(Vector.front() == 10);
}

BOOST_AUTO_TEST_CASE(VectorBack)
{
  Vector< int > Vector;
  Vector.push_back(10);
  BOOST_TEST(Vector.back() == 10);
  Vector.push_back(20);
  BOOST_TEST(Vector.back() == 20);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(VectorCapacity)

BOOST_AUTO_TEST_CASE(VectorEmpty)
{
  Vector< int > Vector;
  BOOST_TEST(Vector.empty());
  Vector.push_back(10);
  BOOST_TEST(!Vector.empty());
  Vector.pop_back();
  BOOST_TEST(Vector.empty());
}

BOOST_AUTO_TEST_CASE(VectorSize)
{
  Vector< int > Vector;
  BOOST_TEST(Vector.size() == 0);
  Vector.push_back(10);
  Vector.push_back(10);
  BOOST_TEST(Vector.size() == 2);
  Vector.pop_back();
  BOOST_TEST(Vector.size() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(VectorPushPop)
{
  Vector< int > Vector;
  Vector.push_back(1000);
  BOOST_TEST(Vector.back() == 1000);
  Vector.push_back(22222);
  BOOST_TEST(Vector.back() == 22222);
  Vector.push_back(1337);
  BOOST_TEST(Vector.back() == 1337);
  Vector.pop_back();
  BOOST_TEST(Vector.back() == 22222);
  Vector.pop_front();
  BOOST_TEST(Vector.back() == Vector.front());
}