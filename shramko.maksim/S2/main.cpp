#include <iostream>
#include <fstream>
#include <limits>
#include <cstddef>
#include "queue.hpp"
#include "stack.hpp"
#include "print.hpp"

namespace
{
  int getPriority(const std::string& op)
  {
    if (op == "+" || op == "-")
    {
      return 0;
    }
    else if (op == "*" || op == "/" || op == "%")
    {
      return 1;
    }
    return -1;
  }

  bool isOperator(const std::string& s)
  {
    if (s.empty())
    {
      return false;
    }
    if (s == "+" || s == "-" || s == "%" || s == "/" || s == "*")
    {
      return true;
    }
    return false;
  }

<<<<<<< HEAD
  void inputInfix(std::istream& in, shramko::Queue<std::string>& queue)
=======
  void inputInfix(std::istream& in, shramko::Queue< std::string >& queue)
>>>>>>> master
  {
    std::string infix;
    std::getline(in, infix);
    if (infix.empty())
    {
      return;
    }
    size_t pos = 0, pos2 = 0;
    std::string temp;
    while (pos2 != std::string::npos)
    {
      pos2 = infix.find(' ', pos);
      if (pos2 == std::string::npos)
      {
        if (pos < infix.size())
        {
          temp = infix.substr(pos);
        }
      }
      else
      {
        temp = infix.substr(pos, pos2 - pos);
      }
      if (!temp.empty())
      {
        queue.push(temp);
      }
      temp.clear();
      pos = pos2 + 1;
    }
  }

<<<<<<< HEAD
  void input(std::istream& in, shramko::Queue<shramko::Queue<std::string>>& queue)
  {
    while (!in.eof())
    {
      shramko::Queue<std::string> temp_inf_expr;
=======
  void input(std::istream& in, shramko::Queue< shramko::Queue< std::string > >& queue)
  {
    while (!in.eof())
    {
      shramko::Queue< std::string > temp_inf_expr;
>>>>>>> master
      inputInfix(in, temp_inf_expr);
      if (!temp_inf_expr.empty())
      {
        queue.push(std::move(temp_inf_expr));
      }
    }
  }

<<<<<<< HEAD
  void convertInfToPost(shramko::Queue<std::string>& inf, shramko::Queue<std::string>& post_queue)
  {
    shramko::Stack<std::string> stack;
=======
  void convertInfToPost(shramko::Queue< std::string >& inf, shramko::Queue< std::string >& post_queue)
  {
    shramko::Stack< std::string > stack;
>>>>>>> master
    while (!inf.empty())
    {
      std::string temp = inf.front();
      inf.pop();
      try
      {
        std::stoll(temp);
        post_queue.push(temp);
      }
      catch (...)
      {
        if (temp == "(")
        {
          stack.push(temp);
        }
        else if (temp == ")")
        {
          if (stack.empty())
          {
            throw std::logic_error("no open bracket");
          }
          std::string stack_temp = stack.top();
          stack.pop();
          while (stack_temp != "(")
          {
            post_queue.push(stack_temp);
            if (stack.empty())
            {
              throw std::logic_error("no open bracket");
            }
            stack_temp = stack.top();
            stack.pop();
          }
        }
        else if (isOperator(temp))
        {
          while (!stack.empty() && stack.top() != "(")
          {
            std::string stack_temp = stack.top();
            if (getPriority(stack_temp) < getPriority(temp))
            {
              break;
            }
            post_queue.push(stack_temp);
            stack.pop();
          }
          stack.push(temp);
        }
        else
        {
          throw std::invalid_argument("wrong infix expression");
        }
      }
    }
    while (!stack.empty())
    {
      std::string temp = stack.top();
      if (temp == "(")
      {
        throw std::logic_error("no closing bracket");
      }
      post_queue.push(temp);
      stack.pop();
    }
  }

  bool isOverflowedAdd(long long a, long long b)
  {
    if (b > 0 && a > std::numeric_limits< long long >::max() - b)
    {
      return true;
    }
    if (b < 0 && a < std::numeric_limits< long long >::min() - b)
    {
      return true;
    }
    return false;
  }

  bool isOverflowedSubstr(long long a, long long b)
  {
    if (b > 0 && a < std::numeric_limits< long long >::min() + b)
    {
      return true;
    }
    if (b < 0 && a > std::numeric_limits< long long >::max() + b)
    {
      return true;
    }
    return false;
  }

  bool isOverflowedMult(long long a, long long b)
  {
    if (a == 0 || b == 0)
    {
      return false;
    }
    if (a > std::numeric_limits< long long >::max() / b)
    {
      return true;
    }
    if (a < std::numeric_limits< long long >::min() / b)
    {
      return true;
    }
    return false;
  }

  bool isOverflowedDivide(long long a, long long b)
  {
    if (b == 0)
    {
      throw std::logic_error("division by zero");
    }
    if (a == std::numeric_limits< long long >::min() && b == -1)
    {
      return true;
    }
    return false;
  }

<<<<<<< HEAD
  bool isOverflowedDivide(long long a, long long b)
  {
    const long long min = std::numeric_limits<long long>::min();
    return (a == min && b == -1);
  }

  long long calculateExpr(shramko::Queue<std::string>& post)
  {
    shramko::Stack<long long> stack;
=======
  long long calculateExpr(shramko::Queue< std::string >& post)
  {
    shramko::Stack< long long > stack;
>>>>>>> master
    while (!post.empty())
    {
      std::string temp = post.front();
      post.pop();
      try
      {
        long long val = std::stoll(temp);
        stack.push(val);
      }
      catch (...)
      {
        if (stack.size() < 2)
        {
          throw std::logic_error("wrong postfix expression");
        }
        long long val1 = stack.top();
        stack.pop();
        long long val2 = stack.top();
        stack.pop();
        if (temp == "+")
        {
          if (isOverflowedAdd(val2, val1))
          {
            throw std::overflow_error("overflow/underflow with add");
          }
          stack.push(val2 + val1);
        }
        else if (temp == "-")
        {
          if (isOverflowedSubstr(val2, val1))
          {
            throw std::overflow_error("overflow/underflow with subtract");
          }
          stack.push(val2 - val1);
        }
        else if (temp == "*")
        {
          if (isOverflowedMult(val2, val1))
          {
            throw std::overflow_error("overflow/underflow with multiply");
          }
          stack.push(val2 * val1);
        }
        else if (temp == "/")
        {
          if (isOverflowedDivide(val2, val1))
          {
            throw std::overflow_error("overflow/underflow with divide");
          }
          stack.push(val2 / val1);
        }
        else if (temp == "%")
        {
          if (isOverflowedDivide(val2, val1))
          {
            throw std::overflow_error("overflow/underflow with modulo");
          }
          if (val2 < 0)
          {
            stack.push(val2 % val1 + (val1 > 0 ? val1 : -val1));
          }
          else
          {
            stack.push(val2 % val1);
          }
        }
        else
        {
          throw std::invalid_argument("wrong postfix expression");
        }
      }
    }
    if (stack.size() != 1)
    {
      throw std::logic_error("wrong postfix expression");
    }
    return stack.top();
  }
}

int main(int argc, const char* const* argv)
{
  using namespace shramko;
<<<<<<< HEAD
  Queue<Queue<std::string>> inf_exprs;
  Stack<long long> results;
=======
  Queue< Queue< std::string > > inf_exprs;
  Stack< long long > results;
>>>>>>> master
  try
  {
    if (argc == 1)
    {
      input(std::cin, inf_exprs);
    }
    else
    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "Error: file not found\n";
        return 1;
      }
      input(file, inf_exprs);
    }
    while (!inf_exprs.empty())
    {
<<<<<<< HEAD
      shramko::Queue<std::string> temp_inf;
      shramko::Queue<std::string>& front_ref = inf_exprs.front();
=======
      Queue< std::string > temp_inf;
      Queue< std::string >& front_ref = inf_exprs.front();
>>>>>>> master
      while (!front_ref.empty())
      {
        temp_inf.push(front_ref.front());
        front_ref.pop();
      }
      inf_exprs.pop();
      if (!temp_inf.empty())
      {
<<<<<<< HEAD
        shramko::Queue<std::string> temp_post;
=======
        Queue< std::string > temp_post;
>>>>>>> master
        convertInfToPost(temp_inf, temp_post);
        results.push(calculateExpr(temp_post));
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  outputStack(std::cout, results);
  std::cout << "\n";
  return 0;
}
