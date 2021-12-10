#ifndef ___PROGRESS_GUIDE__H
#define ___PROGRESS_GUIDE__H

#include <node.h>
#include <APIs/evaluator_api.h>

enum Pattern
{
  _Prior,
  _State
};

template< cube_size N >
class GuideFactory<N>::Guide: public GuideFactory<N>::EvaluatorAPI
{
  size_t  m_index;

  CubeID  m_transposition;
  CubeID  m_transpositionFactory;
  CubeID  m_transpositionProgress;

  Node    * m_node;
  CubeID  * m_prior;
  GroupID * m_state;
  DistID    m_depth;

public:

  Guide() = default;
  Guide( const size_t size, const PosID * pattern, AcceptFunction af, const size_t index, const CubeID trans = 0 );

  void setNode( Node * );

  void setAsRoot  ( const Rubik<N> * ) const;
  void setAsChild () const;

  bool restrict() const;
  void expand  () const;

  CubeID  transpose( const CubeID cubeID = 0 );
  CubeID  getTransposition( const Rubik<N> * ) const;

  bool solveNode() const;
};

template< cube_size N >
GuideFactory<N>::Guide::Guide( const size_t size, const PosID * pattern, AcceptFunction af, const size_t index, const CubeID trans )
  : EvaluatorAPI( size, pattern, af )
  , m_index( index )
  , m_transposition(0)
  , m_transpositionFactory( trans )
  , m_transpositionProgress( 0 )
{
}

template< cube_size N >
void GuideFactory<N>::Guide::setNode( Node * node )
{
  m_node = node;
  m_prior = node -> prior( m_index );
  m_state = node -> state( m_index );
  m_depth = node -> depth;
}

template< cube_size N >
void GuideFactory<N>::Guide::setAsRoot( const Rubik<N> * cube ) const
{
  *m_prior = PatternAPI::getPrior( cube, m_transposition );
  *m_state = PatternAPI::getState( cube, m_transposition );
}

template< cube_size N >
void GuideFactory<N>::Guide::setAsChild() const
{
  const Node * n = m_node - 1;
  const CubeID   prior = n -> prior[ m_index ];
  const BitMapID state = n -> state[ m_index ];
  const RotID    rotID = n -> rotate;

  *m_prior = PatternAPI::movePrior( prior, rotID );
  *m_state = SubgroupAPI::lookUp( prior, state, rotID );
}

template< cube_size N >
CubeID GuideFactory<N>::Guide::transpose( const CubeID cubeID )
{
  m_transpositionProgress = cubeID;
  m_transposition = Simplex::Composition( m_transpositionFactory, m_transpositionProgress );
  return m_transposition;
}

template< cube_size N >
CubeID GuideFactory<N>::Guide::getTransposition( const Rubik<N> * cube ) const
{
  CubeID result;
  DistID dist = EvaluatorAPI::distance( PatternAPI::getState( cube, m_transposition ) );
  all_cubeid( cid )
  {
    const GroupID nextState = PatternAPI::getState( cube, cid );
    const DistID  nextDist  = EvaluatorAPI::distance( nextState );
    if ( nextDist < dist )
    {
      dist = nextDist;
      result = cid;
    }
  }

  return result;
}

template< cube_size N >
bool GuideFactory<N>::Guide::restrict() const
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
void GuideFactory<N>::Guide::expand() const
{
  m_node -> gradient.expand( EvaluatorAPI::gradient( *m_prior, *m_state, m_depth ) );
  m_node -> target.expand( EvaluatorAPI::target( *m_prior, *m_state, m_depth ) );
}

template< cube_size N >
bool GuideFactory<N>::Guide::solveNode() const
{
  const BitMap32ID targetSet = EvaluatorAPI::target( *m_prior, *m_state, m_depth );
  return EvaluatorAPI::accepted( *m_state ) && m_node -> target.hasCommon( targetSet );
}

#endif  //  ! ___PROGRESS_GUIDE__H
