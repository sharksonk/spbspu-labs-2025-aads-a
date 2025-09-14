#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>
#include <limits>
#include <avl_tree.hpp>
#include "key_sum.hpp"

BOOST_AUTO_TEST_CASE(traversal_simple_test)
{
  karnauhova::AvlTree< int, std::string > tree;

  tree[10] = "keyboard";
  tree[12] = "monitor";
  tree[-5] = "mouse";

  karnauhova::KeySum res_lnr;
  karnauhova::KeySum res_rnl;
  karnauhova::KeySum res_br;

  res_lnr = tree.traverse_lnr(res_lnr);
  res_rnl = tree.traverse_rnl(res_rnl);
  res_br = tree.traverse_breadth(res_br);

  BOOST_TEST(res_lnr.result_ == 17);
  BOOST_TEST(res_rnl.result_ == 17);
  BOOST_TEST(res_br.result_ == 17);

  BOOST_TEST(res_lnr.itInput_ == "mouse keyboard monitor");
  BOOST_TEST(res_rnl.itInput_ == "monitor keyboard mouse");
  BOOST_TEST(res_br.itInput_ == "keyboard mouse monitor");
}

BOOST_AUTO_TEST_CASE(traversal_big_tree_test)
{
  karnauhova::AvlTree< int , std::string > tree;
  tree[50] = "root";
  tree[30] = "left";
  tree[70] = "right";
  tree[20] = "leftleft";
  tree[40] = "leftright";
  tree[60] = "rightleft";
  tree[80] = "rightright";

  karnauhova::KeySum lnr_res;
  karnauhova::KeySum rnl_res;
  karnauhova::KeySum br_res;

  lnr_res = tree.traverse_lnr(lnr_res);
  rnl_res = tree.traverse_rnl(rnl_res);
  br_res = tree.traverse_breadth(br_res);

  BOOST_TEST(lnr_res.result_ == 350);
  BOOST_TEST(rnl_res.result_ == 350);
  BOOST_TEST(br_res.result_ == 350);

  BOOST_TEST(lnr_res.itInput_ == "leftleft left leftright root rightleft right rightright");
  BOOST_TEST(rnl_res.itInput_ == "rightright right rightleft root leftright left leftleft");
  BOOST_TEST(br_res.itInput_ == "root left right leftleft leftright rightleft rightright");
}
