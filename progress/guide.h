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

public:

  Guide() = default;
  Guide( const size_t size, const PosID * pattern, AcceptFunction af, const size_t index );

  bool restrict( Node * node ) const;
  void expand  ( Node * node ) const;

  bool restrictNext( Node * node, const RotID rotID ) const;
  void expandNext  ( Node * node, const RotID rotID ) const;

  CubeID  currentPrior( const Rubik<N> * ) const;
  GroupID currentState( const Rubik<N> * ) const;

  size_t index() const
  {
    return m_index;
  }
};

template< cube_size N >
GuideFactory<N>::Guide::Guide( const size_t size, const PosID * pattern, AcceptFunction af, const size_t index )
  : EvaluatorAPI( size, pattern, af )
  , m_index( index )
  , m_transposition(0)
{
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
void GuideFactory<N>::Guide::expand( Node* node ) const
{
  const CubeID  prior = node -> prior[ m_index ];
  const GroupID state = node -> state[ m_index ];

  const BitMapID gradient = EvaluatorAPI::gradient( prior, state, node -> depth );
  const BitMap32ID target = EvaluatorAPI::target  ( prior, state, node -> depth );

  node -> gradient.expand( gradient );
  node -> target.expand( target );
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

template< cube_size N >
void GuideFactory<N>::Guide::expandNext( Node * node, const RotID rotID ) const
{
  Node * next = node + 1;

  const CubeID  prior = node -> prior[ m_index ];
  const GroupID state = node -> state[ m_index ];

  const CubeID  nextPrior = PatternAPI::movePrior( prior, rotID );
  const GroupID nextState = SubgroupAPI::lookUp( state, rotID );

  const BitMapID gradient = EvaluatorAPI::gradient( nextPrior, nextState, next -> depth );
  const BitMap32ID target = EvaluatorAPI::target  ( nextPrior, nextState, next -> depth );

  next -> gradient.expand( gradient );
  next -> target.expand( target );

  next -> prior[ m_index ] = nextPrior;
  next -> state[ m_index ] = nextState;
}

#endif  //  ! ___PROGRESS_GUIDE__H
