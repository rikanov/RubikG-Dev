#ifndef EVALUATOR_FACTORY__H
#define EVALUATOR_FACTORY__H

#include <qeueu.h>
#include <subgroup_api.h>
#include <connection_rules_api.h>
#include <root_api.h>
#include <smart_array.h>

template< cube_size N >
class FEvaluator
{
  NodeChart * m_nodeChart;

  SubgroupAPI<N>    m_subgroup;
  ConnectionsAPI<N> m_connection;
  RootAPI           m_accepted;
  Qeueu             m_qeueu;

  void build();
  void createRoot();
  void bindSolutionNodeCharts( const GroupID, const RotID );
  void buildSolutionGraph();
  void connectEqualNodeCharts();
  void finishTargets();

public:
  FEvaluator () = default;
  ~FEvaluator();

  pArray< NodeChart > create( pArray<GroupID> map, pArray<GroupID> roots, pArray<CubeID> rules );

};

template< cube_size N >
pArray< NodeChart > FEvaluator<N>::create( pArray<GroupID> map, pArray<GroupID> roots, pArray<CubeID> rules )
{
  m_subgroup.init( map );
  m_accepted.init( roots );
  m_connection.init( rules );
  m_qeueu.resize( map -> size );
  m_nodeChart = new NodeChart [ map -> size ] {};
  build();
  return std::make_shared< Array< NodeChart > > ( map -> size, m_nodeChart );
}

template< cube_size N >
void FEvaluator<N>::createRoot()
{
  m_accepted.startIteration( m_nodeChart, m_qeueu );
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
  m_connection.connectIfChild( cChart, pChart, rotID );
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
  const size_t size = m_subgroup.size() - 1;
  for ( GroupID gid = 0; gid < size; ++ gid )
  {
    // ToDo: validate before bonding
    NodeChart & node = m_nodeChart[gid];
    all_rotid( rotID, N )
    {
      const GroupID neighbor = m_subgroup.lookUp( gid, rotID );
      m_connection.connectOnSameLevel( node, m_nodeChart[ neighbor ], rotID );
    }
    node.grade[1] |= node.grade[0];
  }
}

template< cube_size N >
void FEvaluator<N>::finishTargets()
{
  for ( GroupID parentID; m_qeueu >> parentID; )
  {
    const NodeChart & parent = m_nodeChart[ parentID ];
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | parent.grade[0] | parent.grade[1] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const GroupID childID = m_subgroup.lookUp( parentID, rotID );
      NodeChart & child = m_nodeChart[ childID ];
      m_connection.connectAimsToParent( child, parent, rotID );
      m_qeueu << childID;
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
