#ifndef ___GUIDE_BASE__H
#define ___GUIDE_BASE__H


#include <node.h>
#include <APIs/evaluator_api.h>

template< cube_size N >
class GuideFactory<N>::GuideBase: public GuideFactory<N>::EvaluatorAPI
{
  size_t  m_index;

  CubeID  m_transposition;
  CubeID  m_transpositionFactory;
  CubeID  m_transpositionProgress;

  Node    * m_node;

  CubeID  * m_prior;
  GroupID * m_state;
  DistID    m_depth;

protected:

  GuideBase() = default;
  GuideBase(Pattern<N> pattern, AcceptFunction af, const size_t index, const CubeID trans );

  size_t index() const;

  void setNode( Node * );

  void setAsRoot  ( const Rubik<N> * ) const;
  void setAsChild () const;

  bool restrict() const;
  void expand  () const;

  CubeID  transpose( const CubeID cubeID = 0 );
  CubeID  getTransposition( const Rubik<N> * ) const;

  void setOptionalRoot ( const Rubik<N> * cube, Node * node, const CubeID trans );
  bool setScheduledRoot( const Rubik<N> * cube, Node * node, const CubeID trans );
  DistID distanceOf( const Rubik<N> * cube ) const;

  void setOptionalNode ( Node * next );
  bool setScheduledNode( Node * next );
};

template< cube_size N >
GuideFactory<N>::GuideBase::GuideBase( Pattern<N> pattern, AcceptFunction af, const size_t index, const CubeID trans )
  : EvaluatorAPI( pattern, af )
  , m_index( index )
  , m_transposition(0)
  , m_transpositionFactory( trans )
  , m_transpositionProgress( 0 )
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
  m_node = node;
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
CubeID GuideFactory<N>::GuideBase::transpose( const CubeID cubeID )
{
  m_transpositionProgress = cubeID;
  m_transposition = Simplex::Composition( m_transpositionFactory, m_transpositionProgress );
  return m_transposition;
}

template< cube_size N >
bool GuideFactory<N>::GuideBase::restrict() const
{
  const BitMapID gradient = EvaluatorAPI::gradient( *m_prior, *m_state, m_depth );
  if ( ! m_node -> gradient.restrict( gradient ) )
  {
    return false;
  }

  const BitMap32ID target = EvaluatorAPI::target( *m_prior, *m_state, m_depth );
  if ( ! m_node -> target.restrict( target ) )
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

#endif  //  ! ___GUIDE_BASE__H