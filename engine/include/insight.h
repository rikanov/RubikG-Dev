#ifndef INSIGHT__H
#define INSIGHT__H

#include <projection.h>
#include <gen_rotation_set.h>
#include <evaluator.h>

/*
 * Insight object: { Actual state by ID } X { Subgroup map } X { Node evaluator object }
 */
template< cube_size N >
class Insight
{
  GroupID       m_stateID   = 0;
  GroupID       m_lastRadix = 0;
  Subgroup  <N> m_subgroupMap;
  Evaluator <N> m_evaluator;
  
public:
  Insight() = default;
  
  Insight( const PosID * pos, const size_t size, const CubeID orient = 0 )
  {
    init( pos, size, orient );
  }
  
  CubeID prior() const
  {
    return m_stateID / m_lastRadix;
  }
  
  GroupID state() const
  {
    return m_stateID;
  }
  
  GroupID projected() const
  {
    return Projection::LookUp( m_subgroupMap.size(), m_stateID );
  }
  
  DistID distance() const
  {
    return m_evaluator.distance( projected() );
  }
  
  BitMapID gradient() const
  {
    return GenerateRotationSet<N>::Transform( m_evaluator.gradient( projected() ), prior() );
  }
  
  BitMapID gradient( const DistID distID ) const
  {
    if ( distID < distance() )
    {
      return 0;
    }
    if ( distID == distance() )
    {
      return gradient();
    }
    return ( 1 << ( 9 * N ) ) -1;
  }
 
  void init( const PosID * pos, const size_t size, const CubeID orient = 0 )
  {
    m_subgroupMap.init( pos, size, orient );
  }
  
  void extend( const PosID pos )
  {
    m_subgroupMap.extend( pos );
    if ( 0 == m_lastRadix )
    {
      m_lastRadix = 1;
    }
    else
    {
      m_lastRadix *= 24;
    }
  }
  
  void build()
  {
    m_evaluator.map( &m_subgroupMap );
    m_evaluator.root( 0 );
    m_evaluator.build();
  }
  
  void move( const RotID rotID )
  {
    m_stateID = m_subgroupMap.lookUp( m_stateID, rotID, false );
  }
  
  BitMapID operate( const RotID rotID, const DistID distID )
  {
    m_stateID = m_subgroupMap.lookUp( m_stateID, rotID, false );
    return gradient( distID );
  }

  void print( const bool details = false, const bool projected = false ) const
  {
    m_subgroupMap.print( m_stateID, details, projected ); 
  }
};

#endif //  ! INSIGHT__H
