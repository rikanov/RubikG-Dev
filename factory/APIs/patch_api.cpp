#ifndef API_PATCH__H
#define API_PATCH__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <bool_array.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::PatchAPI: public Factory<N>::Patch
{
  const BitMapID  m_priorRotIDs;

public:
  PatchAPI() = default;;
  PatchAPI( const Patch & patchFactory )
    : Patch( patchFactory )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  PatchAPI( const size_t size, const PosID * pos )
    : Patch( size, pos )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  const PatchAPI & get() const
  {
    return *this;
  }

  size_t patchSize() const
  {
    return Patch::m_patch.size();
  }

  PosID getPriorPos() const
  {
    return this -> m_patch.get()[ patchSize() - 1 ];
  }

  PosID getPosID( const size_t id ) const
  {
    return this -> m_patch.get()[id];
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
};

template< cube_size N >
bool Factory<N>::PatchAPI::movePrior( CubeID & prior, const RotID rotID ) const
{
  if ( priorMoving( prior, rotID ) )
  {
    prior = CRotations<N>::Tilt( prior, rotID );
    return true;
  }
  return false;
}

#endif  //  ! API_PATCH__H
