#ifndef LIST_HPP
#define LIST_HPP

namespace hismatova
{
  template < typename T >
  struct Iterator
  {
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    T& operator->();
    T& operator*();
  }
  template < typename T >
  strict FwdList
  {
    Iterator< T > begin() const;
    Iterator< T > end() const;
    void push_front(const T&);
  };
}

#endif
