#ifndef FACTORY_EVALUATOR__H
#define FACTORY_EVALUATOR__H

#include <node_chart.h>
#include <qeueu.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Evaluator: public Factory<N>::RootSetAPI
{
  NodeChart * m_setNodeChart;
  Qeueu       m_qeueu;

  void init();
  void build();
  void createRoot();
  void bindSolutionNodeCharts( const GroupID, const RotID );
  void buildSolutionGraph();
  void connectEqualNodeCharts();
  void finishTargets();

protected:

  cArray<NodeChart> m_nodeChart;

  Evaluator( const size_t size, const PosID * pos  );
  Evaluator( const size_t size, const PosID * pos, AcceptFunction af = Accept<N>::Normal );

};

template< cube_size N >
Factory<N>::Evaluator::Evaluator( const size_t size, const PosID * pos, AcceptFunction af  )
  : RootSetAPI( size, pos )
{
  init();
}


template< cube_size N >
void Factory<N>::Evaluator::init()
{
  m_qeueu.resize( this -> groupSize() );
  Array<NodeChart> nodeArray = MakeArray( new NodeChart [ this -> groupSize() ]{} );
  m_setNodeChart = nodeArray.get();
  build();
  m_nodeChart = nodeArray;
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
    NodeChart & node = m_setNodeChart[ gid ];
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
  NodeChart       & cChart = m_setNodeChart[ child ];
  const NodeChart & pChart = m_setNodeChart[ parent ];
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
    NodeChart & node = m_setNodeChart[gid];
    all_rotid( rotID, N )
    {
      const GroupID neighborID = SubgroupAPI::lookUp( gid, rotID );
      NodeChart & neighbor = m_setNodeChart[neighborID];
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
    NodeChart & parent = m_setNodeChart[ next ];
    BitMap guide;
    guide.unit( CRotations<N>::AllRotIDs );
    guide.exclude( 1 | parent.grade[0] | parent.grade[1] );
    for ( RotID rotID = 0; guide >> rotID; )
    {
      const RotID   inv     = CRotations<N>::GetInvRotID( rotID );
      const GroupID childID = SubgroupAPI::lookUp( next, rotID );
      NodeChart &   child   = m_setNodeChart[ childID ];
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
