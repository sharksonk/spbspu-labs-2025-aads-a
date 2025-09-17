#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include "stack.hpp"

namespace shramko
{
<<<<<<< HEAD
  void outputStack(std::ostream& out, Stack<long long>& stack)
=======
  template< typename T >
  void outputStack(std::ostream& out, Stack< T >& stack)
>>>>>>> master
  {
    if (stack.empty())
    {
      return;
    }
    out << stack.top();
    stack.pop();
    while (!stack.empty())
    {
      out << " " << stack.top();
      stack.pop();
    }
  }
}

#endif
