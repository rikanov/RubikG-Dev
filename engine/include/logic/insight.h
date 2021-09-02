#ifndef INSIGHT__H
#define INSIGHT__H

#include <projection.h>
#include <seeker.h>

/*
 * Insight object: { Actual state by ID } X { Subgroup map } X { Node evaluator a.ka Seeker }
 */
template< cube_size N >
class Insight
{
  GroupID      m_stateID = 0;
  Subgroup <N> m_subgroupMap;
  Seeker   <N> m_evaluator;
  
public:
  Insight() = default;
  
  Insight( const PosID * pos, const size_t size, const CubeID orient = 0 )
  {
    init( pos, size, orient );
  }
  
  CubeID prior() const
  {
    return m_stateID / pow24( m_subgroupMap.size() - 1 );
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
  
  void init( const PosID * pos, const size_t size, const CubeID orient = 0 )
  {
    m_subgroupMap.init( pos, size, orient );clog( "Init OK" );
  }
  
  void extend( const PosID pos )
  {
    m_subgroupMap.extend( pos );
  }
  
  void build()
  {
    m_evaluator.map( &m_subgroupMap ); clog( "mapping OK" );
    m_evaluator.root( 0 );
    m_evaluator.build(); clog( "Build OK" );
  }
  
  void move( const RotID rotID )
  {
    m_stateID = m_subgroupMap.lookUp( m_stateID, rotID, false );
  }
  
  void print( const bool details = false, const bool projected = false ) const
  {
    m_subgroupMap.print( m_stateID, details, projected ); 
  }
};

#endif //  ! INSIGHT__H
