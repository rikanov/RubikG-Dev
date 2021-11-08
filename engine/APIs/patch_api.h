#ifndef PATCH_API__H
#define PATCH_API__H

#include <cube_rotations.h>
#include <smart_array.h>

template< cube_size N >
class PatchAPI
{
  pArray< PosID > m_patch;
  BitMapID        m_priorRotIDs;

public:
  PatchAPI() = default;
  void init( pArray< PosID > );
  
  size_t size() const
  {
    return m_patch -> size;    
  }

  PosID getPriorPos() const
  {
    return m_patch -> data[ m_patch -> size - 1 ];
  }

  PosID getPosID( const size_t id ) const
  {
    return m_patch -> data[id];
  }
  
  BitMapID priorRotIDs() const
  {
    return m_priorRotIDs;
  }

  bool priorMoving( const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << rotID );
  }

  bool priorMoving( const CubeID prior, const RotID rotID ) const
  {
    return m_priorRotIDs & ( 1ULL << ( CRotations<N>::GetRotID( rotID, Simplex::Inverse( prior ) ) ) );
  }

  bool movePrior( CubeID & prior, const RotID rotID ) const;
  bool valid( GroupID ) const;
};

template< cube_size N >
void PatchAPI<N>::init( pArray< PosID > patch )
{
  m_patch = patch;
  m_priorRotIDs = CPositions<N>::ActOn( getPriorPos() );
}

template< cube_size N >
bool PatchAPI<N>::movePrior( CubeID & prior, const RotID rotID ) const
{
  const Coord position = CPositions<N>::GetCoord( getPriorPos(), prior );
  if ( priorMoving( prior, rotID ) )
  {
    prior = CRotations<N>::Tilt( prior, rotID );
    return true;
  }
  return false;
}

template< cube_size N >
bool PatchAPI<N>::valid( GroupID gid ) const
{
  BoolArray positions( CPositions<N>::GetSize() );

  for ( size_t index = 0; index < m_patch -> size; ++ index, gid /= 24 )
  {
    const PosID next = CPositions<N>::GetPosID( getPosID( index ), gid % 24 );
    if ( positions( next ) )
      return false;
    positions.set( next, true );
  }
  return true;
}

#endif  //  ! PATCH_API__H
