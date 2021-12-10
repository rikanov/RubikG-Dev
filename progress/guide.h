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

public:

  Guide() = default;
  Guide( const size_t size, const PosID * pattern, AcceptFunction af, const size_t index, const CubeID trans = 0 );

  void setAsRoot  ( Node *, const Rubik<N> * ) const;
  void setAsChild ( Node *) const;

  bool restrict( Node * ) const;
  void expand( const Node *, BitMap &, BitMap & ) const;

  CubeID  transpose( const CubeID cubeID = 0 );
  CubeID  getTransposition( const Rubik<N> * ) const;

  bool solve( const Node * ) const;
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
void GuideFactory<N>::Guide::setAsRoot( Node * node, const Rubik<N> * cube ) const
{
  node -> prior[ m_index ] = PatternAPI::getPrior( cube, m_transposition );
  node -> state[ m_index ] = PatternAPI::getState( cube, m_transposition );
}

template< cube_size N >
void GuideFactory<N>::Guide::setAsChild( Node * node ) const
{
  const Node * n = node - 1;
  const CubeID   prior = n -> prior[ m_index ];
  const BitMapID state = n -> state[ m_index ];
  const RotID    rotID = n -> rotate;

  node -> prior[ m_index ] = PatternAPI::movePrior( prior, rotID );
  node -> state[ m_index ] = SubgroupAPI::lookUp( prior, state, rotID );
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
void GuideFactory<N>::Guide::expand( const Node* node, BitMap & gradient, BitMap & target ) const
{
  gradient.expand( EvaluatorAPI::gradient( node -> prior[ m_index ],  node -> state[ m_index ], node -> depth ) );
  target.expand( EvaluatorAPI::target( node -> prior[ m_index ],  node -> state[ m_index ], node -> depth ) );
}

template< cube_size N >
bool GuideFactory<N>::Guide::solve( const Node * node ) const
{
  return EvaluatorAPI::accepted( node -> state[ m_index ] ) && node -> alignedTo( m_index );
}

#endif  //  ! ___PROGRESS_GUIDE__H
