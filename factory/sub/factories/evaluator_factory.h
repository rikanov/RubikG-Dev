#ifndef ___EVALUATOR_FACTORY__H
#define ___EVALUATOR_FACTORY__H

#include <node_chart.h>
#include <qeueu.h>
#include <bitmap_set.h>
#include <APIs/root_set_api.h>
#include <APIs/connection_api.h>
#include <APIs/subgroup_api.h>

template< cube_size N >
class GuideFactory<N>::Evaluator: public GuideFactory<N>::RootSetAPI
                                , public GuideFactory<N>::ConnectionAPI
                                , public GuideFactory<N>::SubgroupAPI
{
  Qeueu m_qeueu;

  void init();
  void build();
  void createRoot();
  void bindSolutionNodeCharts( const GroupID, const RotID );
  void buildSolutionGraph();

protected:

  Array<NodeChart> m_nodeChart;

  Evaluator();
  Evaluator( Pattern<N> pattern, AcceptFunction af = Accept<N>::Normal );

};

template< cube_size N >
GuideFactory<N>::Evaluator::Evaluator()
  : RootSetAPI()
  , m_nodeChart( 0 )
{

}

template< cube_size N >
GuideFactory<N>::Evaluator::Evaluator( Pattern<N> pattern, AcceptFunction af )
  : RootSetAPI    ( pattern, af )
  , ConnectionAPI ( pattern )
  , SubgroupAPI   ( pattern )
  , m_nodeChart( SubgroupAPI::groupSize() )
{
  init();
}


template< cube_size N >
void GuideFactory<N>::Evaluator::init()
{
  m_qeueu.resize( GroupGeneratorAPI::groupSize() );
  build();
}

template< cube_size N >
void GuideFactory<N>::Evaluator::createRoot()
{
  // first-grade gradient = 0 --> unsolvable
  // first_grade gradient = 1 --> solved state
  // initialize root nodes as solved with zero RotID --> first-grade gradient = 1
  m_qeueu.clean();

  auto rootNodeSettings = [ this ]( const GroupID gid )
  {
    NodeChart & node = m_nodeChart[ gid ];
    node.grade = 1;
    node.aim   = this -> acceptedPriorStates();
  };

  RootSetAPI::setRoots( rootNodeSettings );
  RootSetAPI::toQeueu( m_qeueu );
}

template< cube_size N >
void GuideFactory<N>::Evaluator::bindSolutionNodeCharts( const GroupID parent, const RotID rotID )
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
void GuideFactory<N>::Evaluator::buildSolutionGraph()
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
void GuideFactory<N>::Evaluator::build()
{
  createRoot();
  buildSolutionGraph();
  createRoot();
  m_qeueu.resize( 0 ); //  de-alloc qeueu memory
}

#endif  //  ! ___EVALUATOR_FACTORY__H
