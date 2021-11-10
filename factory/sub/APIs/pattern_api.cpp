#ifndef API_PATCH__H
#define API_PATCH__H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <bool_array.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::PatternAPI: public Factory<N>::Pattern
{
  const BitMapID  m_priorRotIDs;

public:
  PatternAPI()
    : Pattern()
    , m_priorRotIDs( 0 )
  {
  }

  PatternAPI( const Pattern & patchFactory )
    : Pattern( patchFactory )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  PatternAPI( const size_t size, const PosID * pos )
    : Pattern( size, pos )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  size_t patchSize() const
  {
    return Pattern::m_pattern.size();
  }

  PosID getPriorPos() const
  {
    return Pattern::m_pattern[ patchSize() - 1 ];
  }

  PosID getPosID( const size_t id ) const
  {
    return Pattern::m_pattern[id];
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
bool Factory<N>::PatternAPI::movePrior( CubeID & prior, const RotID rotID ) const
{
  if ( priorMoving( prior, rotID ) )
  {
    prior = CRotations<N>::Tilt( prior, rotID );
    return true;
  }
  return false;
}

#endif  //  ! API_PATCH__H
