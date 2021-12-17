#ifndef ___DYNAMIC_ARRAY__H
#define ___DYNAMIC_ARRAY__H

#include <memory>
#include <text_output.h>
#include <initializer_list>

template<class Type>
class Array
{
protected:
  size_t                  m_size;
  std::shared_ptr<Type[]> m_array;

public:
  Array()
    : m_size( 0 )
    , m_array ( nullptr )
    {}

  Array( const size_t size )
    : m_size ( size )
    , m_array( new Type [size] {} )
    {}

  void fill( const Type * beg, const Type * end )
  {
    m_array = std::shared_ptr<Type[]> ( new Type [ end - beg ] );
    for ( size_t id = 0; beg != end; ++ id, ++ beg )
    {
      m_array[id] = *beg;
    }
  }

  Type & get( const size_t & id )
  {
    return m_array[id];
  }

  const Type & get( const size_t & id ) const
  {
    return m_array[id];
  }
  Type & operator[]( const size_t & id )
  {
    return get( id );
  }

  const Type & operator[]( const size_t & id ) const
  {
    return get( id );
  }

  Type * operator() ( const size_t & id )
  {
    return begin() + id;
  }

  const Type * operator() ( const size_t & id ) const
  {
    return begin() + id;
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
    return m_array.get();
  }

  Type * end()
  {
    return m_array.get() + m_size;
  }

  const Type * begin() const
  {
    return m_array.get();
  }

  const Type * end() const
  {
    return m_array.get() + m_size;
  }

  void init( const size_t size, const Type * data )
  {
    if ( m_size != size )
    {
      m_size = size;
      m_array = std::shared_ptr<Type[]> ( new Type[size] {} );
    }
    for ( size_t i = 0; i < size; ++ i )
    {
      m_array[i] = data[i];
    }
  }

  void init( const std::initializer_list<Type> & list )
  {
    init( list.size(), list.begin() );
  }
};

#endif  //  ! ___DYNAMIC_ARRAY__H
