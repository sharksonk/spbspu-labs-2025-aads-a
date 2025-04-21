#ifndef BITREE_ITERATOR_HPP
#define BITREE_ITERATOR_HPP
#include "bitree.hpp"
namespace karnauhova
{
  template< typename Key, typename Value, typename Compare = std::less< Key >>
  struct BiTreeIterator
  {
  public:
    bool hasPrev() const;
    bool hasNext() const;
    BiTreeIterator< Key, Value > next() const;
    BiTreeIterator< Key, Value > prev() const;
    
    const Value& value() const;
  private:
    karnauhova::BiTree< Key, Value >* node_;
  };

  template< typename Key, typename Value, typename Compare>
  bool BiTreeIterator< Key, Value, Compare >::hasPrev() const
  {
    if (node_ == nullptr)
    {
        return false;
    }
    if (node->left)
    {
        return true;
    }
    BiTree< Key, Value >* last = node_;
    while(last->parent)
    {
        BiTree< Key, Value >* parent = last->parent;
        if (parent->right == last)
        {
        return true;
        }
        last = last->parent;
    }
    return false;
    }

    template< class T >
    bool BiTreeIterator< T >::hasNext() const
    {
    if (node == nullptr)
    {
        return false;
    }
    if (node->right)
    {
        return true;
    }
    BiTree< T >* last = node;
    while(last->parent)
    {
        BiTree< T >* parent = last->parent;
        if (parent->left == last)
        {
        return true;
        }
        last = last->parent;
    }
    return false;
    }

    template< class T >
    BiTreeIterator< T > BiTreeIterator< T >::next() const
    {
    if (!hasNext())
    {
        throw std::logic_error("Error: no next");
    }
    if (node->right)
    {
        BiTree< T >* current = node->right;
        while (current->left)
        {
        current = current->left;
        }
        BiTreeIterator< T > it{current};
        return it;
    }
    BiTree< T >* parent = node->parent;
    BiTree< T >* last = node;
    while (parent->left != last && parent->parent)
    {
        last = last->parent;
        parent = parent->parent;
    }
    BiTreeIterator< T > it{parent};
    return it;
    }

    template< class T >
    BiTreeIterator< T > BiTreeIterator< T >::prev() const
    {
    if (!hasPrev())
    {
        throw std::logic_error("Error: no prev");
    }
    if (node->left)
    {
        BiTree< T >* current = node->left;
        while (current->right)
        {
        current = current->right;
        }
        BiTreeIterator< T > it{current};
        return it;
    }
    BiTree< T >* parent = node->parent;
    BiTree< T >* last = node;
    while (parent->right != last && parent->parent)
    {
        last = last->parent;
        parent = parent->parent;
    }
    BiTreeIterator< T > it{parent};
    return it;
    }

    template< class T >
    const T& BiTreeIterator< T >::data() const
    {
    return node->data;
    }
}
#endif
