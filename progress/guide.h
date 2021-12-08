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
  int  m_index;

  CubeID  m_transposition;
  CubeID  m_transpositionFactory;
  CubeID  m_transpositionProgress;

public:

  Guide();
  Guide( const size_t size, const PosID * pattern, AcceptFunction af, const int index, const CubeID trans = 0 );

  bool restrict( Node * node ) const;
  bool restrictNext( Node * node, const RotID rotID ) const;

  CubeID  transpose( const CubeID cubeID = 0 );
  CubeID  getTransposition( const Rubik<N> * ) const;
  CubeID  currentPrior    ( const Rubik<N> * ) const;
  GroupID currentState    ( const Rubik<N> * ) const;


  size_t index() const
  {
    return m_index;
  }
};

template< cube_size N >
GuideFactory<N>::Guide::Guide()
  : m_index( -1 ) // dummy guide
  , m_transposition(0)
  , m_transpositionFactory( 0 )
  , m_transpositionProgress( 0 )
{
}

template< cube_size N >
GuideFactory<N>::Guide::Guide( const size_t size, const PosID * pattern, AcceptFunction af, const int index, const CubeID trans )
  : EvaluatorAPI( size, pattern, af )
  , m_index( index )
  , m_transposition(0)
  , m_transpositionFactory( trans )
  , m_transpositionProgress( 0 )
{
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
  if ( -1 == m_index )
  {
    return 0;
  }

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
bool GuideFactory<N>::Guide::restrict( Node * node ) const
{
  const CubeID  prior = node -> prior[ m_index ];
  const GroupID state = node -> state[ m_index ];

  const BitMapID gradient = EvaluatorAPI::gradient( prior, state, node -> depth );
  if ( ! node -> gradient.restrict( gradient ) )
  {
    return false;
  }

  const BitMap32ID target = EvaluatorAPI::target( prior, state, node -> depth );
  if ( ! node -> target.restrict( target ) )
  {
    return false;
  }

  return true;
}

template< cube_size N >
bool GuideFactory<N>::Guide::restrictNext( Node * node, const RotID rotID ) const
{
  Node * next = node + 1;

  const CubeID  prior = node -> prior[ m_index ];
  const GroupID state = node -> state[ m_index ];

  const CubeID  nextPrior = PatternAPI::movePrior( prior, rotID );
  const GroupID nextState = SubgroupAPI::lookUp( prior, state, rotID );

  const BitMapID gradient = EvaluatorAPI::gradient( nextPrior, nextState, next -> depth );
  if ( ! next -> gradient.restrict( gradient ) )
  {
    return false;
  }

  const BitMap32ID target = EvaluatorAPI::target( nextPrior, nextState, next -> depth );
  if ( ! next -> target.restrict( target ) )
  {
    return false;
  }

  next -> prior[ m_index ] = nextPrior;
  next -> state[ m_index ] = nextState;

  return true;
}

template< cube_size N >
CubeID GuideFactory<N>::Guide::currentPrior( const Rubik<N> * cube ) const
{
  return PatternAPI::getPrior( cube, m_transposition );
}

template< cube_size N >
GroupID GuideFactory<N>::Guide::currentState( const Rubik<N> * cube ) const
{
  return PatternAPI::getState( cube, m_transposition );
}

#endif  //  ! ___PROGRESS_GUIDE__H
