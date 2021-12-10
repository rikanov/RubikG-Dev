#ifndef BOOL_ARRAY__H
#define BOOL_ARRAY__H

class BoolArray
{
  size_t     m_arraySize;
  uint32_t * m_boolArray;
  
public:
  BoolArray()
  : m_arraySize( 0 )
  , m_boolArray( nullptr )
  {}
  
  BoolArray( const size_t size )
  : m_arraySize( size == 0 ? 0 : 1 + size / 32 )
  , m_boolArray( size == 0 ? nullptr : new uint32_t [ m_arraySize ] {} )
  {}
  
  virtual ~BoolArray()
  {
    delete[] m_boolArray;
  }

  void resize( const size_t size )
  {
    m_arraySize = 1 + size / 32;
    delete[] m_boolArray;
    m_boolArray = ( size == 0 ) ? nullptr : new uint32_t [ m_arraySize ] {};
  }
  
  void set( const uint32_t index, const bool t )
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
  
  inline bool operator() ( const uint32_t index ) const
  {
    return get( index );
  }
  
  inline bool at( const uint32_t index ) const
  {
    return get( index );
  }
 
  void clean()
  {
    uint32_t * P = m_boolArray;
    for( int id = 0; id < m_arraySize; ++ id )
      * ( P ++ ) = 0;
  }
};

#endif