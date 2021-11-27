#ifndef ___DYNAMIC_ARRAY__H
#define ___DYNAMIC_ARRAY__H

#include <memory>

template<class Type>
class Array
{
  size_t                  m_size;
  std::shared_ptr<Type[]> m_array;

public:
  Array()
    : m_size( 0 )
    , m_array ( nullptr )
    {}

  Array( const size_t size )
    : m_size ( size )
    , m_array( new Type [size] )
    {}

  Type & operator[]( const size_t & id )
  {
    return m_array[id];
  }

  const Type & operator[]( const size_t & id ) const
  {
    return m_array[id];
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

  Type * begin()
  {
    return m_array.raw();
  }

  Type * end()
  {
    return m_array.raw() + m_size;
  }

  const Type * begin() const
  {
    return m_array.raw();
  }

  const Type * end() const
  {
    return m_array.raw() + m_size;
  }
};

#endif  //  ! ___DYNAMIC_ARRAY__H
