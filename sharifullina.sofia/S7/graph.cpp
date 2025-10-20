#include "graph.hpp"
#include <stdexcept>

namespace
{
  void bubbleSort(std::vector< std::pair< std::string, std::vector< int > > > & vec)
  {
    for (size_t i = 0; i < vec.size(); ++i)
    {
      for (size_t j = i + 1; j < vec.size(); ++j)
      {
        if (vec[j] < vec[i])
        {
          std::swap(vec[i], vec[j]);
        }
      }
    }
  }
}

void sharifullina::Graph::extract(const Graph & graph, const std::set< std::string > & extractVertexes)
{
  for (auto it = extractVertexes.cbegin(); it != extractVertexes.cend(); it++)
  {
    addVertex(*it);
  }

  auto it = graph.edges.begin();
  auto endIt = graph.edges.end();
  while (it != endIt)
  {
    const auto vertexes = it->first;
    if (hasVertex(vertexes.first) && hasVertex(vertexes.second))
    {
      for (auto itWeight = it->second.cbegin(); itWeight != it->second.cend(); ++itWeight)
      {
        addEdge(vertexes.first, vertexes.second, *itWeight);
      }
    }
    ++it;
  }
}

void sharifullina::Graph::merge(const Graph & graph)
{
  for (auto it = graph.edges.begin(); it != graph.edges.end(); it++)
  {
    for (auto itWeight = it->second.begin(); itWeight != it->second.end(); ++itWeight)
    {
      addEdge(it->first.first, it->first.second, *itWeight);
    }
  }
  for (auto it = graph.vertexes.begin(); it != graph.vertexes.end(); it++)
  {
    addVertex(*it);
  }
}

void sharifullina::Graph::addVertex(const std::string & v)
{
  vertexes.insert(v);
}

bool sharifullina::Graph::hasVertex(const std::string & vertex) const
{
  return vertexes.find(vertex) != vertexes.end();
}

bool sharifullina::Graph::hasEdge(const std::string & v1, const std::string & v2, int weight) const
{
  auto it = edges.find({v1, v2});
  if (it == edges.end())
  {
    return false;
  }

  for (int existWeight: it->second)
  {
    if (existWeight == weight)
    {
      return true;
    }
  }
  return false;
}

void sharifullina::Graph::addEdge(const std::string & v1, const std::string & v2, int weight)
{
  vertexes.insert(v1);
  vertexes.insert(v2);
  auto it = edges.find({v1, v2});
  if (it == edges.end())
  {
    std::vector< int > data = {weight};
    edges.insert({v1, v2}, data);
    return;
  }
  auto & temp = edges.at({v1, v2});
  temp.push_back(weight);
}

void sharifullina::Graph::deleteEdge(const std::string & v1, const std::string & v2, int weight)
{
  auto it = edges.find({v1, v2});
  if (it == edges.end())
  {
    throw std::logic_error("ERROR: there is no such edge");
  }

  auto & weights = edges.at(it->first);
  auto itWeight = weights.begin();
  bool deleted = false;
  while (itWeight != weights.end())
  {
    if (*itWeight == weight)
    {
      itWeight = weights.erase(itWeight);
      deleted = true;
    }
    else
    {
      ++itWeight;
    }
  }

  if (!deleted)
  {
    throw std::logic_error("ERROR: there is no such weight");
  }

  if (weights.empty())
  {
    edges.erase(it);
  }
}

std::vector< std::string > sharifullina::Graph::getVertexes() const
{
  return {vertexes.begin(), vertexes.end()};
}

std::vector< std::pair< std::string, std::vector< int > > > sharifullina::Graph::getOutbound(const std::string & vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;

  auto it = edges.begin();
  auto endIt = edges.end();
  while (it != endIt)
  {
    if (it->first.first == vertex)
    {
      result.push_back({it->first.second, it->second});
    }
    ++it;
  }

  bubbleSort(result);
  return result;
}

std::vector< std::pair< std::string, std::vector< int > > > sharifullina::Graph::getInbound(const std::string & vertex) const
{
  std::vector< std::pair< std::string, std::vector< int > > > result;

  auto it = edges.begin();
  auto endIt = edges.end();
  while (it != endIt)
  {
    if (it->first.second == vertex)
    {
      result.push_back({it->first.first, it->second});
    }
    ++it;
  }

  bubbleSort(result);
  return result;
}
