#ifndef FACTORY_EVALUATOR__H
#define FACTORY_EVALUATOR__H

#include <node_chart.h>
#include <qeueu.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Evaluator: public Factory<N>::RootSetAPI
{
  Qeueu       m_qeueu;

  void init();
  void build();
  void createRoot();
  void bindSolutionNodeCharts( const GroupID, const RotID );
  void buildSolutionGraph();
  void connectEqualNodeCharts();
  void finishTargets();

protected:

  Array<NodeChart> m_nodeChart;

  Evaluator();
  Evaluator( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );

};

template< cube_size N >
Factory<N>::Evaluator::Evaluator()
  : RootSetAPI()
  , m_nodeChart( 0 )
{

}

template< cube_size N >
Factory<N>::Evaluator::Evaluator( const size_t size, const PosID * pos, AcceptFunction af  )
  : RootSetAPI( size, pos )
  , m_nodeChart( SubgroupAPI::groupSize() )
{
  init();
}


template< cube_size N >
void Factory<N>::Evaluator::init()
{
  m_qeueu.resize( GroupGeneratorAPI::groupSize() );
  build();
}

template< cube_size N >
void Factory<N>::Evaluator::createRoot()
{
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  m_qeueu.clean();

  auto rootNodeSettings = [ this ]( const GroupID gid )
  {
    NodeChart & node = m_nodeChart[ gid ];
    node.grade[0] = 1;
    node.aim  [0] = this -> acceptedPriorStates();
  };

  RootSetAPI::toQeueu( m_qeueu );
  RootSetAPI::setRoots( rootNodeSettings );
}

template< cube_size N >
void Factory<N>::Evaluator::bindSolutionNodeCharts( const GroupID parent, const RotID rotID )
{
  const GroupID      child = SubgroupAPI::lookUp( parent, rotID );
  NodeChart       & cChart = m_nodeChart[ child ];
  const NodeChart & pChart = m_nodeChart[ parent ];
  if ( m_qeueu << child )
  {
    cChart.level = pChart.level + 1;
  }
  ConnectionAPI::connectIfChild( cChart, pChart, rotID );
}

template< cube_size N >
void Factory<N>::Evaluator::buildSolutionGraph()
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
void Factory<N>::Evaluator::connectEqualNodeCharts()
{
  const size_t size = SubgroupAPI::groupSize() - 1;
  for ( GroupID gid = 0; gid < size; ++ gid )
  {
    if ( ! SubgroupAPI::valid( gid ) )
      continue;
    NodeChart & node = m_nodeChart[gid];
    all_rotid( rotID, N )
    {
      const GroupID neighborID = SubgroupAPI::lookUp( gid, rotID );
      NodeChart & neighbor = m_nodeChart[neighborID];
      ConnectionAPI::connectOnSameLevel( node, neighbor, rotID );
    }
    node.grade[1] |= node.grade[0];
  }
}

template< cube_size N >
void Factory<N>::Evaluator::finishTargets()
{
  for ( GroupID next; m_qeueu >> next; )
  {
    NodeChart & parent = m_nodeChart[ next ];
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | parent.grade[0] | parent.grade[1] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const RotID   inv     = CRotations<N>::GetInvRotID( rotID );
      const GroupID childID = SubgroupAPI::lookUp( next, rotID );
      NodeChart &   child   = m_nodeChart[ childID ];
      ConnectionAPI::connectAimsToParent( child, parent, rotID );
      m_qeueu << childID;
    }
  }
}

template< cube_size N >
void Factory<N>::Evaluator::build()
{
  createRoot();
  buildSolutionGraph();
  connectEqualNodeCharts();
  createRoot();
  finishTargets();
  m_qeueu.resize( 0 ); //  de-alloc qeueu memory
}

#endif  //  ! FACTORY_EVALUATOR__H
