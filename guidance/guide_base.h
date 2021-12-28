#ifndef ___GUIDE_BASE__H
#define ___GUIDE_BASE__H


#include <node.h>
#include <APIs/evaluator_api.h>

template< cube_size N >
class GuideFactory<N>::GuideBase: public GuideFactory<N>::EvaluatorAPI
{
  size_t  m_index;

  CubeID  m_transposition;
  Node  * m_node;

  CubeID  * m_prior;
  GroupID * m_state;
  DistID    m_depth;

protected:

  GuideBase() = default;
  GuideBase( const Subgroup & subgroup, AcceptFunction af, const size_t index );

  size_t index() const;

  void setNode( Node * );

  void setAsRoot  ( const Rubik<N> * ) const;
  void setAsChild () const;

  bool restrict() const;
  void expand  () const;

  DistID distanceOf( const Rubik<N> * cube ) const;

public:
  void transpose( const CubeID cubeID = 0 );
  CubeID  getTransposition( const Rubik<N> * ) const;

  bool solveNode( const Node * node ) const;
};

template< cube_size N >
GuideFactory<N>::GuideBase::GuideBase( const Subgroup & subgroup, AcceptFunction af, const size_t index )
  : EvaluatorAPI( subgroup, af )
  , m_index( index )
  , m_transposition( 0 )
{

}

template< cube_size N >
size_t GuideFactory<N>::GuideBase::index() const
{
  return m_index;
}

template< cube_size N >
void GuideFactory<N>::GuideBase::setNode( Node * node )
{
  m_node  = node;
  m_prior = node -> prior( m_index );
  m_state = node -> state( m_index );
  m_depth = node -> depth;
}

template< cube_size N >
void GuideFactory<N>::GuideBase::setAsRoot( const Rubik<N> * cube ) const
{
  *m_prior = Pattern<N>::getPrior( cube, m_transposition );
  *m_state = Pattern<N>::getState( cube, m_transposition );
}

template< cube_size N >
void GuideFactory<N>::GuideBase::setAsChild() const
{
  const Node * n = m_node - 1;
  const CubeID   prior = n -> prior[ m_index ];
  const BitMapID state = n -> state[ m_index ];
  const RotID    rotID = n -> rotate;

  *m_prior = Pattern<N>::movePrior( prior, rotID );
  *m_state = SubgroupAPI::lookUp( prior, state, rotID );
}

template< cube_size N >
void GuideFactory<N>::GuideBase::transpose( const CubeID cubeID )
{
  m_transposition = Simplex::Composition( EvaluatorAPI::base(), cubeID );
}

template< cube_size N >
CubeID GuideFactory<N>::GuideBase::getTransposition( const Rubik<N> * cube ) const
{
  CubeID result = m_transposition;
  DistID dist = distanceOf( cube );
  all_cubeid( cid )
  {
    const GroupID nextState = Pattern<N>::getState( cube, cid );
    const DistID  nextDist  = EvaluatorAPI::distance( nextState );
    if ( nextDist < dist )
    {
      dist = nextDist;
      result = cid;
    }
  }

  return Simplex::Composition( result, m_transposition );
}

template< cube_size N >
bool GuideFactory<N>::GuideBase::restrict() const
{
  const BitMapID gradient = EvaluatorAPI::gradient( *m_prior, *m_state, m_depth );
  if ( ! m_node -> gradient.restrict( gradient ) )
  {
    return false;
  }

  if ( ! m_node -> target.restrict( EvaluatorAPI::target( *m_prior, *m_state, m_depth ) ) )
  {
    return false;
  }

  return true;
}

template< cube_size N >
void GuideFactory<N>::GuideBase::expand() const
{
  m_node -> gradient.expand( EvaluatorAPI::gradient( *m_prior, *m_state, m_depth ) );
  m_node -> target.expand( EvaluatorAPI::target( *m_prior, *m_state, m_depth ) );
}

template< cube_size N >
DistID GuideFactory<N>::GuideBase::distanceOf( const Rubik<N> * cube ) const
{
  return EvaluatorAPI::distance( Pattern<N>::getState( cube, m_transposition ) );
}

template< cube_size N >
bool GuideFactory<N>::GuideBase::solveNode( const Node * node ) const
{
  const size_t index = GuideBase::index();
  const CubeID prior = node -> prior[ index ];
  const GroupID state = node -> state[ index ];
  const BitMap32ID target = EvaluatorAPI::target( prior, state, node -> depth );
  return EvaluatorAPI::accepted( state ) && node -> target.hasCommon( target );
}

#endif  //  ! ___GUIDE_BASE__H
