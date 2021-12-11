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

  Rubik<N>  m_cube;

  Array<Guide> m_scheduled;
  Array<Guide> m_optional;

  Guide * m_nextScheduled;
  Guide * m_nextOptional;

  CubeID m_transposition;


protected:

  GuideHandler();
  ~GuideHandler();

  void newTransposition();
  void add( Guide guide, const ProgressTask );

  bool setRoot ( Node * node );
  bool nextNode( Node * node );

  bool emptyPool( Node * );
  bool optional() const
  {
    return m_optional.begin() != m_nextOptional;
  }

  void setCube( const Rubik<N> & );
  void setCube( const Sequence & );
  void showCube() const;
  const Rubik<N> & getCube() const;

};

template< cube_size N >
GuideHandler<N>::GuideHandler()
  : m_scheduled( Node::MaxPatterns )
  , m_optional ( Node::MaxPatterns )
{
  m_nextScheduled = m_scheduled.begin();
  m_nextOptional  = m_optional.begin();
  newTransposition();
}

template< cube_size N >
void GuideHandler<N>::newTransposition()
{
  m_transposition = 0xFF; // invalid
}

template< cube_size N >
void GuideHandler<N>::add( GuideHandler::Guide guide, const ProgressTask task )
{
  if ( _Scheduled == task && ! ( 0xFF == m_transposition ) )
  {
    m_transposition = guide.getTransposition( &m_cube );
  }
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
bool GuideHandler<N>::setRoot( Node * node )
{
  newTransposition();
  node -> reset();

  NodeInit<N>::setAsRoot( node, optional() );

  for ( auto P = m_optional.begin(); P != m_nextOptional; ++ P )
  {
    P -> setNode( node );
    P -> transpose( m_transposition );
    P -> setAsRoot( &m_cube );
    P -> expand();
  }


  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    P -> setNode( node );
    P -> transpose( m_transposition );
    P -> setAsRoot( &m_cube );
    if ( ! P -> restrict() )
    {
      return false;
    }
  }

  return true;
}

template< cube_size N >
bool GuideHandler<N>::nextNode( Node * node )
{
  node -> rotate = node -> gradient.next();
  Node * next = node + 1;
  next -> reset();

  for ( auto P = m_optional.begin(); P != m_nextOptional; ++ P)
  {
    P -> setNode( next );
    P -> setAsChild();
    P -> expand();
  }
  if ( ! NodeInit<N>::setAsChild( next, optional() ) )
  {
    return false;
  }

  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    P -> setNode( next );
    P -> setAsChild();
    if ( ! P -> restrict() )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
bool GuideHandler<N>::emptyPool( Node * node )
{
  for ( auto P = m_optional.begin(), next = P; P != m_nextOptional; ++ next )
  {
    next -> setNode( node );
    if ( next -> solveNode() )
    {
      *( m_nextScheduled ++ ) = *next;
      -- m_nextOptional;
    }
    else
    {
      *( P ++ ) = *next;
    }
  }

  // a small optimization
  if ( m_nextOptional - m_optional.begin() == 1 )
  {
    *( m_nextScheduled ++ ) = m_optional[0];
    -- m_nextOptional;
    return false;
  }

  return ! optional();
}

template< cube_size N >
void GuideHandler<N>::setCube( const Rubik<N> & cube )
{
  m_cube = cube;
}

template< cube_size N >
void GuideHandler<N>::setCube( const Sequence & seq )
{
  m_cube.rotate( seq );
}

template< cube_size N >
void GuideHandler<N>::showCube() const
{
  m_cube.print();
}

template< cube_size N >
const Rubik<N> & GuideHandler<N>::getCube() const
{
  return m_cube;
}

template< cube_size N >
GuideHandler<N>::~GuideHandler()
{
}

#endif  //  ! ___GUIDE_HANDLER__H
