#ifndef FACTORY_ARRAY__H
#define FACTORY_ARRAY__H

#include <memory>

template< cube_size N >
template<class Type>
class Factory<N>::Array
{
  size_t                m_size;
  std::shared_ptr<Type> m_array;
public:
  Array()
    : m_size( 0 )
    , m_array ( nullptr )
  {}

  Array( const size_t size )
    : m_size( size )
    , m_array( new Type[size] {}, []( Type * p ){ delete[] p; } )
    {}

  Type & operator[]( const size_t & id )
  {
    return m_array.get()[id];
  }

  const Type & operator[]( const size_t & id ) const
  {
    return m_array.get()[id];
  }

  Type * get()
  {
    return m_array.get();
  }

  const Type * get() const
  {
    return m_array.get();
  }

  size_t size() const
  {
    return m_size;
  }

  Array & operator= ( const Array<Type> array )
  {
    m_size  = array.size();
    m_array = array.m_array;
    return *this;
  };
};

#endif  //  ! FACTORY_ARRAY__H
