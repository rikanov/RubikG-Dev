#ifndef ___GUIDE_HANDLER__H
#define ___GUIDE_HANDLER__H

#include <guide_manager_base.h>
#include <node_init.h>


template< cube_size N >
class GuideManager:  protected GuideManagerBase<N>
                  ,  protected NodeInit<N>
{
protected:

  bool setRoot ( Node * node );
  bool nextNode( Node * node );
};

template<cube_size N>
bool GuideManager<N>::setRoot( Node * node )
{
  GuideManagerBase<N>::newTransposition();
  node -> reset();

  NodeInit<N>::setAsRoot( node, GuideManagerBase<N>::optional() );

  GuideManagerBase<N>::setOptionalRoot( node );
  return GuideManagerBase<N>::setScheduledRoot( node );
}

template< cube_size N >
bool GuideManager<N>::nextNode( Node * node )
{
  node -> rotate = node -> gradient.next();
  Node * next = node + 1;
  next -> reset();
  GuideManagerBase<N>::setOptionalNext( next );

  return NodeInit<N>::setAsChild( next, GuideManagerBase<N>::optional() ) && GuideManagerBase<N>::setScheduledNext( next );
}

#endif  //  ! ___GUIDE_HANDLER__H
