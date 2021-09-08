#ifndef BITMANIP__H
#define BITMANIP__H

#include <base_types.h>

template< cube_size N >
class CRotationSet
{
  constexpr unsigned int m_length  = 3 * N;
  constexpr unsigned int m_3length = 9 * N;
  
  CRotationSet() = default;
  
  static void Permute( BitMapID & , const uint8_t, const uint8_t, const uint8_t );
  static BitMapID Inverse( const BitMapID, const uint8_t );
  
public:
 
  static BitMapID Transform( const BitMapID, const CubeID );
};

template< cube_size N >
BitMapID CRotationSet::inverse( const BitMapID rotSetID , const uint8_t b )
{
  BitMapID result = 0;
  size_t A = 1ULL << ( b + m_3length - 1);
  size_t B = 1ULL << b;
  for( int prog = 0; prog < m_3length, ++ prog )
  {
    if ( rotSetID | A )
      result |= B;
    
    A >>= 1;
    B <<= 1;
  } 
  return result;
}

template< cube_size N >
void CRotationSet::Permute3( BitMapID & rotationSet, const uint8_t c, const uint8_t b, const uint8_t a )
{
  constexpr BitMapID mask = ( 1ULL << m_length ) - 1;
  const BitMapID A = ( rotationSet                 ) & mask;
  const BitMapID B = ( rotationSet >>     m_length ) & mask;
  const BitMapID C = ( rotationSet >> 2 * m_length ) & mask;
  rotationSet =  A << ( a * m_length );
  rotationSet |= B << ( b * m_length );
  rotationSet |= C << ( c * m_length );
}

#endif  //  ! BITMANIP__H