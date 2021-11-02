#ifndef FACTORY_COM_INTERFACE__H
#define FACTORY_COM_INTERFACE__H

#include <memory>

template< typename Type, typename Val = size_t >
class PackagedData
{

  PackagedData( const size_t s, const Type * a )
    : size( s )
    , array( a )
    , value( 0 )
  {
  }

  PackagedData( const size_t s, const Type * a, const Val v )
    : size( s )
    , array( a )
    , value( v )
  {
  }

public:
  PackagedData() = delete;
  const size_t   size;
  const Type   * array;
  const Val      value;

  static std::shared_ptr< PackagedData<Type, Val> > Send( const size_t s, const Type * a )
  {
    return std::make_shared< PackagedData<Type, Val> > ( PackagedData<Type, Val>( s, a ) );
  }

  static std::shared_ptr< PackagedData<Type, Val> > Send( const size_t s, const Type * a, const Val v )
  {
    return std::make_shared< PackagedData<Type, Val> > ( PackagedData<Type, Val> ( s, a, v ) );
  }
};

#endif  //  ! FACTORY_COM_INTERFACE__H
