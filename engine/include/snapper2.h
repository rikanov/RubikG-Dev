#ifndef SNAPPER2__H
#define SNAPPER2__H

#include <gen_rotation_set.h>
#include <evaluator_api.h>
#include <snapshots.h>
#include <factories.h>
#include <scan.h>
#include <interface.h>

static constexpr size_t MaximumSubgroups = Snapshots::MaximumSubgroups;

template< cube_size N >
class Snapper2
{
  static constexpr size_t MaximumIDAsteps = 12;

  BitMapID * m_allowedGradient;

  size_t             m_numberOfTasks;
  size_t             m_activateTasks;
  Snapshots        * m_snapshotArray;
  SubgroupAPI<N>   * m_subgroupArray;
  EvaluatorAPI<N>  * m_evaluateArray;
  PatchAPI<N>      * m_patchArray;

  Snapshots * m_deepestLevel;
  Snapshots * m_currentLevel;
  Snapshots * m_rootLevel; // allias only

  // Factories
  FPatch          m_patches;
  FRoot<N>        m_rootFactory;
  FSubgroup<N>    m_subgroups;
  FConnections<N> m_connection;
  FEvaluator<N>   m_evaluator;

  Rubik<N>  * m_rubik;

  bool setNextState( const RotID );
  bool rotate( const RotID );

  void initGradients();

  void maximumTreeHeight( const DistID );
  bool progressResult();
  bool initSearch( const DistID depth);
  void initRoot( const CubeID trans );
  void startIDA( const CubeID trans = 0 );
  void iterativelyDeepening();
  void setCube();
public:

  bool logs = false;
  Snapper2();
  ~Snapper2();

  void toSolve( Rubik<N> * );
  void newTask( const PosID * ,const size_t , const CubeID orient = 0, AcceptFunction af = Accept<N>::Normal );
  void start  ( const size_t tasks = 0 );

  void printState() const;
};

template< cube_size N >
Snapper2<N>::Snapper2()
 :  m_allowedGradient ( new BitMapID [ CRotations<N>::AllRotIDs ] )
 ,  m_numberOfTasks   ( 0 )
 ,  m_snapshotArray   ( new Snapshots       [ MaximumIDAsteps  ] )
 ,  m_subgroupArray   ( new SubgroupAPI<N>  [ MaximumSubgroups ] )
 ,  m_evaluateArray   ( new EvaluatorAPI<N> [ MaximumSubgroups ] )
 ,  m_patchArray      ( new PatchAPI<N>     [ MaximumSubgroups ] )
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
  SubgroupAPI<N>  * nextSubgroup = m_subgroupArray + m_numberOfTasks;
  EvaluatorAPI<N> * nextEvaluate = m_evaluateArray + m_numberOfTasks;
  PatchAPI<N>     * nextPatch    = m_patchArray    + m_numberOfTasks;
  
  auto patch = m_patches.create( startPos, size );
  nextPatch -> init( patch );

  auto group = m_subgroups.create( patch );
  nextSubgroup -> init( group );

  auto roots   = m_rootFactory.create( patch, acceptFunction );
  auto connect = m_connection.create( patch );
  auto chart   = m_evaluator.create( group, roots, connect );
  nextEvaluate -> init( chart );
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
  if ( m_currentLevel -> gradient.contains( 0 ) )
  {
    return true;
  }
  auto emptyTree = [&]()
    {
      return  m_currentLevel == m_rootLevel && m_currentLevel -> gradient.empty();
    };
  bool result = false; 
  for ( RotID nextRot = 0; ! result && ! emptyTree(); )
  {
    if ( m_currentLevel -> gradient >> nextRot )
    {
      result = rotate( nextRot );
    }
    else
    {
      -- m_currentLevel;
    }
  }
  return result;
}

template< cube_size N >
bool Snapper2<N>::setNextState( const RotID rotID )
{
  Snapshots * nextLevel = m_currentLevel + 1;
  auto & gradient = nextLevel -> gradient;
  auto & target   = nextLevel -> target;
  gradient.set( m_allowedGradient[ rotID ] );
  target = m_currentLevel -> target;
  const DistID depth = m_deepestLevel - nextLevel;

  for ( size_t index = 0; index < m_activateTasks; ++ index )
  {
    const SubgroupAPI<N> & subgroup = m_subgroupArray[ index ];
    const PatchAPI<N>    & patch    = m_patchArray   [ index ];

    auto & prior = nextLevel -> prior[ index ];
    auto & state = nextLevel -> state[ index ];

    prior = m_currentLevel -> prior[ index ];
    state = subgroup.lookUp( m_currentLevel -> state[index], rotID, prior );

    if ( patch.priorMoving( prior, rotID ) )
    {
      prior = CRotations<N>::Tilt( prior, rotID );
    }

    gradient.restrict( m_evaluateArray [ index ].gradient( prior, state, depth ) );
    if ( gradient.empty() )
      return false;

    target &= m_evaluateArray [ index ].target( prior, state, depth );
    if ( 0 == target )
      return false;
  }
  return true;
}

template< cube_size N >
bool Snapper2<N>::rotate( const RotID rotID )
{
  if ( ! setNextState( rotID ) )
  {
    return false;
  }
  m_currentLevel -> step = rotID;
  if ( ++ m_currentLevel < m_deepestLevel )
  {
    return false;
  }
  if ( m_currentLevel -> gradient.contains( 0 ) ) // solved
  {
    return true;
  }
  -- m_currentLevel;
  return false;
}

template< cube_size N >
void Snapper2<N>::initRoot( const CubeID trans )
{
  m_currentLevel = m_rootLevel;
  for ( size_t index = 0; index < m_activateTasks; ++ index )
  {
    m_currentLevel -> prior[ index ] = Scan::GetPrior( *m_rubik, m_patchArray[index], trans );
    m_currentLevel -> state[ index ] = Scan::GetState( *m_rubik, m_patchArray[index], trans );
  }
}

template< cube_size N >
bool Snapper2<N>::initSearch( const DistID depth)
{
  BitMap & gradient = m_rootLevel -> gradient;
  BitMap32ID & target = m_rootLevel -> target;
  gradient.set( m_allowedGradient[0] );
  target = ( 1 << 24 ) - 1;

  for ( size_t index = 0; index < m_activateTasks && 0 < target && ! gradient.empty(); ++ index )
  {
    const CubeID  prior = m_rootLevel -> prior[ index ];
    const GroupID state = m_rootLevel -> state[ index ];
    const BitMapID grad = m_evaluateArray[ index ].gradient ( prior, state, depth );

    gradient.restrict( grad );
    target &= m_evaluateArray[ index ].target ( prior, state, depth );
  }
  return m_rootLevel -> gradient.empty() || 0 == m_rootLevel -> target;
}

template< cube_size N >
void Snapper2<N>::startIDA( const CubeID trans )
{
  initRoot( trans );
  DistID height = 0;
  while ( initSearch( height ) )
  {
    ++ height;
  }
  maximumTreeHeight( height );
}

template< cube_size N >
void Snapper2<N>::setCube()
{
  NL();
  clog( "Global steps:", m_rubik -> steps() );
  const size_t steps =  m_currentLevel - m_rootLevel;
  Sequence seq;
  for ( const Snapshots * P = m_rootLevel; P != m_currentLevel; ++ P )
  {
    seq << P -> step;
    m_rubik -> rotate( P -> step );
    if ( logs )
    {
      clog_( "depth:", ( m_deepestLevel - P ), CRotations<N>::ToString( P -> step ) );
      P -> print( N );
      m_rubik -> print();
    }
  }
  CRotations<N>::Print( seq );
  m_rubik -> print();
  clog( "Current global steps:", m_rubik -> steps() );
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
    if ( progressResult() )
    {
      setCube();
      break;
    }
    initSearch( ++ m_deepestLevel - m_rootLevel );
  }
}

template< cube_size N >
void Snapper2<N>::start( const size_t tasks )
{
  m_activateTasks = ( 0 == tasks ) ? m_numberOfTasks : tasks;
  iterativelyDeepening();
}

template< cube_size N >
void Snapper2<N>::printState() const
{
  m_currentLevel -> print( N );
  for ( size_t index = 0; index < m_activateTasks; ++ index )
  {
    BitMap::Print_( m_evaluateArray[index].target( 0, m_currentLevel -> state[ index ], 0 ), 24, 4 );
    clog_( Simplex::GetCube( m_currentLevel -> prior[ index ] ).toString(), "-->" );
    CubeSet::Print( m_evaluateArray[index].target( m_currentLevel -> prior[ index ], m_currentLevel -> state[ index ], 0 ), 0 == index );
  }
}


template< cube_size N >
Snapper2<N>::~Snapper2()
{
  delete[] m_allowedGradient;
  delete[] m_snapshotArray;
  delete[] m_subgroupArray;
  delete[] m_evaluateArray;
  delete[] m_patchArray;
}

#endif  //  ! SNAPPER2__H
