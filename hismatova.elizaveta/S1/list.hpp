#ifndef LIST_HPP
#define LIST_HPP
#include "node.hpp"
#include "iterator.hpp"

namespace hismatova
{
  template < typename T >
  struct List
  {
    ~List();
    List();
    List(const List< T >& num);
    Iterator< T > begin() const;
    Iterator< T > end() const;
    T& front();
    T& back();
    bool empty() const noexcept;
    size_t size() const noexcept;
    void push_front(const T& data);
    void pop_front();
    void swap(List< T >& num);
    void clear();
    void reverse();
  private:
    size_t size;
    Node < T >* fakeNode;
  };
  template< typename T >
  List<T>::~List()
  {
    clear();
    delete[] reinterpret_cast< char* >(fakeNode);
  }
  template< typename T >
  T& List< T >::front()
  {
    return fakeNode->next->data;
  }
  template< typename T>
  T& List< T >::back()
  {
    Node< T >* current = fakeNode;
    while (current->next != fakeNode)
    {
      current = current->next;
    }
    return current->data;
  }
  template< typename T >
  bool List< T >::empty const noexcept
  {
    return size == 0;
  }
  template< typename T >
  size_t List< T >::size() const noexcept
  {
    return size;
  }
  template< typename T >
  Iterator< T > List< T >::begin() const
  {
    return Iterator< T >(fakeNode->next);
  }
  template< typename T >
  Iterator< T > List< T >::end() const
  {
    return Iterator< T >(fakeNode);
  }
}

#endif
