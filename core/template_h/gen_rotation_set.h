#ifndef BITMANIP__H
#define BITMANIP__H

#include <base_types.h>
#include <simplex.h>

template< cube_size N >
class GenerateRotationSet
{
  static constexpr unsigned int m_length  = 3 * N;
  
  GenerateRotationSet() = default;
  
  static void FlipAxis  ( BitMapID & , const Axis );
  static void Permute   ( BitMapID & , const Axis, const Axis, const Axis );
  static Axis TransAxis ( BitMapID & , const Axis, const CubeID );
public:
 
  static void Transform( BitMapID &, const CubeID );
};

template< cube_size N >
void GenerateRotationSet<N>::FlipAxis( BitMapID & rotSetID , const Axis a )
{
  constexpr BitMapID mask = ( 1ULL << m_length ) - 1;
  BitMapID shadowing = 0;
  BitMapID flipped   = 0;
  for( Axis axis: { _X, _Y, _Z } )
  {
    if ( axis != a )
    {
      shadowing |= mask << ( axis * m_length );
    }
  }
  size_t right = 1ULL << ( a * m_length );
  size_t left  = 1ULL << ( ( a + 1 ) * m_length - 1 );
  for( int prog = 0; prog < m_length; ++ prog )
  {
    if ( rotSetID & right )
      flipped |= left;

    left  >>= 1;
    right <<= 1;
  }
  rotSetID &= shadowing;
  rotSetID |= flipped;
}

template< cube_size N >
void GenerateRotationSet<N>::Permute( BitMapID & rotSetID, const Axis x, const Axis y, const Axis z )
{
  constexpr BitMapID mask = ( 1ULL << m_length ) - 1;
  const BitMapID X = ( rotSetID                 ) & mask;
  const BitMapID Y = ( rotSetID >>     m_length ) & mask;
  const BitMapID Z = ( rotSetID >> 2 * m_length ) & mask;
  
  rotSetID  = X << ( x * m_length );
  rotSetID |= Y << ( y * m_length );
  rotSetID |= Z << ( z * m_length );
}


template< cube_size N >
Axis GenerateRotationSet<N>::TransAxis( BitMapID & rotSetID, const Axis axis, const CubeID cid )
{ 
  const Orient axisBase = GetAxisBase( axis );
  
  switch( Simplex::GetCube( cid ).whereIs( axisBase ) )
  {
    case _F:
      FlipAxis( rotSetID, axis );
      return _Z;

    case _R:
      FlipAxis( rotSetID, axis );
      return _X;

    case _U:
      FlipAxis( rotSetID, axis );
      return _Y;

    case _D:
      return _Y;

    case _L:
      return _X;

    case _B:
      return _Z;
 
    default:
      return _NA;
  }
}

template< cube_size N >
void GenerateRotationSet<N>::Transform( BitMapID & rotSetID , const CubeID cid )
{
  rotSetID >>= 1; // omit 0 place ie. empty rotation
  const Axis x = TransAxis( rotSetID, _X, cid );
  const Axis y = TransAxis( rotSetID, _Y, cid );
  const Axis z = TransAxis( rotSetID, _Z, cid );

  Permute( rotSetID, x, y, z );
  rotSetID <<= 1;
}

#endif  //  ! BITMANIP__H
