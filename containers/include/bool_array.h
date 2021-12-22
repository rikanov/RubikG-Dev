#ifndef ___BOOL_ARRAY__H
#define ___BOOL_ARRAY__H

#include <dynamic_array.h>

class BoolArray
{
  Array<uint32_t> m_boolArray;
  
public:
  BoolArray() = default;
  
  BoolArray( const size_t size )
    : m_boolArray( size == 0 ? 0 : 1 + size / 32 )
  {}
  
  void resize( const size_t size )
  {
    m_boolArray = Array<uint32_t> ( ( size == 0 ) ? 0 : 1 + size / 32 );
  }
  
  void set( const uint32_t index, const bool t = true )
  {
    if ( t )
      m_boolArray[ index / 32 ] |=  ( 1ULL << ( index % 32 ) );
    else
      m_boolArray[ index / 32 ] &= ~( 1ULL << ( index % 32 ) );  
  }
  
  bool get( const uint32_t index ) const
  {
    return m_boolArray[ index / 32 ] & ( 1ULL << ( index % 32 ) );
  }
  
  inline bool operator[] ( const uint32_t index ) const
  {
    return get( index );
  }
  
  inline bool at( const uint32_t index ) const
  {
    return get( index );
  }
 
  void clean()
  {
    uint32_t * P = m_boolArray.begin();
    for( int id = 0; id < m_boolArray.size(); ++ id )
      * ( P ++ ) = 0;
  }

  bool operator == ( const BoolArray & ba ) const
  {
    if ( m_boolArray.size() != ba.m_boolArray.size() )
    {
      return false;
    }
    for ( size_t i = 0; i < m_boolArray.size(); ++ i )
    {
      if ( m_boolArray[i] != ba.m_boolArray[i] )
      {
        return false;
      }
    }
    return true;
  }
};

#endif  //  ! ___BOOL_ARRAY__H
