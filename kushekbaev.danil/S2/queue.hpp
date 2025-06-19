#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "vector.hpp"

namespace kushekbaev
{
  template < typename T >
  class Queue
  {
    public:
      Queue() = default;
      Queue(const Queue& other) noexcept;
      Queue(Queue&& other) noexcept;
      ~Queue() = default;

      Queue& operator=(const Queue& other) noexcept;
      Queue& operator=(Queue&& other) noexcept;

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push(const T& value);
      T drop();
      void pop();
      void clear();

    private:
      Vector< T > vector_;
  };

  template < typename T >
  Queue< T >::Queue(const Queue& other) noexcept:
    vector_(other.vector_)
  {}

  template < typename T >
  Queue< T >::Queue(Queue&& other) noexcept:
    vector_(std::move(other.vector_))
  {}

  template < typename T >
  Queue< T >& Queue< T >::operator=(const Queue& other) noexcept
  {
    if (this != &other)
    {
      vector_ = other.vector_;
    }
    return *this;
  }

  template< typename T >
  Queue< T >& Queue< T >::operator=(Queue&& other) noexcept
  {
    if (this != &other)
    {
      vector_ = std::move(other.vector_);
    }
    return *this;
  }

  template < typename T >
  T& Queue< T >::front() noexcept
  {
    return vector_.front();
  }

  template < typename T >
  T& Queue< T >::back() noexcept
  {
    return vector_.back();
  }

  template < typename T >
  const T& Queue< T >::front() const noexcept
  {
    return vector_.front();
  }

  template < typename T >
  const T& Queue< T >::back() const noexcept
  {
    return vector_.back();
  }

  template < typename T >
  bool Queue< T >::empty() const noexcept
  {
    return vector_.empty();
  }

  template < typename T >
  size_t Queue< T >::size() const noexcept
  {
    return vector_.size();
  }

  template < typename T >
  void Queue< T >::push(const T& value)
  {
    vector_.push_back(value);
  }

  template < typename T >
  T Queue< T >::drop()
  {
    if (vector_.empty())
    {
      throw std::out_of_range("The vector in the queue is empty!");
    }
    T tmp = 0;
    try
    {
      tmp = std::move(front());
      vector_.pop_back();
    }
    catch (...)
    {
      throw std::runtime_error("Error in drop");
    }
    return tmp;
  }

  template < typename T >
  void Queue< T >::pop()
  {
    if (vector_.empty())
    {
      throw std::out_of_range("The vector in the queue is empty!");
    }
    vector_.pop_front();
  }

  template < typename T >
  void Queue< T >::clear()
  {
    while (!empty())
    {
      pop();
    }
  }
}

#endif
