#include "commands.hpp"
#include <limits>
#include <algorithm>

void smirnov::printError(std::ostream & err)
{
  err << "<INVALID COMMAND>\n";
}

void smirnov::command_graphs(const GraphCollection & graphs, std::ostream & out)
{
  auto names = graphs.listGraphs();
  if (!names.empty())
  {
    out << names[0];
    for (size_t i = 1; i < names.size(); ++i)
    {
      out << "\n" << names[i];
    }
  }
  out << "\n";
}

void smirnov::vertexes(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    printError(err);
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  const Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  std::vector< std::string > vertices = g->getVertices();
  if (!vertices.empty())
  {
    out << vertices[0];
    for (size_t i = 1; i < vertices.size(); ++i)
    {
        out << "\n" << vertices[i];
    }
  }
  out << "\n";
}

void smirnov::outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex))
  {
    printError(err);
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  const Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  auto verts = g->getVertices();
  if (std::find(verts.begin(), verts.end(), vertex) == verts.end())
  {
    printError(err);
    return;
  }
  auto edges = g->getOutboundEdges(vertex);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  auto it = edges.begin();
  out << it->first << " " << it->second;
  for (++it; it != edges.end(); ++it)
  {
    out << "\n" << it->first << " " << it->second;
  }
  out << "\n";
}

void smirnov::inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex))
  {
    printError(err);
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  const Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  auto verts = g->getVertices();
  if (std::find(verts.begin(), verts.end(), vertex) == verts.end())
  {
    printError(err);
    return;
  }
  auto edges = g->getInboundEdges(vertex);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  auto it = edges.begin();
  auto end = edges.end();
  out << it->first << " " << it->second;
  for (++it; it != end; ++it)
  {
    out << "\n" << it->first << " " << it->second;
  }
  out << "\n";
}

void smirnov::bind(GraphCollection & graphs, std::istream & in, std::ostream & err)
{
  std::string graphName, from, to;
  unsigned weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    printError(err);
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  g->addEdge(from, to, weight);
}

void smirnov::cut(GraphCollection & graphs, std::istream & in, std::ostream & err)
{
  std::string graphName, from, to;
  unsigned weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    printError(err);
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  if (!g->removeEdge(from, to, weight))
  {
    printError(err);
  }
}

void smirnov::create(GraphCollection & graphs, std::istream & in, std::ostream & err)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    printError(err);
    return;
  }
  if (!graphs.addGraph(graphName))
  {
    printError(err);
    return;
  }
  Graph * g = graphs.getGraph(graphName);
  if (!g)
  {
    printError(err);
    return;
  }
  size_t count;
  if (!(in >> count))
  {
    printError(err);
    return;
  }
  for (size_t i = 0; i < count; ++i)
  {
    std::string vertex;
    if (!(in >> vertex))
    {
      printError(err);
      return;
    }
    g->addVertex(vertex);
  }
}

void smirnov::merge(GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    printError(err);
    return;
  }
  if (graphs.getGraph(newName) != nullptr)
  {
    printError(err);
    return;
  }
  const Graph * g1 = graphs.getGraph(name1);
  const Graph * g2 = graphs.getGraph(name2);
  if (!g1 || !g2)
  {
    printError(err);
    return;
  }
  Graph mergedGraph(newName);
  std::vector< std::string > vertices1 = g1->getVertices();
  for (size_t i = 0; i < vertices1.size(); ++i)
  {
    mergedGraph.addVertex(vertices1[i]);
  }
  for (size_t i = 0; i < vertices1.size(); ++i)
  {
    std::vector< std::pair< std::string, unsigned > > edges = g1->getOutboundEdges(vertices1[i]);
    for (size_t j = 0; j < edges.size(); ++j)
    {
      mergedGraph.addEdge(vertices1[i], edges[j].first, edges[j].second);
    }
  }
  std::vector< std::string > vertices2 = g2->getVertices();
  for (size_t i = 0; i < vertices2.size(); ++i)
  {
    if (!mergedGraph.hasVertex(vertices2[i]))
    {
      mergedGraph.addVertex(vertices2[i]);
    }
  }
  for (size_t i = 0; i < vertices2.size(); ++i)
  {
    std::vector< std::pair< std::string, unsigned > > edges = g2->getOutboundEdges(vertices2[i]);
    for (size_t j = 0; j < edges.size(); ++j)
    {
      mergedGraph.addEdge(vertices2[i], edges[j].first, edges[j].second);
    }
  }
  std::vector< std::string > allVertices = mergedGraph.getVertices();
  if (!allVertices.empty())
  {
    out << allVertices[0];
    for (size_t i = 1; i < allVertices.size(); ++i)
    {
      out << "\n" << allVertices[i];
    }
  }
  out << "\n";
}

void smirnov::extract(GraphCollection & graphs, std::istream & in, std::ostream & err)
{
  std::string newGraphName, sourceGraphName;
  size_t vertexCount;
  if (!(in >> newGraphName >> sourceGraphName >> vertexCount))
  {
    printError(err);
    return;
  }
  if (graphs.getGraph(newGraphName))
  {
    printError(err);
    return;
  }
  const Graph * sourceGraph = graphs.getGraph(sourceGraphName);
  if (!sourceGraph)
  {
    printError(err);
    return;
  }
  std::vector< std::string > vertices;
  for (size_t i = 0; i < vertexCount; ++i)
  {
    std::string vertexName;
    if (!(in >> vertexName))
    {
      printError(err);
      return;
    }
    bool vertexExists = false;
    auto allVertices = sourceGraph->getVertices();
    for (size_t j = 0; j < allVertices.size(); ++j)
    {
      if (allVertices[j] == vertexName)
      {
        vertexExists = true;
        break;
      }
    }
    if (!vertexExists)
    {
      printError(err);
      return;
    }
    vertices.push_back(vertexName);
  }
  Graph newGraph(newGraphName);
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    newGraph.addVertex(vertices[i]);
  }
  for (size_t i = 0; i < vertices.size(); ++i)
  {
    std::string fromVertex = vertices[i];
    auto outEdges = sourceGraph->getOutboundEdges(fromVertex);
    for (size_t j = 0; j < outEdges.size(); ++j)
    {
      std::string toVertex = outEdges[j].first;
      unsigned edgeWeight = outEdges[j].second;
      bool vertexInNewGraph = false;
      for (size_t k = 0; k < vertices.size(); ++k)
      {
        if (vertices[k] == toVertex)
        {
          vertexInNewGraph = true;
          break;
        }
      }
      if (vertexInNewGraph)
      {
        newGraph.addEdge(fromVertex, toVertex, edgeWeight);
      }
    }
  }
  graphs.addGraph(newGraphName);
}

void smirnov::processCommands(GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string cmd;
  while (in >> cmd)
  {
    if (cmd == "graphs")
    {
      command_graphs(graphs, out);
    }
    else if (cmd == "vertexes")
    {
      vertexes(graphs, in, out, err);
    }
    else if (cmd == "outbound")
    {
      outbound(graphs, in, out, err);
    }
    else if (cmd == "inbound")
    {
      inbound(graphs, in, out, err);
    }
    else if (cmd == "bind")
    {
      bind(graphs, in, err);
    }
    else if (cmd == "cut")
    {
      cut(graphs, in, err);
    }
    else if (cmd == "create")
    {
      create(graphs, in, err);
    }
    else if (cmd == "merge")
    {
      merge(graphs, in, out, err);
    }
    else if (cmd == "extract")
    {
      extract(graphs, in, err);
    }
    else
    {
      printError(err);
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
