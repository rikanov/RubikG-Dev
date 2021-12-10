#ifndef ___GUIDE_HANDLER__H
#define ___GUIDE_HANDLER__H

#include <dynamic_array.h>
#include <node_init.h>
#include <guide.h>

enum ProgressTask
{
  _Scheduled,
  _Optional
};

template< cube_size N >
class GuideHandler:  protected NodeInit<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Array<Guide> m_scheduled;
  Array<Guide> m_optional;

  Guide * m_nextScheduled;
  Guide * m_nextOptional;

protected:

  GuideHandler();
  ~GuideHandler();

  void add( Guide guide, const ProgressTask );

  bool setRoot ( Node * node, const Rubik<N> *, const CubeID );
  bool nextNode( Node * node ) const;

  bool emptyPool( const Node * );
};

template< cube_size N >
GuideHandler<N>::GuideHandler()
  : m_scheduled( Node::MaxPatterns )
  , m_optional ( Node::MaxPatterns )
{
  m_nextScheduled = m_scheduled.begin();
  m_nextOptional  = m_optional.begin();
}

template< cube_size N >
void GuideHandler<N>::add( GuideHandler::Guide guide, const ProgressTask task )
{
  if ( _Scheduled == task )
  {
    *( m_nextScheduled ++ ) = guide;
  }
  else
  {
    *( m_nextOptional ++ ) = guide;
  }
}

template<cube_size N>
bool GuideHandler<N>::setRoot( Node * node, const Rubik<N> * cube, const CubeID trans )
{
  NodeInit<N>::setAsRoot( node );

  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    P -> transpose( trans );
    P -> setAsRoot( node, cube );
    if ( ! P -> restrict( node) )
    {
      return false;
    }
  }

  if ( m_optional.begin() == m_nextOptional )
  {
    return true;
  }

  BitMap gradient;
  BitMap target;
  for ( auto P = m_optional.begin(); P != m_nextOptional; ++ P )
  {
    P -> transpose( trans );
    P -> setAsRoot( node, cube );
    P -> expand( node, gradient, target );
  }

  return true;
}

template< cube_size N >
bool GuideHandler<N>::nextNode( Node * node ) const
{
  node -> rotate = node -> gradient.next();
  Node * next = node + 1;
  NodeInit<N>::setAsChild( next );

  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    P -> setAsChild( next );
    if ( ! P -> restrict( next ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
bool GuideHandler<N>::emptyPool( const Node * node )
{
  for ( auto P = m_optional.begin(), next = P; P != m_nextOptional; ++ next )
  {
    if ( next -> solve( node ) )
    {
      -- m_nextOptional;
    }
    else
    {
      *( P ++ ) = *next;
    }
  }
  return m_optional.begin() == m_nextOptional;
}

template< cube_size N >
GuideHandler<N>::~GuideHandler()
{
}

#endif  //  ! ___GUIDE_HANDLER__H
