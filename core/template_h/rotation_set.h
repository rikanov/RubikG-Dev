#ifndef BITMANIP__H
#define BITMANIP__H

#include <base_types.h>

template< cube_size N >
class CRotationSet
{
  static CRotationSet * Singleton;
  
  uint16_t * m_inverse;
  constexpr unsigned int m_length = 3 * N;
  
  CRotationSet();
  ~CRotationSet();
  
  static void Permute3( BitMapID & rotationSet, const uint8_t c, const uint8_t b, const uint8_t a );
  void init();
  
public:
  
  static void Init();
  static void OnExit();
  static BitMapID Transform( const BitMapID, const CubeID );
};

template< cube_size N >
CRotationSet * CRotationSet::Singleton = nullptr;

template< cube_size N >
CRotationSet::CRotationSet()
 :  m_inverse( nullptr )
{
  
}

template< cube_size N >
CRotationSet::~CRotationSet()
 :  m_inverse( nullptr )
{
  delete[] m_inverse;
}

template< cube_size N >
void CRotationSet::init()
{
  m_inverse = new uint16_t [ 1 << ( length * 3 ) ];
}

template< cube_size N >
void CRotationSet::Permute3( BitMapID & rotationSet, const uint8_t c, const uint8_t b, const uint8_t a )
{
  const BitMapID mask = ( 1ULL << m_length ) - 1;
  const BitMapID A = ( rotationSet                 ) & mask;
  const BitMapID B = ( rotationSet >>     m_length ) & mask;
  const BitMapID C = ( rotationSet >> 2 * m_length ) & mask;
  rotationSet =  A << ( a * m_length );
  rotationSet |= B << ( b * m_length );
  rotationSet |= C << ( c * m_length );
}

#endif  //  ! BITMANIP__H