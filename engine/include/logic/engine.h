#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED

#include <insight.h>
#include <sequence.h>

template< cube_size N >
class Engine
{
  using _crot = CExtRotations<N>;
  using _insight = std::shared_ptr< Insight<N> >;

  static constexpr size_t s_Size = CPositions<N>::GetSize();

  _insight * m_insights;
  _insight * m_lastInsight;

  Rubik<N> & m_rubik;

  DistID  m_depth;
  RotID * m_stackPointer;
  RotID   m_rotStack[200];
  void rotate( const RotID rotID )
  {
    for ( auto p = m_insights; p < m_lastInsight; ++ p )
      (*p) -> rotate( rotID );
  }

  _insight getGuide() const
  {
    for ( auto p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() >= m_depth )
        return *p;
    }
    return nullptr;
  }

  bool isSolvable() const
  {
    for ( auto * p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() > m_depth )
        return false;
    }
    return true;
  }

  bool guidedSearch( _insight );
  bool exec ( const Axis refA, const Layer refL );

public:
  Engine( Rubik<N> & );
  ~Engine();

  void operator << ( _insight );
  void swap( _insight , _insight );

  Sequence run  ( const int depth );

};

template< cube_size N > 
Engine<N>::Engine( Rubik <N> & rubik )
  : m_insights( new _insight [ s_Size ]{} )
  , m_lastInsight( m_insights )
  , m_rubik( rubik )
{
}

template< cube_size N >
void Engine<N>::operator << ( _insight next )
{
  *( m_lastInsight ++ ) = next;
}

template< cube_size N >
void Engine<N>::swap ( _insight A, _insight B )
{
  for ( auto p = m_insights; p < m_lastInsight; ++ p )
  {
    if ( *p == A )
    {
      *p = B;
      return;
    }
  }
}

template< cube_size N >
bool Engine<N>::guidedSearch( _insight insight )
{
  if ( insight == nullptr || ! isSolvable() )
  {
    return false;
  }

  if ( insight -> distance() == 0 )
  {
    return true;
  }

  --m_depth;
  ++m_stackPointer;
  for( RotID next = insight -> start(); next != 0; next = insight -> next() )
  {
    *m_stackPointer = next;
    rotate( next );
    if ( guidedSearch( insight ) )
    {
      return true;
    }
    rotate( _crot::GetInvRotID( next ) ); // revert
  }
  --m_stackPointer;
  ++m_depth;

  return false;
}

template< cube_size N >
bool Engine<N>::exec( const Axis refA, const Layer refL )
{
  if ( getGuide() )
  {
    return guidedSearch( getGuide() );
  }
  ++ m_stackPointer;
  -- m_depth;
  for( Axis axis : { _X, _Y, _Z } )
  {
    for( Layer layer = axis == refA ? refL + 1 : 0; layer < _crot::NT; ++ layer )
    {
      const RotID rotID = _crot::GetRotID( axis, layer, 1 );
      RotID turned =  rotID;
      for( Turn turn: { 1, 2, 3} )
      {
        rotate( rotID );
        *m_stackPointer = turned ++;
        if ( exec( axis, layer ) )
        {
          return true;
        }
      }
      rotate( rotID ); // fourth turn --> revert
    }
  }
  ++ m_depth;
  -- m_stackPointer;
  return false;
}

template< cube_size N >
Sequence Engine<N>::run( const int depth )
{
  for( auto P = m_insights; P != m_lastInsight; ++ P )
  {
    ( *P ) -> set( m_rubik );
  }
  m_stackPointer = m_rotStack - 1;
  for ( int d = 0; d < depth; ++ d)
  {
    m_depth = d;
    if ( exec( _NA, 0 ) )
    {
      *( m_stackPointer + 1 ) = 0; // termination sign
      break;
    }
  }
  return Sequence( m_rotStack, 200);
}

template< cube_size N >
Engine<N>::~Engine()
{
  delete[] m_insights;
}

#endif  //  ! ENGINE__H_INCLUDED
