#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <hashTable.hpp>
#include "graph.hpp"

namespace averenkov
{
  void loadGraphsFromFile(HashTable< std::string, Graph >& graphs, std::istream& in);
  void printGraphs(std::ostream& out, const HashTable< std::string, Graph >& graphs);
  void printVertices(std::ostream& out, std::istream& in, const HashTable< std::string, Graph >& graphs);
  void printOutbound(std::ostream& out, std::istream& in, const HashTable< std::string, Graph >& graphs);
  void printInbound(std::ostream& out, std::istream& in, const HashTable< std::string, Graph >& graphs);
  void bindEdge(std::istream& in, HashTable< std::string, Graph >& graphs);
  void cutEdge(std::istream& in, HashTable< std::string, Graph >& graphs);
  void createGraph(std::istream& in, HashTable< std::string, Graph >& graphs);
  void mergeGraphs(std::istream& in, HashTable< std::string, Graph >& graphs);
  void extractGraph(std::istream& in, HashTable< std::string, Graph >& graphs);
  void commandsInit(HashTable< std::string, std::function< void() > >& cmds, HashTable< std::string, Graph > graphs);
}

#endif
