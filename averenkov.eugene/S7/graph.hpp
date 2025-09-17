#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <array.hpp>
#include <unordered_map>
#include <functional>

namespace averenkov
{
  struct Graph
  {
    std::string name;
    std::unordered_map< std::string, std::unordered_map< std::string, Array< size_t > > > edges;
    std::unordered_map< std::string, bool > vertices;
  };

}

#endif

