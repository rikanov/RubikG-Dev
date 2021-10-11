#ifndef INSIGHT__H
#define INSIGHT__H

#include <projection.h>
#include <gen_rotation_set.h>
#include <subgroup_cached.h>
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
  CubeID       * m_priorStack;
  CubeID       * m_prior;
  Subgroup2<N> * m_subgroupMap;
  Evaluator<N> * m_evaluator;
  bool          m_updated = false;

public:
  Insight()
   :  m_stateStack  ( new GroupID [ StateStackSize ] {} )
   ,  m_stateID     ( m_stateStack )
   ,  m_priorStack  ( new CubeID  [ StateStackSize ] {} )
   ,  m_prior       ( m_priorStack )
   ,  m_subgroupMap ( new Subgroup2<N> () )
   ,  m_evaluator   ( new Evaluator<N> () )
  {

  }

  Insight( const PosID * pos, const size_t size, const CubeID orient = 0 )
   :  m_stateStack  ( new GroupID [ StateStackSize ] {} )
   ,  m_stateID     ( m_stateStack )
   ,  m_priorStack  ( new CubeID  [ StateStackSize ] {} )
   ,  m_prior       ( m_priorStack )
   ,  m_subgroupMap ( new Subgroup2<N> () )
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
    return *m_prior;
  }

  GroupID state( const bool projected = false ) const
  {
    return projected ? m_subgroupMap -> usePrior( prior(), *m_stateID ) : *m_stateID;
  }
  
  void toSolve( const Rubik<N> & R, CubeID & trans, const bool ref )
  {
    m_stateID = m_stateStack;
    m_prior   = m_priorStack;
    *m_stateID = m_subgroupMap -> getState( R, trans );
    *m_prior   = m_subgroupMap -> getPrior( R, trans );

    if ( false == ref )
    {
      return;
    }

    update();

    DistID  minimum = distance( );
    GroupID chState  = *m_stateID;
    CubeID  chPrior  = *m_prior;

    all_cubeid ( cid )
    {
      *m_stateID = m_subgroupMap -> getState( R, cid );
      *m_prior   = m_subgroupMap -> getPrior( R, cid );
      if ( distance() < minimum )
      {
        trans   = cid;
        minimum = distance();
        chState = *m_stateID;
        chPrior = *m_prior;
      }
    }
    *m_stateID = chState;
    *m_prior   = chPrior;
  }
  
  DistID distance() const
  {
    return m_evaluator -> distance( *m_stateID );
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
    BitMapID grad = distID == D ? m_evaluator -> grade1( *m_stateID ) : m_evaluator -> grade2( *m_stateID );
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
    BitMap32ID aim = distID == D ? m_evaluator -> aim1( *m_stateID ) : m_evaluator -> aim2( *m_stateID );
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
    m_evaluator -> root( 0 );
    m_evaluator -> build();
    m_updated = true;
  }
  
  void reset()
  {
    m_prior   = m_priorStack;
    m_stateID = m_stateStack;
  }

  void move( const RotID rotID )
  {
    *( m_prior   + 1 ) = m_subgroupMap -> priorMoving( prior(), rotID ) ?  CRotations<N>::Tilt( prior(), rotID ) : prior();
    ++ m_prior;
    *( m_stateID + 1 ) = m_subgroupMap -> lookUp( *m_stateID, rotID, prior() );
    ++ m_stateID;
  }

  void back()
  {
    -- m_stateID;
    -- m_prior;
  }

  void print( const bool details = false, const bool projected = false ) const
  {
    clog_( "stateID:" );
    clog( *m_stateID );
    m_subgroupMap -> print( *m_stateID, projected ? 0 : prior(), details );
  }
  
  ~Insight()
  {
    delete[] m_stateStack;
    delete[] m_priorStack;
    delete   m_subgroupMap;
    delete   m_evaluator;
  }
};

#endif //  ! INSIGHT__H
