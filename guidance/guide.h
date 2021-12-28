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
  Guide( const Subgroup & subgroup, AcceptFunction af, const size_t index, const CubeID trans );

  void setOptionalRoot ( const Rubik<N> * cube, Node * node, const CubeID trans );
  bool setScheduledRoot( const Rubik<N> * cube, Node * node, const CubeID trans );

  void setOptionalNode ( Node * next );
  bool setScheduledNode( Node * next );

};

template< cube_size N >
GuideFactory<N>::Guide::Guide( const Subgroup & subgroup, AcceptFunction af, const size_t index, const CubeID trans )
  : GuideBase( subgroup, af, index)
{
  EvaluatorAPI::rebase( trans );
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


#endif  //  ! ___PROGRESS_GUIDE__H
