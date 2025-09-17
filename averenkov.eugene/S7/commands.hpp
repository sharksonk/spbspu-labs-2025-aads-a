#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include "graph.hpp"

namespace averenkov
{
  void loadGraphsFromFile(std::unordered_map< std::string, Graph >& graphs, std::istream& in);
  void printGraphs(std::ostream& out, const std::unordered_map< std::string, Graph >& graphs);
  void printVertices(std::ostream& out, std::istream& in, const std::unordered_map< std::string, Graph >& graphs);
  void printOutbound(std::ostream& out, std::istream& in, const std::unordered_map< std::string, Graph >& graphs);
  void printInbound(std::ostream& out, std::istream& in, const std::unordered_map< std::string, Graph >& graphs);
  void bindEdge(std::istream& in, std::unordered_map< std::string, Graph >& graphs);
  void cutEdge(std::istream& in, std::unordered_map< std::string, Graph >& graphs);
  void createGraph(std::istream& in, std::unordered_map< std::string, Graph >& graphs);
  void mergeGraphs(std::istream& in, std::unordered_map< std::string, Graph >& graphs);
  void extractGraph(std::istream& in, std::unordered_map< std::string, Graph >& graphs);
  void commandsInit(std::unordered_map< std::string, std::function< void() > >& cmds, std::unordered_map< std::string, Graph > graphs);
}

#endif
