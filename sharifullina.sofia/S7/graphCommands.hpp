#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <map>
#include <hash_table/hashTable.hpp>
#include "graph.hpp"

namespace sharifullina
{
  using map = std::map< std::string, sharifullina::Graph >;
  
  void getGraphs(std::ostream & out, const map & graphs);
  void getVertexes(std::istream & in, std::ostream & out, const map & graphs);
  void getOutbound(std::istream & in, std::ostream & out, const map & graphs);
  void getInbound(std::istream & in, std::ostream & out, const map & graphs);
  void createEdge(std::istream & in, map & graphs);
  void deleteEdge(std::istream & in, map & graphs);
  void createGraph(std::istream & in, map & graphs);
  void mergeGraph(std::istream & in, map & graphs);
  void extractGraph(std::istream & in, map & graphs);
}
#endif
