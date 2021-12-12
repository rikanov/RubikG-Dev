#ifndef ___GUIDE_HANDLER__H
#define ___GUIDE_HANDLER__H

#include <guide_handler_base.h>
#include <node_init.h>


template< cube_size N >
class GuideHandler:  protected GuideHandlerBase<N>
                  ,  protected NodeInit<N>
{
protected:

  bool setRoot ( Node * node );
  bool nextNode( Node * node );
};

template<cube_size N>
bool GuideHandler<N>::setRoot( Node * node )
{
  GuideHandlerBase<N>::newTransposition();
  node -> reset();

  NodeInit<N>::setAsRoot( node, GuideHandlerBase<N>::optional() );

  GuideHandlerBase<N>::setOptionalRoot( node );
  return GuideHandlerBase<N>::setScheduledRoot( node );
}

template< cube_size N >
bool GuideHandler<N>::nextNode( Node * node )
{
  node -> rotate = node -> gradient.next();
  Node * next = node + 1;
  next -> reset();
  GuideHandlerBase<N>::setOptionalNext( next );

  return NodeInit<N>::setAsChild( next, GuideHandlerBase<N>::optional() ) && GuideHandlerBase<N>::setScheduledNext( next );
}

#endif  //  ! ___GUIDE_HANDLER__H
