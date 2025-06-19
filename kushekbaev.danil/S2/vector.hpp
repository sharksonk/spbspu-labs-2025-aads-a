#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class Vector
  {
    public:
      Vector() noexcept;
      Vector(const Vector& other) noexcept;
      Vector(Vector&& other) noexcept;
      ~Vector();

      Vector& operator=(const Vector& other);
      Vector& operator=(Vector&& other);

      T& front() noexcept;
      T& back() noexcept;
      const T& front() const noexcept;
      const T& back() const noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;

      void push_back(const T& value);
      void pop_front();
      void pop_back();

    private:
      T* data_;
      size_t size_;
      size_t capacity_;
      void extend_vector();
  };

  template< typename T >
  Vector< T >::Vector() noexcept:
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< typename T >
  Vector< T >::Vector(const Vector& other) noexcept:
    data_(new T[other.capacity_]),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i] = other.data_[i];
    }
  }

  template< typename T >
  Vector< T >::Vector(Vector&& other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< typename T >
  Vector< T >::~Vector()
  {
    delete[] data_;
  }

  template< typename T >
  Vector< T >& Vector< T >::operator=(const Vector& other)
  {
    if (this != &other)
    {
      T* newData = new T[other.capacity_];
      for (size_t i = 0; i < other.size_; ++i)
      {
        newData[i] = other.data_[i];
      }
      delete[] data_;
      data_ = newData;
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    return *this;
  }

  template< typename T >
  Vector< T >& Vector< T >::operator=(Vector&& other)
  {
    if (this != &other)
    {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  template< typename T >
  bool Vector< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< typename T >
  size_t Vector< T >::size() const noexcept
  {
    return size_;
  }

  template< typename T >
  T& Vector< T >::front() noexcept
  {
    return data_[0];
  }

  template< typename T >
  T& Vector< T >::back() noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  const T& Vector< T >::front() const noexcept
  {
    return data_[0];
  }

  template< typename T >
  const T& Vector< T >::back() const noexcept
  {
    return data_[size_ - 1];
  }

  template< typename T >
  void Vector< T >::push_back(const T& value)
  {
    if (size_ == capacity_)
    {
      extend_vector();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Vector< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("The Vector is empty!");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      std::move(data_[i]);
    }
    --size_;
  }

  template< typename T >
  void Vector< T >::pop_back()
  {
    if (!empty())
    {
      --size_;
    }
  }

  template< typename T >
  void Vector< T >::extend_vector()
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    T* newData = new T[newCapacity];
    size_t i = 0;
    try
    {
      for (; i < size_; ++i)
      {
        newData[i] = std::move(data_[i]);
      }
    }
    catch (...)
    {
      for (size_t j = 0; j < i; ++j)
      {
        newData[j].~T();
      }
      delete[] newData;
      throw;
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }
}

#endif