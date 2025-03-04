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
  template< typename T >
  void List< T >::push_front(const T& data)
  {
    Node* newNode = new Node{data, fakeNode->next};
    fakeNode->next = newNode;
    size++;
  }
  template< typename T >
  List< T >::List(const List< T >& num):
    List()
  {
    auto temp = num.begin();
    for (size_t i = num.size(); i > 0; i--)
    {
      push_front(*temp);
      temp++;
    }
    reverse();
  }
  template< typename T >
  void List< T >::clear()
  {
    Node< T >* temp = fakeNode->next;
    while (size)
    {
      Node< T >* current = temp->next;
      delete temp;
      temp = current;
      size--;
    }
  }
  template< class T >
  void List< T >::reverse()
  {
    if (fakeNode->next == fakeNode)
    {
      return;
    }
    Node< T >* next = fakeNode->next;
    Node< T >* temp = fakeNode;
    while (next->next != fakeNode)
    {
      Node< T >* temp2 = next->next;
      next->next = temp;
      temp = next;
      next = temp2;
    }
    next->next = temp;
    fakeNode->next = next;
  }
  template< typename T >
  List< T >::List():
    fakeNode(reinterpret_cast< NodeList< T >* >(new char[sizeof(NodeList< T >)])),
    size(0)
  {
    fakeNode->next = fakeNode;
  }
  template< typename T >
  void List< T >::swap(List< T >& num)
  {
    std::swap(fakeNode, num.fakeNode);
    std::swap(size, num.size);
  }
  template < typename T >
  void List< T >::pop_front()
  {
    fakeNode->next = fakeNode->next->next;
    if (fakeNode->next == fakeNode)
    {
      return;
    }
    delete fakeNode->next;
    size--;
  }
}

#endif
