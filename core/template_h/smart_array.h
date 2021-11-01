#ifndef OWN_ARRAY__H
#define OWN_ARRAY__H

#include <memory>


template < typename T >
class Array
 : public std::enable_shared_from_this< Array<T> >
{

  mutable const T * nextData;

public:
  const size_t   size;
  const T *      data;
  const uint64_t value;
  Array( const size_t s, const T * d )
    : size ( s )
    , data ( d )
    , value( 0 )
  {

  }

  Array( const size_t s, const T * d, const uint64_t v )
    : size ( s )
    , data ( d )
    , value( v )
  {

  }
  ~Array()
  {
    delete[] data;
  }

  std::shared_ptr< Array< T > > get() const
  {
    return this -> shared_from_this();
  }

  bool start() const
  {
    nextData = data;
    return size > 0;
  }

  bool next( T & d ) const
  {
    if ( nextData - data == size )
      return false;
    d = *( nextData ++ );
    return true;
  }
};

template< typename T >
using pArray = std::shared_ptr< Array<T> >;

#endif  //  ! OWN_ARRAY__H
