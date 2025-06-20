#ifndef BUCKET_HPP
#define BUCKET_HPP

namespace smirnov
{
  template< class Key, class Value >
  struct Bucket
  {
    Key key;
    Value value;
    bool occupied = false;
    Bucket():
      key(),
      value(),
      occupied(false)
    {}
    Bucket(const Key & k, const Value & v):
      key(k),
      value(v),
      occupied(true)
    {}
  };
}
#endif
