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
  if (names.empty())
  {
    out << std::endl;
    return;
  }
  out << names[0];
  for (size_t i = 1; i < names.size(); ++i)
  {
    out << "\n" << names[i];
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
  auto vertices = g->getVertices();
  if (vertices.empty())
  {
    out << std::endl;
    return;
  }
  out << vertices[0];
  for (size_t i = 1; i < vertices.size(); ++i)
  {
    out << "\n" << vertices[i];
  }
  out << "\n";
}

void smirnov::outbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string graphName, vertexName;
  if (!(in >> graphName >> vertexName))
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
  if (!g->hasVertex(vertexName))
  {
    printError(err);
    return;
  }
  auto edges = g->getOutboundEdges(vertexName);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  out << edges[0].first << " " << edges[0].second;
  for (size_t i = 1; i < edges.size(); ++i)
  {
    out << "\n" << edges[i].first << " " << edges[i].second;
  }
  out << "\n";
}

void smirnov::inbound(const GraphCollection & graphs, std::istream & in, std::ostream & out, std::ostream & err)
{
  std::string graphName, vertexName;
  if (!(in >> graphName >> vertexName))
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
  if (!g->hasVertex(vertexName))
  {
    printError(err);
    return;
  }
  auto edges = g->getInboundEdges(vertexName);
  if (edges.empty())
  {
    out << "\n";
    return;
  }
  out << edges[0].first << " " << edges[0].second;
  for (size_t i = 1; i < edges.size(); ++i)
  {
    out << "\n" << edges[i].first << " " << edges[i].second;
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

void smirnov::merge(GraphCollection& graphs, std::istream& in, std::ostream&, std::ostream& err)
{
    std::string newName, name1, name2;
    if (!(in >> newName >> name1 >> name2)) {
        printError(err);
        return;
    }

    // Проверяем существование графов
    const Graph* g1 = graphs.getGraph(name1);
    const Graph* g2 = graphs.getGraph(name2);
    if (!g1 || !g2 || graphs.getGraph(newName)) {
        printError(err);
        return;
    }

    Graph newGraph(newName);

    // Объединяем вершины
    auto vertices1 = g1->getVertices();
    auto vertices2 = g2->getVertices();
    for (const auto& v : vertices1) {
        newGraph.addVertex(v);
    }
    for (const auto& v : vertices2) {
        if (!newGraph.hasVertex(v)) {
            newGraph.addVertex(v);
        }
    }

    // Объединяем ребра
    for (const auto& v : vertices1) {
        auto edges = g1->getOutboundEdges(v);
        for (const auto& e : edges) {
            newGraph.addEdge(v, e.first, e.second);
        }
    }
    for (const auto& v : vertices2) {
        auto edges = g2->getOutboundEdges(v);
        for (const auto& e : edges) {
            newGraph.addEdge(v, e.first, e.second);
        }
    }

    graphs.addGraph(newName);
}

void smirnov::extract(GraphCollection& graphs, std::istream& in, std::ostream&, std::ostream& err)
{
    std::string newName, oldName;
    size_t count;
    if (!(in >> newName >> oldName >> count)) {
        printError(err);
        return;
    }

    const Graph* oldGraph = graphs.getGraph(oldName);
    if (!oldGraph || graphs.getGraph(newName)) {
        printError(err);
        return;
    }

    std::vector<std::string> vertices;
    for (size_t i = 0; i < count; ++i) {
        std::string vertex;
        if (!(in >> vertex)) {
            printError(err);
            return;
        }
        if (!oldGraph->hasVertex(vertex)) {
            printError(err);
            return;
        }
        vertices.push_back(vertex);
    }

    Graph newGraph(newName);
    for (const auto& v : vertices) {
        newGraph.addVertex(v);
    }

    for (const auto& from : vertices) {
        auto edges = oldGraph->getOutboundEdges(from);
        for (const auto& e : edges) {
            if (std::find(vertices.begin(), vertices.end(), e.first) != vertices.end()) {
                newGraph.addEdge(from, e.first, e.second);
            }
        }
    }

    graphs.addGraph(newName);
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
      extract(graphs, in, out, err);
    }
    else
    {
      printError(err);
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}
