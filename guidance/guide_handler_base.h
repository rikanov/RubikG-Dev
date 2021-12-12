#ifndef ___GUIDE_HANDLER_BASE__H
#define ___GUIDE_HANDLER_BASE__H

#include <dynamic_array.h>
#include <guide.h>

enum ProgressTask
{
  _Scheduled,
  _Optional
};

template< cube_size N >
class GuideHandlerBase
{
  using Guide = typename GuideFactory<N>::Guide;

  Rubik<N>  m_cube;

  Array<Guide> m_scheduled;
  Array<Guide> m_optional;

  Guide * m_nextScheduled;
  Guide * m_nextOptional;

  CubeID m_transposition;

protected:

  GuideHandlerBase();
  bool optional() const;

  void setOptionalRoot ( Node * ) ;
  bool setScheduledRoot( Node * ) ;

  void setOptionalNext ( Node * ) ;
  bool setScheduledNext( Node * ) ;

  void newTransposition();

  bool emptyPool( Node * node );

  void add( Guide guide, const ProgressTask );
  void setCube( const Rubik<N> & );
  void setCube( const Sequence & );
  void showCube() const;
  const Rubik<N> & getCube() const;
};

template< cube_size N >
GuideHandlerBase<N>::GuideHandlerBase()
  : m_scheduled( Node::MaxPatterns )
  , m_optional ( Node::MaxPatterns )
{
  m_nextScheduled = m_scheduled.begin();
  m_nextOptional  = m_optional.begin();
  newTransposition();
}

template< cube_size N >
bool GuideHandlerBase<N>::optional() const
{
  return m_optional.begin() != m_nextOptional;
}

template< cube_size N >
void GuideHandlerBase<N>::setOptionalRoot( Node * node )
{
  for ( auto P = m_optional.begin(); P != m_nextOptional; ++ P )
  {
    P -> setOptionalRoot( &m_cube, node, m_transposition );
  }
}

template< cube_size N >
bool GuideHandlerBase<N>::setScheduledRoot( Node * node )
{
  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    if ( ! P -> setScheduledRoot( &m_cube, node, m_transposition ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
void GuideHandlerBase<N>::setOptionalNext( Node * next )
{
  for ( auto P = m_optional.begin(); P != m_nextOptional; ++ P)
  {
    P -> setOptionalNode( next );
  }
}

template< cube_size N >
bool GuideHandlerBase<N>::setScheduledNext( Node * next )
{
  for ( auto P = m_scheduled.begin(); P != m_nextScheduled; ++ P )
  {
    if ( ! P -> setScheduledNode( next ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
void GuideHandlerBase<N>::newTransposition()
{
  m_transposition = 0xFF; // invalid
}

template< cube_size N >
void GuideHandlerBase<N>::add( GuideHandlerBase::Guide guide, const ProgressTask task )
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

template< cube_size N >
bool GuideHandlerBase<N>::emptyPool( Node * node )
{
  for ( auto P = m_optional.begin(), next = P; P != m_nextOptional; ++ next )
  {
    if ( next -> solveNode( node ) )
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
void GuideHandlerBase<N>::setCube( const Rubik<N> & cube )
{
  m_cube = cube;
}

template< cube_size N >
void GuideHandlerBase<N>::setCube( const Sequence & seq )
{
  m_cube.rotate( seq );
}

template< cube_size N >
void GuideHandlerBase<N>::showCube() const
{
  m_cube.print();
}

template< cube_size N >
const Rubik<N> & GuideHandlerBase<N>::getCube() const
{
  return m_cube;
}

#endif  //  ! ___GUIDE_HANDLER_BASE__H

