#ifndef ___GUIDE_HANDLER_BASE__H
#define ___GUIDE_HANDLER_BASE__H

#include <dynamic_stack.h>
#include <guide_scheduler.h>
#include <node_init.h>

template< cube_size N >
class GuideManager:  public Scheduler<N>
                  ,  protected NodeInit<N>
{
  using Guide = typename GuideFactory<N>::Guide;

  Stack<Guide> m_scheduled;
  Stack<Guide> m_optional;

  CubeID m_transposition;

protected:

  Rubik<N>  m_cube;

  GuideManager();
  void setStep( const size_t );
  bool setRoot ( Node * node );
  bool nextNode( Node * node );
  bool optional() const;

  void setOptionalRoot ( Node * ) ;
  bool setScheduledRoot( Node * ) ;

  void setOptionalNext ( Node * ) ;
  bool setScheduledNext( Node * ) ;

  bool emptyPool( Node * node );

  void add( Guide guide, const ProgressTask );
};

template< cube_size N >
GuideManager<N>::GuideManager()
  : m_scheduled( Node::MaxPatterns )
  , m_optional ( Node::MaxPatterns )
{
  m_transposition = 0xFF;
}

template< cube_size N >
void GuideManager<N>::setStep( const size_t size )
{
  Scheduler<N>::expand( m_scheduled, m_optional, size );
}

template<cube_size N>
bool GuideManager<N>::setRoot( Node * node )
{
  if ( 0xFF == m_transposition )
  {
    const Guide * first = m_scheduled.empty() ? m_optional.begin() : m_scheduled.begin();
    m_transposition = first -> getTransposition( &m_cube );
  }
  node -> reset();

  NodeInit<N>::setAsRoot( node, GuideManager<N>::optional() );

  GuideManager<N>::setOptionalRoot( node );
  return GuideManager<N>::setScheduledRoot( node );
}

template< cube_size N >
bool GuideManager<N>::nextNode( Node * node )
{
  node -> rotate = node -> gradient.next();
  Node * next = node + 1;
  next -> reset();
  GuideManager<N>::setOptionalNext( next );

  return NodeInit<N>::setAsChild( next, GuideManager<N>::optional() ) && GuideManager<N>::setScheduledNext( next );
}

template< cube_size N >
bool GuideManager<N>::optional() const
{
  return ! m_optional.empty();
}

template< cube_size N >
void GuideManager<N>::setOptionalRoot( Node * node )
{
  for ( auto P = m_optional.begin(); P != m_optional.end(); ++ P )
  {
    P -> setOptionalRoot( &m_cube, node, m_transposition );
  }
}

template< cube_size N >
bool GuideManager<N>::setScheduledRoot( Node * node )
{
  for ( auto P = m_scheduled.begin(); P != m_scheduled.end(); ++ P )
  {
    if ( ! P -> setScheduledRoot( &m_cube, node, m_transposition ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
void GuideManager<N>::setOptionalNext( Node * next )
{
  for ( auto P = m_optional.begin(); P != m_optional.end(); ++ P)
  {
    P -> setOptionalNode( next );
  }
}

template< cube_size N >
bool GuideManager<N>::setScheduledNext( Node * next )
{
  for ( auto P = m_scheduled.begin(); P != m_scheduled.end(); ++ P )
  {
    if ( ! P -> setScheduledNode( next ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
bool GuideManager<N>::emptyPool( Node * node )
{
  for ( auto P = m_optional.begin(), next = P; P != m_optional.end(); ++ next )
  {
    if ( next -> solveNode( node ) )
    {
      m_scheduled.push( *next );
      m_optional.pop();
    }
    else
    {
      *( P ++ ) = *next;
    }
  }

  // a small optimization
  if ( m_optional.size() == 1 )
  {
    m_scheduled.push( m_optional[0] );
    m_optional.pop();
    return false;
  }

  return ! optional();
}

#endif  //  ! ___GUIDE_HANDLER_BASE__H

