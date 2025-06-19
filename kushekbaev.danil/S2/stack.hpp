#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include "vector.hpp"

namespace kushekbaev
{
  template< typename T >
  class Stack
  {
    public:
      Stack() = default;
      Stack(const Stack& other) noexcept;
      Stack(Stack&& other) noexcept;
      ~Stack() = default;

      Stack& operator=(const Stack& other) noexcept;
      Stack& operator=(Stack&& other) noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      T& top() noexcept;
      const T& top() const noexcept;

      void push(const T& value);
      T drop();
      void pop();

    private:
      Vector< T > vector_;
  };

  template< typename T >
  Stack< T >::Stack(const Stack& other) noexcept:
    vector_(other.vector_)
  {}

  template< typename T >
  Stack< T >::Stack(Stack&& other) noexcept:
    vector_(std::move(other.vector_))
  {}

  template< typename T >
  Stack< T >& Stack< T >::operator=(const Stack& other) noexcept
  {
    if (this != &other)
    {
      vector_ = other.vector_;
    }
    return *this;
  }

  template< typename T >
  Stack< T >& Stack< T >::operator=(Stack&& other) noexcept
  {
    if (this != &other)
    {
      vector_ = std::move(other.vector_);
    }
    return *this;
  }

  template< typename T >
  bool Stack< T >::empty() const noexcept
  {
    return vector_.empty();
  }

  template< typename T >
  size_t Stack< T >::size() const noexcept
  {
    return vector_.size();
  }

  template< typename T >
  T& Stack< T >::top() noexcept
  {
    return vector_.back();
  }

  template< typename T >
  const T& Stack< T >::top() const noexcept
  {
    return vector_.back();
  }

  template< typename T >
  void Stack< T >::push(const T& value)
  {
    return vector_.push_back(value);
  }

  template< typename T >
  T Stack< T >::drop()
  {
    if (vector_.empty())
    {
      throw std::out_of_range("The vector in the queue is empty!");
    }
    T tmp = 0;
    try
    {
      tmp = std::move(top());
      vector_.pop_back();
    }
    catch (...)
    {
      throw std::runtime_error("Error in drop");
    }
    return tmp;
  }

  template< typename T >
  void Stack< T >::pop()
  {
    if (vector_.empty())
    {
      throw std::out_of_range("The Vector in the queue is empty!");
    }
    vector_.pop_back();
  }
}

#endif
