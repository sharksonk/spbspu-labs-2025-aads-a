#include "ioutils.hpp"
#include <iostream>

void kushekbaev::splitExpr(const std::string& string, kushekbaev::Queue< std::string >& arithmeticQ)
{
  std::string symbol;
  char delimiter = ' ';
  size_t begin = string.find_first_not_of(delimiter);
  size_t end = 0;
  while (begin != std::string::npos)
  {
    end = string.find(delimiter, begin);
    symbol = string.substr(begin, end - begin);
    arithmeticQ.push(symbol);
    begin = string.find_first_not_of(delimiter, end);
  }
}

void kushekbaev::inputExpr(std::istream& input, kushekbaev::Queue< kushekbaev::Queue< std::string > >& Q)
{
  std::string string;
  kushekbaev::Queue< std::string > infixQ;
  while (std::getline(input, string))
  {
    if (string.empty())
    {
      continue;
    }
    infixQ.clear();
    splitExpr(string, infixQ);
    Q.push(std::move(infixQ));
  }
}

void kushekbaev::output(std::ostream& output, kushekbaev::Stack< long long int >& results)
{
  if (!results.empty())
  {
    output << results.top();
    results.pop();
    while (!results.empty())
    {
      output << " " << results.top();
      results.pop();
    }
  }
}