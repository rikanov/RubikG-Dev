#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED

#include <insight.h>

template< cube_size N >
class Engine
{
  using _crot = CExtRotations<N>;

  static constexpr size_t s_Size = CPositions<N>::GetSize();
  Insight <N> ** m_insights;
  Insight <N> ** m_lastInsight;
  Rubik   <N> &  m_rubik;

  DistID  m_depth;
  RotID * m_stackPointer;
  RotID   m_rotStack[200];
  void rotate( const RotID rotID )
  {
    for ( Insight<N> ** p = m_insights; p < m_lastInsight; ++ p )
      (*p) -> rotate( rotID );
  }

  Insight<N> * getGuide() const
  {
    for ( Insight<N> ** p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() >= m_depth )
        return *p;
    }
    return nullptr;
  }

  bool isSolvable() const
  {
    for ( Insight<N> ** p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() > m_depth )
        return false;
    }
    return true;
  }

  bool guidedSearch( Insight<N> * );

public:
  Engine( Rubik<N> & );
  ~Engine();

  void operator << ( Insight<N> & );
  void operator << ( Insight<N> * );

  void run  ( const int depth );
  bool exec ( const Axis refA, const Layer refL );
};

template< cube_size N > 
Engine<N>::Engine( Rubik <N> & rubik )
  : m_insights( new Insight<N> * [ s_Size ]{} )
  , m_lastInsight( m_insights )
  , m_rubik( rubik )
{
}

template< cube_size N >
void Engine<N>::operator << ( Insight<N> & next )
{
  *( m_lastInsight ++ ) = &next;
}

template< cube_size N >
void Engine<N>::operator << ( Insight<N> * next )
{
  *( m_lastInsight ++ ) = next;
}

template< cube_size N >
bool Engine<N>::guidedSearch( Insight<N> * insight )
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
void Engine<N>::run( const int depth )
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
      m_rubik.rotate( m_rotStack );
      break;
    }
  }
}

template< cube_size N >
Engine<N>::~Engine()
{
  delete[] m_insights;
}

#endif  //  ! ENGINE__H_INCLUDED
