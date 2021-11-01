#ifndef EVALUATOR_FACTORY__H
#define EVALUATOR_FACTORY__H

#include <cube_set.h>
#include <qeueu.h>
#include <subgroup_api.h>
#include <bitmap_set.h>
#include <acceptance.h>
#include <subgroup_api.h>
#include <evaluator.h>
#include <smart_array.h>
#include <root_factory.h>

template< cube_size N >
class FEvaluator
{
  NodeChart * m_nodeChart;

  SubgroupAPI <N>  m_subgroup;
  pArray <GroupID> m_accepted;
  Qeueu            m_qeueu;

  void build();
  void createRoot();
  void bindSolutionNodeCharts( const GroupID, const RotID );
  void buildSolutionGraph();
  void connectEqualNodeCharts();
  void finishTargets();

  BitMap32ID mergeAim( const RotID rotID, const BitMap32ID set ) const;

public:
  FEvaluator () = default;
  ~FEvaluator();

  pArray< NodeChart > create( pArray< GroupID > map, pArray< GroupID > roots );

};

template< cube_size N >
pArray< NodeChart > FEvaluator<N>::create( pArray< GroupID > map, pArray< GroupID > roots )
{
  m_subgroup.init( map );
  m_qeueu.resize( map -> size );
  m_nodeChart = new NodeChart [ map -> size ] {};
  m_accepted = roots;
  build();
  return std::make_shared< Array< NodeChart > > ( map -> size, m_nodeChart );
}

template< cube_size N >
BitMap32ID FEvaluator<N>::mergeAim( const RotID rotID, const BitMap32ID set ) const
{
  if ( m_subgroup.priorMoving( rotID ) )
  {
    return CubeSet::GetCubeSet( CRotations<N>::GetTilt( rotID ), set );
  }
  return set;
}

template< cube_size N >
void FEvaluator<N>::createRoot()
{
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  m_qeueu.clean();
  m_accepted -> start();
  for ( GroupID gid = 0; m_accepted -> next( gid ); )
  {
    NodeChart & node = m_nodeChart[ gid ];
    node.grade[0] = 1;
    node.aim  [0] = m_accepted -> value;
    m_qeueu << gid;
  }
}

template< cube_size N >
void FEvaluator<N>::bindSolutionNodeCharts( const GroupID parent, const RotID rotID )
{
  const GroupID      child = m_subgroup.lookUp( parent, rotID );
  NodeChart       & cChart = m_nodeChart[ child ];
  const NodeChart & pChart = m_nodeChart[ parent ];
  if ( m_qeueu << child )
  {
    cChart.value = pChart.value + 1;
  }
  if ( cChart.value == pChart.value + 1 )
  {
    const RotID rotInv = CRotations<N>::GetInvRotID( rotID );
    cChart.grade[0] |= 1ULL << rotInv;
    cChart.aim[0]   |= mergeAim( rotInv, pChart.aim[0] );
  }
}

template< cube_size N >
void FEvaluator<N>::buildSolutionGraph()
{
  GroupID parent;
  while ( m_qeueu >> parent )
  {
    all_rotid ( rotID, N )
    {
      bindSolutionNodeCharts( parent, rotID );
    }
  }
}

template< cube_size N >
void FEvaluator<N>::connectEqualNodeCharts()
{
  const size_t size = pow24( m_subgroup.size() - 1 );
  for ( GroupID gid = 0; gid < size; ++ gid )
  {
    // ToDo: validate before bonding
    NodeChart & node = m_nodeChart[gid];
    all_rotid( rotID, N )
    {
      const GroupID neighbor = m_subgroup.lookUp( gid, rotID );
      if ( node.value == m_nodeChart[ neighbor ].value )
      {
        node.grade[1] |= ( 1ULL << rotID );
        node.aim  [1] |= mergeAim( rotID, node.aim[0] );
      }
    }
    node.grade[1] |= node.grade[0];
  }
}

template< cube_size N >
void FEvaluator<N>::finishTargets()
{
  for ( GroupID next; m_qeueu >> next; )
  {
    NodeChart & node = m_nodeChart[ next ];
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | node.grade[0] | node.grade[1] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const RotID   inv     = CRotations<N>::GetInvRotID( rotID );
      const GroupID former  = m_subgroup.lookUp( next, rotID );
      m_nodeChart[former].aim[0] |= mergeAim( inv, node.aim[0] );
      m_nodeChart[former].aim[1] |= mergeAim( inv, node.aim[1] );
      m_qeueu << former;
    }
  }
}

template< cube_size N >
void FEvaluator<N>::build()
{
  createRoot();
  buildSolutionGraph();
  connectEqualNodeCharts();
  createRoot();
  finishTargets();
  m_qeueu.resize( 0 ); //  de-alloc qeueu memory
}

template< cube_size N >
FEvaluator<N>::~FEvaluator()
{
}

#endif  //  ! EVALUATOR_FACTORY__H
