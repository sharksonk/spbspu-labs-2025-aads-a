#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <iostream>
#include <functional>
#include <map>
#include <limits>

using dict_t = std::map< size_t, std::string >;
using dataset_t = std::map< std::string, dict_t >;

namespace kushekbaev
{
  void executeCommand(std::istream& in, std::ostream& out, dataset_t& dataset);

  void print(std::ostream& out, std::istream& in, const dataset_t& dataset);

  void complement(std::istream& in, const dataset_t& dataset);

  void intersect(std::istream& in, const dataset_t& dataset);

  void unification(std::istream& in, const dataset_t& dataset);

  struct PrintCommand
  {
    std::ostream& out;
    std::istream& in;
    const dataset_t& dataset;

    void operator()() const
    {
      print(out, in, dataset);
    }
  };

  struct ComplementCommand
  {
    std::istream& in;
    const dataset_t& dataset;

    void operator()() const
    {
      complement(in, dataset);
    }
  };

  struct IntersectCommand
  {
    std::istream& in;
    const dataset_t& dataset;

    void operator()() const
    {
      intersect(in, dataset);
    }
  };

  struct UnionCommand
  {
    std::istream& in;
    const dataset_t& dataset;

    void operator()() const
    {
      unification(in, dataset);
    }
  };
}

#endif
