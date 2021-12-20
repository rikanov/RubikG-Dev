#ifndef ___PROGRESS_GUIDE__H
#define ___PROGRESS_GUIDE__H

#include <guide_base.h>


enum ProgressTask
{
  _Scheduled,
  _Optional
};

template< cube_size N >
class GuideFactory<N>::Guide: public GuideFactory<N>::GuideBase
{
public:

  Guide() = default;
  Guide( Pattern<N> pattern, AcceptFunction af, const size_t index, const CubeID trans = 0 );

  void setOptionalRoot ( const Rubik<N> * cube, Node * node, const CubeID trans );
  bool setScheduledRoot( const Rubik<N> * cube, Node * node, const CubeID trans );

  void setOptionalNode ( Node * next );
  bool setScheduledNode( Node * next );


  bool solveNode( const Node * node ) const;
};

template< cube_size N >
GuideFactory<N>::Guide::Guide( Pattern<N> pattern, AcceptFunction af, const size_t index, const CubeID trans )
  : GuideBase( pattern, af, index, trans )
{
}

template< cube_size N >
void GuideFactory<N>::Guide::setOptionalRoot( const Rubik<N> * cube, Node * node, const CubeID transposition )
{
  GuideBase::setNode( node );
  GuideBase::transpose( transposition );
  GuideBase::setAsRoot( cube );
  GuideBase::expand();
}

template< cube_size N >
bool GuideFactory<N>::Guide::setScheduledRoot(const Rubik<N> * cube, Node* node, const CubeID transposition )
{
  GuideBase::setNode( node );
  GuideBase::transpose( transposition );
  GuideBase::setAsRoot( cube );
  return GuideBase::restrict();
}

template< cube_size N >
void GuideFactory<N>::Guide::setOptionalNode( Node * next)
{
  GuideBase::setNode( next );
  GuideBase::setAsChild();
  GuideBase::expand();
}

template< cube_size N >
bool GuideFactory<N>::Guide::setScheduledNode( Node * next)
{
  GuideBase::setNode( next );
  GuideBase::setAsChild();
  return GuideBase::restrict();
}

template< cube_size N >
bool GuideFactory<N>::Guide::solveNode( const Node * node ) const
{
  const size_t index = GuideBase::index();
  const CubeID  prior = node -> prior[ index ];
  const GroupID state = node -> state[ index ];

  const BitMap32ID targetSet = EvaluatorAPI::target( prior, state, node -> depth );
  return EvaluatorAPI::accepted( state ) && node -> target.hasCommon( targetSet );
}

#endif  //  ! ___PROGRESS_GUIDE__H
