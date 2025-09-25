#ifndef BUCKET_HPP
#define BUCKET_HPP

#include <utility>

namespace averenkov::detail
{
  template< class Key, class Value >
  struct Bucket
  {
    Key key;
    Value value;
    bool occupied = false;
    bool deleted = false;
  };
}

#endif
