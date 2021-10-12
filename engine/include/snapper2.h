#ifndef SNAPPER2__H
#define SNAPPER2__H

#include <gen_rotation_set.h>
#include <evaluator.h>
#include <snapshots.h>

static constexpr size_t MaximumSubgroups = Snapshots::MaximumSubgroups;

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
  
  Rubik<N>  * m_rubik;

  void setState( const size_t, const RotID, CubeID &, GroupID & );
  bool rotate( const RotID );

  void initGradients();

  void maximumTreeHeight( const DistID );
  bool progressResult();
  void initSearch( const DistID depth);
  void initRoot( const CubeID trans );
  void startIDA( const CubeID trans = 0 );
  void iterativelyDeepening();
  void setCube();
public:

  Snapper2();
  ~Snapper2();

  void toSolve( Rubik<N> * );
  void newTask( const PosID * ,const size_t , const CubeID orient = 0, AcceptFunction af = Accept<N>::Normal );
  void start();
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
void Snapper2<N>::toSolve( Rubik<N> * cube )
{
  m_rubik = cube;
}

template< cube_size N >
void Snapper2<N>::newTask( const PosID* startPos, const size_t size, const CubeID orient, AcceptFunction acceptFunction )
{
  Subgroup2<N> * nextSubgroup = m_subgroupArray + m_numberOfTasks;
  Evaluator<N> * nextEvaluate = m_evaluateArray + m_numberOfTasks;
  
  nextSubgroup -> init   ( startPos, size, orient );
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
bool Snapper2<N>::progressResult()
{
  bool result = false;
  RotID nextRot;
  while ( m_currentLevel != m_rootLevel || ! m_currentLevel -> gradient.empty() )
  {
    if ( m_currentLevel -> gradient >> nextRot )
    {
      if ( rotate( nextRot ) )
      {
        result = true;clog( "result");
        break;
      }
    }
    else
    {
      -- m_currentLevel;
    }
  }
  return result;
}

template< cube_size N >
void Snapper2<N>::setState( const size_t index, const RotID rotID, CubeID & prior, GroupID & state )
{
  Snapshots * nextLevel = m_currentLevel + 1;
  const Subgroup2<N> * subgroup = m_subgroupArray + index;
  
  prior = m_currentLevel -> prior[ index ];
  state = subgroup -> lookUp( m_currentLevel -> state[index], rotID, prior );

  if ( subgroup -> priorMoving( prior, rotID ) )
  { 
    prior =  CRotations<N>::Tilt( prior, rotID );
  }

  nextLevel -> prior[ index ] = prior;
  nextLevel -> state[ index ] = state;
}

template< cube_size N >
bool Snapper2<N>::rotate( const RotID rotID )
{
  Snapshots * nextLevel = m_currentLevel + 1;
  nextLevel -> gradient.set( m_allowedGradient[ rotID ] );
  nextLevel -> target = m_currentLevel -> target;


  const DistID depth = m_deepestLevel - nextLevel;
  CubeID  prior;
  GroupID state;
  for ( size_t index = 0; index < m_numberOfTasks; ++ index )
  {
    setState( index, rotID, prior, state );
    
    nextLevel -> gradient.restrict( m_evaluateArray [ index ].gradient( prior, state, depth ) );
    if ( nextLevel -> gradient.empty() )
      return false;
    
    nextLevel -> target &= m_evaluateArray [ index ].target( prior, state, depth );
    if ( 0 == nextLevel -> target )
      return false;
  }
  m_currentLevel -> step = rotID;
  if ( ++ m_currentLevel < m_deepestLevel || ! ( m_currentLevel -- ) -> gradient.contains( 0 ) )
  {
    return false;
  }
  ++ m_currentLevel;
  return true; // solved
}

template< cube_size N >
void Snapper2<N>::initRoot( const CubeID trans )
{
  m_currentLevel = m_rootLevel;
  for ( size_t index = 0; index < m_numberOfTasks; ++ index )
  {
    m_currentLevel -> prior[ index ] = m_subgroupArray[ index ].getPrior( *m_rubik, trans );
    m_currentLevel -> state[ index ] = m_subgroupArray[ index ].getState( *m_rubik, trans );
  }
}

template< cube_size N >
void Snapper2<N>::initSearch( const DistID depth)
{
  m_rootLevel -> gradient.set( m_allowedGradient[0] );
  m_rootLevel -> target = ( 1 << 24 ) - 1;

  for ( size_t index = 0; index < m_numberOfTasks && 0 < m_rootLevel -> target && ! m_rootLevel -> gradient.empty(); ++ index )
  {
    const CubeID  prior = m_rootLevel -> prior[ index ];
    const GroupID state = m_rootLevel -> state[ index ];
    const BitMapID grad = m_evaluateArray[ index ].gradient ( prior, state, depth );

    m_rootLevel -> gradient.restrict( grad );
    m_rootLevel -> target &= m_evaluateArray[ index ].target ( prior, state, depth );
  }
}

template< cube_size N >
void Snapper2<N>::startIDA( const CubeID trans )
{
  initRoot( trans );
  DistID depth = 0;
  do
  {
    initSearch( depth ++ );
  } while ( 0 == m_rootLevel -> target || m_rootLevel ->gradient.empty() );
  m_deepestLevel = m_rootLevel + depth - 1;
}

template< cube_size N >
void Snapper2<N>::setCube()
{
  NL();
  clog( "size:", m_currentLevel - m_rootLevel );
  for ( const Snapshots * P = m_rootLevel; P != m_currentLevel; ++ P )
  {
    clog( CRotations<N>::ToString( P -> step ) );
    m_rubik -> rotate( P -> step );
  }
  m_rubik -> print();
}

template< cube_size N >
void Snapper2<N>::iterativelyDeepening()
{
  startIDA();
  if ( m_deepestLevel == m_rootLevel )
  {
    clog( "already solved..." );
    return; // solved at start, nothing to do
  }
  while ( m_deepestLevel < m_rootLevel + MaximumIDAsteps )
  {
    initSearch( m_deepestLevel - m_rootLevel );
    if ( progressResult() )
    {
      setCube();
      break;
    }
    ++ m_deepestLevel;
  }
}

template< cube_size N > void Snapper2<N>::start()
{
  iterativelyDeepening();
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
