#ifndef ITERATOR_HPP
#define ITERATOR_HPP

namespace hismatova
{
  template < typename T >
  class List;
  template < typename T >
  class Iterator
  {
  private:
    typename List< T >::Node* current;
  public:
    explicit Iterator(typename List< T >::Node* node):
      current(node)
    {}
    T& operator*() { return current->data; }
    Iterator& operator++()
    {
      current = current->next;
      return *this;
    }
    bool operator!=(const Iterator& other) const { return current != other.current; }
  };
}
#endif
