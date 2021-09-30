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
  static constexpr size_t StateStackSize = 20;

  GroupID      * m_stateStack;
  GroupID      * m_stateID ;
  Subgroup <N> * m_subgroupMap;
  Evaluator<N> * m_evaluator;
  bool          m_updated = false;

public:
  Insight()
   :  m_stateStack  ( new GroupID [ StateStackSize ] {} )
   ,  m_stateID     ( m_stateStack )
   ,  m_subgroupMap ( new Subgroup <N> () )
   ,  m_evaluator   ( new Evaluator<N> () )
  {

  }

  Insight( const PosID * pos, const size_t size, const CubeID orient = 0 )
   :  m_stateStack  ( new GroupID [ StateStackSize ] {} )
   ,  m_stateID     ( m_stateStack )
   ,  m_subgroupMap ( new Subgroup <N> () )
   ,  m_evaluator   ( new Evaluator<N> () )
  {
    init( pos, size, orient );
  }
  
  size_t size() const
  {
    return m_subgroupMap -> size();
  }
  
  CubeID prior() const
  {
    return m_subgroupMap -> prior( *m_stateID );
  }
  
  PosID priorPos() const
  {
    return m_subgroupMap -> priorPos( *m_stateID );
  }

  GroupID state() const
  {
    return *m_stateID;
  }
  
  void toSolve( const Rubik<N> & R, CubeID & trans, const bool ref )
  {
    m_stateID = m_stateStack;
    *m_stateID = m_subgroupMap -> getStateID( R, trans );

    if ( false == ref )
    {
      return;
    }

    update();

    DistID  minimum = distance( );
    GroupID chosen  = *m_stateID;

    all_cubeid ( cid )
    {
      *m_stateID = m_subgroupMap -> getStateID( R, cid );
      if ( distance() < minimum )
      {
        trans   = cid;
        minimum = distance();
        chosen  = *m_stateID;
      }
    }
    *m_stateID = chosen;
  }
  
  GroupID projected() const
  {
    return Projection::LookUp( m_subgroupMap -> size(), *m_stateID );
  }
  
  DistID distance() const
  {
    return m_evaluator -> distance( projected() );
  }
  
  BitMapID gradient( const DistID distID ) const
  {
    const DistID D = distance();
    if ( distID < D )
    {
      return 0;
    }
    if ( distID > D + 1 )
    {
      return ( 1ULL << ( 9 * N + 1 ) ) - 2;
    }
    BitMapID grad = distID == D ? m_evaluator -> grade1( projected() ) : m_evaluator -> grade2( projected() );
    GenerateRotationSet<N>::Transform( grad, prior() );
    return grad;
  }
 
  BitMap32ID aim( const DistID distID ) const
  {
    const DistID D = distance();
    if ( distID < D )
    {
      return 0;
    }
    if ( distID > D + 1 )
    {
      return ( 1 << 24 ) - 1;
    }
    BitMap32ID aim = distID == D ? m_evaluator -> aim1( projected() ) : m_evaluator -> aim2( projected() );
    return CubeSet::GetCubeSet( prior(), aim );
  }

  Insight<N> & init( const PosID * pos, const size_t size, const CubeID orient = 0, AcceptFunction af = Accept<N>::Normal )
  {
    m_updated = false;
    m_subgroupMap -> init( pos, size, orient );
    m_evaluator -> accept( af );
    return *this;
  }
  
  void extend( const PosID pos )
  {
    m_updated = false;
    m_subgroupMap -> extend( pos );
  }
  
  void update()
  {
    if ( false == m_updated )
    {
      build();
    }
  }
  
  void build()
  {
    m_evaluator -> map( m_subgroupMap );
    m_evaluator -> build();
    m_updated = true;
  }
  
  void reset()
  {
    m_stateID = m_stateStack;
  }

  void move( const RotID rotID )
  {
    *( m_stateID + 1 ) = m_subgroupMap -> lookUp( *m_stateID, rotID, false );
    ++ m_stateID;
  }

  void back()
  {
    -- m_stateID;
  }

  void print( const bool details = false, const bool projected = false ) const
  {
    clog_( "stateID:" );
    clog( *m_stateID );
    m_subgroupMap -> print( *m_stateID, details, projected );
  }
  
  ~Insight()
  {
    delete[] m_stateStack;
    delete   m_subgroupMap;
    delete   m_evaluator;
  }
};

#endif //  ! INSIGHT__H
