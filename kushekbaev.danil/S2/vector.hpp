#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <stdexcept>

namespace kushekbaev
{
  template< typename T >
  class RAII
  {
    public:
      RAII(size_t cap_);
      ~RAII();
      void move(T* src, size_t i);
      T* release();
    private:
      T* data_;
      size_t size_;
  };

  template< typename T >
  RAII< T >::RAII(size_t cap_):
    data_(new T[cap_]),
    size_(0)
  {}

  template< typename T >
  RAII< T >::~RAII()
  {
    delete[] data_;
  }

  template< typename T >
  void RAII< T >::move(T* src, size_t j)
  {
    data_[size_] = std::move(src[j]);
    ++size_;
  }

  template< typename T >
  T* RAII< T >::release()
  {
    T* result = data_;
    data_ = nullptr;
    size_ = 0;
    return result;
  }

  template< typename T >
  class Vector
  {
    public:
      Vector();
      Vector(const Vector& other);
      Vector(Vector&& other);
      ~Vector();

      Vector& operator=(const Vector& other);

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
      void resize_array();
  };

  template< typename T >
  Vector< T >::Vector():
    data_(new T[1]),
    size_(0),
    capacity_(1)
  {}

  template< typename T >
  Vector< T >::Vector(const Vector& other):
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
  Vector< T >::Vector(Vector&& other):
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
      resize_array();
    }
    data_[size_++] = value;
  }

  template< typename T >
  void Vector< T >::pop_front()
  {
    if (empty())
    {
      throw std::out_of_range("The array is empty!");
    }
    for (size_t i = 1; i < size_; ++i)
    {
      data_[i - 1] = std::move(data_[i]);
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
  void Vector< T >::resize_array()
  {
    size_t newCapacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    RAII< T > tmp(newCapacity);
    for (size_t i = 0; i < size_; ++i)
    {
      tmp.move(data_, i);
    }
    for (size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }
    delete[] data_;
    data_ = tmp.release();
    capacity_ = newCapacity;
  }
}

#endif
