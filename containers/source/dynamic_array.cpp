#include <dynamic_array.h>


template<class Type>
Array<Type>::Array()
  : m_size( 0 )
  , m_array ( nullptr )
{

}

template<class Type>
Array<Type>::Array( const size_t size )
  : m_size ( size )
  , m_array( new Type [size] {} )
{

}

template<class Type>
void Array<Type>::fill( const Type * beg, const Type * end )
{
  const int d = ( beg < end ) ? 1 : -1;
  for ( size_t id = 0; beg != end; ++ id, beg += d )
  {
    m_array[id] = *beg;
  }
}

template<class Type>
Type & Array<Type>::get( const size_t & id )
{
  return m_array[id];
}

template<class Type>
const Type & Array<Type>::get( const size_t & id ) const
{
  return m_array[id];
}

template<class Type>
Type * Array<Type>::at( const size_t & id )
{
  return begin() + id;
}

template<class Type>
const Type * Array<Type>::at( const size_t & id ) const
{
  return begin() + id;
}

template<class Type>
Type & Array<Type>::operator[]( const size_t & id )
{
  return get( id );
}

template<class Type>
const Type & Array<Type>::operator[]( const size_t & id ) const
{
  return get( id );
}

template<class Type>
Type * Array<Type>::operator() ( const size_t & id )
{
  return at( id );
}

template<class Type>
const Type * Array<Type>::operator() ( const size_t & id ) const
{
  return at( id );
}

template<class Type>
size_t Array<Type>::size() const
{
  return m_size;
}

template<class Type>
Array<Type> & Array<Type>::operator = ( const Array<Type> array )
{
  m_size  = array.size();
  m_array = array.m_array;
  return *this;
};

template<class Type>
Type * Array<Type>::begin()
{
  return m_array.get();
}

template<class Type>
Type * Array<Type>::end()
{
  return m_array.get() + m_size;
}

template<class Type>
const Type * Array<Type>::begin() const
{
  return m_array.get();
}

template<class Type>
const Type * Array<Type>::end() const
{
  return m_array.get() + m_size;
}

template<class Type>
void Array<Type>::init( const size_t size, const Type * data )
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

template<class Type>
void Array<Type>::init( const std::initializer_list<Type> & list )
{
  init( list.size(), list.begin() );
}

