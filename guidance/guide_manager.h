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
  Stack<BitMapID> m_restrictions;
  CubeID m_transposition;

protected:

  const Rubik<N> * m_cube;

  GuideManager();
  void restrict( const BitMapID );
  void setStep( const size_t );
  bool setRoot ( Node * node, const bool );
  bool nextNode( Node * node );
  bool optional() const;

  void setOptionalRoot ( Node * ) ;
  bool setScheduledRoot( Node * ) ;

  void setOptionalNext ( Node * ) ;
  bool setScheduledNext( Node * ) ;

  bool emptyPool( Node * node );

  void reset();
};

template< cube_size N >
GuideManager<N>::GuideManager()
  : m_scheduled( Node::MaxPatterns )
  , m_optional ( Node::MaxPatterns )
  , m_restrictions( Node::MaxPatterns )
{
  m_transposition = 0xFF;
}

template< cube_size N >
void GuideManager<N>::restrict( const BitMapID restriction )
{
  m_restrictions.push( restriction | 1 );
}


template< cube_size N >
void GuideManager<N>::setStep( const size_t step )
{
  NodeInit<N>::initGradients( m_restrictions[step] );
  Scheduler<N>::expand( m_scheduled, m_optional, step );
}

template<cube_size N>
bool GuideManager<N>::setRoot( Node * node, const bool symmetricSearch )
{
  if ( 0xFF == m_transposition )
  {
    const Guide * first = m_scheduled.empty() ? m_optional.begin() : m_scheduled.begin();
    m_transposition = first -> getTransposition( m_cube );
  }
  node -> reset();

  NodeInit<N>::setAsRoot( node, optional(), symmetricSearch );

  setOptionalRoot( node );
  return setScheduledRoot( node );
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
    P -> setOptionalRoot( m_cube, node, m_transposition );
  }
}

template< cube_size N >
bool GuideManager<N>::setScheduledRoot( Node * node )
{
  for ( auto P = m_scheduled.begin(); P != m_scheduled.end(); ++ P )
  {
    if ( ! P -> setScheduledRoot( m_cube, node, m_transposition ) )
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
      m_scheduled.push( *next, [next]( const Guide & p ) { return next -> stricterThan( p ); } );
      m_optional.pop();
    }
    else
    {
      *( P ++ ) = *next;
    }
  }

  return ! optional();
}

template< cube_size N >
void GuideManager<N>::reset()
{
  m_transposition = 0xFF;
  m_scheduled = Stack<Guide>( Node::MaxPatterns );
  m_optional  = Stack<Guide>( Node::MaxPatterns );
}

#endif  //  ! ___GUIDE_HANDLER_BASE__H

