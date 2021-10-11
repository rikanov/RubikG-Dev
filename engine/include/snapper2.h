#ifndef SNAPPER2__H
#define SNAPPER2__H

#include <base_types.h>
#include <gen_rotation_set.h>
#include <evaluator.h>

static constexpr size_t MaximumSubgroups = 40;


struct Snapshots
{
  BitMap       gradient;
  BitMap32ID   target;
  
  GroupID    * state;
  CubeID     * prior;

public:

  Snapshots()
   :  target ( ( 1 << 24 ) - 1 )
   ,  state  ( new GroupID [ MaximumSubgroups ] )
   ,  prior  ( new CubeID  [ MaximumSubgroups ] )
  {
  }

  ~Snapshots()
  {
    delete[] state;
    delete[] prior;
  }

};

template< cube_size N >
class Snapper2
{
  static constexpr size_t MaximumIDAsteps = 12;

  BitMapID * m_allowedGradient;

  size_t         m_numberOfTasks;
  Snapshots    * m_snapshotArray;
  Subgroup2<N> * m_subgroupArray;
  Evaluator<N> * m_evaluateArray;

  Snapshots * m_deepestLevel;
  Snapshots * m_currentLevel;
  Snapshots * m_rootLevel; // allias only
  
public:

  Snapper2();
  ~Snapper2();

  void initGradients();

  void newTask( const PosID * ,const size_t , const CubeID orient = 0, AcceptFunction af = Accept<N>::Normal );
  void maximumTreeHeight( const DistID );
  bool progress();
  
  void setState( const size_t, const RotID, CubeID &, GroupID & );
  bool rotate( const RotID );

  BitMapID gradient( const CubeID, const GroupID, const Evaluator<N> * ) const;
  BitMap32ID target( const CubeID, const GroupID, const Evaluator<N> * ) const;
  
  
};

template< cube_size N >
Snapper2<N>::Snapper2()
 :  m_allowedGradient ( new BitMapID [ CRotations<N>::AllRotIDs ] )
 ,  m_numberOfTasks   ( 0 )
 ,  m_snapshotArray   ( new Snapshots    [ MaximumIDAsteps  ] )
 ,  m_subgroupArray   ( new Subgroup2<N> [ MaximumSubgroups ] )
 ,  m_evaluateArray   ( new Evaluator<N> [ MaximumSubgroups ] )
{
  m_deepestLevel   = m_snapshotArray;
  m_currentLevel   = m_snapshotArray;
  m_rootLevel      = m_snapshotArray;
  initGradients();
}

template< cube_size N >
void Snapper2<N>::initGradients()
{
  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 1; // include the solved bit

  RotID rotID    = 0;
  BitMapID allow = allRotations;

  m_allowedGradient[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    m_allowedGradient[ rotID ++ ] = allow;
  }
}

template< cube_size N >
void Snapper2<N>::newTask( const PosID* startPos, const size_t size, const CubeID orient, AcceptFunction acceptFunction )
{
  Subgroup2<N> * nextSubgroup = m_subgroupArray + m_numberOfTasks;
  Evaluator<N> * nextEvaluate = m_evaluateArray + m_numberOfTasks;
  
  nextSubgroup -> init   ( size, startPos, orient );
  nextEvaluate -> map    ( nextSubgroup   );
  nextEvaluate -> accept ( acceptFunction );
  nextEvaluate -> build  ();

  ++ m_numberOfTasks;
}

template< cube_size N >
void Snapper2<N>::maximumTreeHeight( const DistID depth )
{
  m_deepestLevel = m_snapshotArray + depth;
}

template< cube_size N >
bool Snapper2<N>::progress()
{
  while ( ! m_currentLevel -> gradient.contains( 1 ) && ( m_currentLevel != m_rootLevel || ! m_currentLevel -> gradient.empty() ) )
  {
    RotID nextRot;
    if ( ! ( m_currentLevel -> gradient >> nextRot && rotate( nextRot ) ) )
    {
      -- m_currentLevel;
    }
  }
  return m_currentLevel -> gradient.contains( 1 );
}

template< cube_size N >
void Snapper2<N>::setState( const size_t index, const RotID rotID, CubeID & prior, GroupID & state )
{
  Snapshots * nextLevel = m_currentLevel + 1;
  const Subgroup2<N> * subgroup = m_subgroupArray + index;
  
  prior = m_currentLevel -> prior[ index ];
  if ( subgroup -> priorMoving( prior, rotID ) )
  { 
    prior =  CRotations<N>::Tilt( prior, rotID );
  }
  state = subgroup -> lookUp( m_currentLevel -> state[index], rotID, prior );
  
  nextLevel -> prior[ index ] = prior;
  nextLevel -> state[ index ] = state;
}

template< cube_size N >
bool Snapper2<N>::rotate( const RotID rotID )
{
  if ( m_currentLevel == m_deepestLevel )
  {
    return false;
  }
  Snapshots * nextLevel = m_currentLevel + 1;
  nextLevel -> gradient.set( m_allowedGradient[ rotID ] );
  nextLevel -> target   = m_currentLevel -> target;
  CubeID  prior;
  GroupID state;
  for ( size_t index = 0; index < m_numberOfTasks; ++ index )
  {
    setState( index, rotID, prior, state );
    
    nextLevel -> gradient.restrict( gradient( prior, state, m_evaluateArray + index ) );
    if ( nextLevel -> gradient.empty() )
      return true;
    
    nextLevel -> target &= target( prior, state, m_evaluateArray + index );
    if ( 0 == nextLevel -> target )
      return true;
  }
  ++ m_currentLevel;
  return true;
}

template< cube_size N >
BitMapID Snapper2<N>::gradient( const CubeID prior, const GroupID state, const Evaluator<N> * evaluator ) const
{
  const DistID dist = evaluator -> distance( state );
  const DistID D = m_deepestLevel - m_currentLevel;
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1ULL << ( 9 * N + 1 ) ) - 2;
  }
  BitMapID grad = dist == D ? evaluator -> grade1( state ) : evaluator -> grade2( state );
  GenerateRotationSet<N>::Transform( grad, prior );
  return grad;
}

template< cube_size N >
BitMap32ID Snapper2<N>::target( const CubeID prior, const GroupID state, const Evaluator<N> * evaluator ) const
{
  const DistID dist = evaluator -> distance( state );
  const DistID D = m_deepestLevel - m_currentLevel;
  if ( dist < D )
  {
    return 0;
  }
  if ( dist > D + 1 )
  {
    return ( 1 << 24 ) - 1;
  }
  BitMap32ID aim = dist == D ? evaluator -> aim1( state ) : evaluator -> aim2( state );
  return CubeSet::GetCubeSet( prior, aim );
}

template< cube_size N >
Snapper2<N>::~Snapper2()
{
  delete[] m_allowedGradient;
  delete[] m_snapshotArray;
  delete[] m_subgroupArray;
  delete[] m_evaluateArray;
}


#endif  //  ! SNAPPER2__H
