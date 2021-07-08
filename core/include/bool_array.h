#ifndef BOOL_ARRAY__H
#define BOOL_ARRAY__H

class BoolArray
{
  const size_t   m_arraySize;
  uint32_t     * m_boolArray;
  
public:
  BoolArray( const size_t size )
  : m_arraySize( 1 + size / 32 )
  , m_boolArray( new uint32_t [ m_arraySize ] {} )
  {}
  
  ~BoolArray()
  {
    delete[] m_boolArray;
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
  
  void reset()
  {
    uint32_t * P = m_boolArray;
    for( int id = 0; id < m_arraySize; ++ id )
      * ( P ++ ) = 0;
  }
};

#endif