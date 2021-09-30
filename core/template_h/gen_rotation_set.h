#ifndef BITMANIP__H
#define BITMANIP__H

#include <base_types.h>
#include <simplex.h>

template< cube_size N >
class GenerateRotationSet
{
  static constexpr unsigned m_length  = 3 * N;
  static constexpr BitMapID m_mask    = ( 1ULL << m_length ) - 1;

  static const BitMapID * m_flipped;
  static const BitMapID   * m_shadowing;

  GenerateRotationSet() = default;
  
  static void FlipAxis  ( BitMapID & , const Axis );
  static void Permute   ( BitMapID & , const Axis, const Axis, const Axis );
  static Axis TransAxis ( BitMapID & , const Axis, const CubeID );

  static void createFlipCache();
  static void createShadowing();

public:
 
  static void Instance();
  static void OnExit();
  static void Transform( BitMapID &, const CubeID );
};

 // Instantiations
// ---------------
template< cube_size N >
const BitMapID * GenerateRotationSet<N>:: m_flipped = nullptr;

template< cube_size N >
const BitMapID * GenerateRotationSet<N>:: m_shadowing = nullptr;
// ---------------------------------------------------------- //


template<cube_size N>
void GenerateRotationSet<N>::Instance()
{
  if ( nullptr == m_flipped )
  {
    createFlipCache();
    createShadowing();
  }
}

template<cube_size N>
void GenerateRotationSet<N>::OnExit()
{
  delete[] m_flipped;   m_flipped   = nullptr;
  delete[] m_shadowing; m_shadowing = nullptr;
}

template<cube_size N>
void GenerateRotationSet<N>::createFlipCache()
{
  BitMapID * flipped   = new BitMapID [ 1 << m_length ] {};

  for ( BitMapID rotSetID = 0; rotSetID < ( 1 << m_length ); ++ rotSetID )
  {
    for( int prog = 0; prog < m_length; ++ prog )
    {
      if ( rotSetID & ( 1ULL << prog ) )
        flipped[ rotSetID ] |= ( 1ULL << ( m_length - prog - 1 ) );

    }
  }
  m_flipped   = flipped;
}

template< cube_size N >
void GenerateRotationSet<N>::createShadowing()
{
  BitMapID   * shadowing = new BitMapID   [ 3 ] {};
  for ( Axis axis: { _X, _Y, _Z } )
   for ( Axis excl: {_X, _Y, _Z } )
     if ( axis != excl )
       shadowing[ axis ] |= m_mask << ( excl * m_length );

  m_shadowing = shadowing;
}


template< cube_size N >
void GenerateRotationSet<N>::FlipAxis( BitMapID & rotSetID , const Axis axis )
{
  const BitMapID flippedPart = m_flipped[ ( rotSetID >> ( axis * m_length ) ) & m_mask ];
  rotSetID &= m_shadowing [ axis ];
  rotSetID |= flippedPart << ( axis * m_length );
}

template< cube_size N >
void GenerateRotationSet<N>::Permute( BitMapID & rotSetID, const Axis x, const Axis y, const Axis z )
{
  const BitMapID X = ( rotSetID                 ) & m_mask;
  const BitMapID Y = ( rotSetID >>     m_length ) & m_mask;
  const BitMapID Z = ( rotSetID >> 2 * m_length ) & m_mask;
  
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
  if ( rotSetID < 2 )
    return; // do nothing with gradients belong to solved or unsolvable states 

  rotSetID >>= 1; // omit 0 place ie. empty rotation
  const Axis x = TransAxis( rotSetID, _X, cid );
  const Axis y = TransAxis( rotSetID, _Y, cid );
  const Axis z = TransAxis( rotSetID, _Z, cid );

  Permute( rotSetID, x, y, z );
  rotSetID <<= 1;
}

#endif  //  ! BITMANIP__H
