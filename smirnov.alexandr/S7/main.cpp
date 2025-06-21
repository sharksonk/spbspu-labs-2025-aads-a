#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <limits>
#include "graphCollection.hpp"
#include "commands.hpp"

namespace
{
  void loadGraphsFromFile(std::ifstream & inputFile, smirnov::GraphCollection & graphs)
  {
    std::string graphName;
    size_t edgeCount;
    while (inputFile >> graphName >> edgeCount)
    {
      smirnov::Graph graph(graphName);
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string from, to;
        unsigned weight;
        if (!(inputFile >> from >> to >> weight))
        {
          throw std::runtime_error("Input file format error");
        }
        graph.addEdge(from, to, weight);
      }
      if (!graphs.addGraph(graphName))
      {
        throw std::runtime_error("Duplicate graph name in input file");
      }
      smirnov::Graph * g = graphs.getGraph(graphName);
      if (g)
      {
        *g = graph;
      }
    }
  }
}

int main(int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cerr << "Error: input filename required\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Error: cannot open file " << argv[1] << "\n";
    return 1;
  }

  smirnov::GraphCollection graphs;

  try
  {
    loadGraphsFromFile(inputFile, graphs);
  }
  catch (const std::exception & ex)
  {
    std::cerr << "Error loading graphs: " << ex.what() << "\n";
    return 1;
  }

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      if (command == "graphs")
      {
        smirnov::graphs(graphs, std::cout);
      }
      else if (command == "vertexes")
      {
        smirnov::vertexes(graphs, std::cin, std::cout);
      }
      else if (command == "outbound")
      {
        smirnov::outbound(graphs, std::cin, std::cout);
      }
      else if (command == "inbound")
      {
        smirnov::inbound(graphs, std::cin, std::cout);
      }
      else if (command == "bind")
      {
        smirnov::bind(graphs, std::cin);
      }
      else if (command == "cut")
      {
        smirnov::cut(graphs, std::cin);
      }
      else if (command == "create")
      {
        smirnov::create(graphs, std::cin);
      }
      else if (command == "merge")
      {
        smirnov::merge(graphs, std::cin);
      }
      else if (command == "extract")
      {
        smirnov::extract(graphs, std::cin);
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
    catch (const std::invalid_argument &)
    {
      std::cerr << "<INVALID COMMAND>" << std::endl;
      return 1;
    }
    return 0;
  }
}
