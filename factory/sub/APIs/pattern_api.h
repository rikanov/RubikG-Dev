#ifndef ___PATTERN_API_H
#define ___PATTERN_API_H

#include <cube_positions.h>
#include <cube_rotations.h>
#include <bool_array.h>
#include <factories/pattern_factory.h>
#include <rubik.h>

template< cube_size N >
class GuideFactory<N>::PatternAPI: public GuideFactory<N>::Pattern
{
  BitMapID  m_priorRotIDs;

public:
  PatternAPI()
    : Pattern()
    , m_priorRotIDs( 0 )
  {
  }

  PatternAPI( const Pattern & patchGuideFactory )
    : Pattern( patchGuideFactory )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  PatternAPI( const size_t size, const PosID * pos )
    : Pattern( size, pos )
    , m_priorRotIDs( CPositions<N>::ActOn( getPriorPos() ) )
  {

  }

  size_t patternSize() const
  {
    return Pattern::m_pattern.size();
  }

  PosID getPriorPos() const
  {
    return Pattern::m_pattern[ patternSize() - 1 ];
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

  CubeID movePrior( const CubeID prior, const RotID rotID ) const;
  bool valid( GroupID ) const;

  CubeID getPrior ( const Rubik<N> * Cube, const CubeID trans = 0 ) const;
  GroupID getState( const Rubik<N> * Cube, const CubeID trans = 0 ) const;
};

template< cube_size N >
CubeID GuideFactory<N>::PatternAPI::movePrior( const CubeID prior, const RotID rotID ) const
{
  return priorMoving( prior, rotID ) ? CRotations<N>::Tilt( prior, rotID ) : prior;
}

template< cube_size N >
bool GuideFactory<N>::PatternAPI::valid( GroupID gid ) const
{
  BoolArray positions( CPositions<N>::GetSize() );

  for ( size_t index = 0; index < this -> patternSize(); ++ index, gid /= 24 )
  {
    const PosID next = CPositions<N>::GetPosID( getPosID( index ), gid % 24 );
    if ( positions( next ) )
      return false;
    positions.set( next, true );
  }
  return true;
}

template< cube_size N >
CubeID GuideFactory<N>::PatternAPI::getPrior(const Rubik<N>* Cube, const CubeID trans) const
{
  return Cube -> transpose( getPriorPos(), trans );
}



template< cube_size N >
GroupID GuideFactory<N>::PatternAPI::getState( const Rubik<N> * Cube, const CubeID trans ) const
{
  BitMapID result = 0;
  const CubeID invPrior = Simplex::Inverse( getPrior( Cube, trans ) );
  for ( int id = 0; id < patternSize() - 1; ++ id )
  {
    result += Simplex::Composition( Cube -> transpose( getPosID( id ), trans ), invPrior ) * pow24( id );
  }
  return result;
}
#endif  //  ! ___PATTERN_API_H
