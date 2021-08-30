#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED

/*
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

  size_t   m_rootComplexity;
  DistID   m_searchDepth;
  Sequence m_result;
  
  void rotate( const RotID rotID )
  {
    for ( auto p = m_insights; p < m_lastInsight; ++ p )
      (*p) -> rotate( rotID );
  }

  _insight getGuide() const
  {
    for ( auto p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() >= m_searchDepth )
        return *p;
    }
    return nullptr;
  }

  bool isSolvable() const
  {
    for ( auto * p = m_insights; p < m_lastInsight; ++ p )
    {
      if ( ( *p ) -> distance() > m_searchDepth )
        return false;
    }
    return true;
  }

  bool startDeepSearch( const DistID );
  bool guidedSearch( _insight );
  bool deepSearch ( const Axis refA, const Layer refL );
  size_t complexity( const DistID weight = 0 ) const;

public:
  Engine( Rubik<N> & );
  ~Engine();

  void operator << ( _insight );
  void swap( _insight , _insight );

  Sequence solve  ( const int depth, const bool E = true );

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
bool Engine<N>::startDeepSearch( const DistID depth )
{
  for ( int d = 0; d < depth; ++ d)
  {
    m_rootComplexity = d > 10 ? complexity( 1 ) : 0;
    m_searchDepth = d; clog_( "\rdepth: ", d, ' ' );
    if ( deepSearch( _NA, 0 ) )
    {
      NL();
      return true;
    }
  }
  NL();
  return false;
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

  --m_searchDepth;
  for( RotID next = insight -> start(); next != 0; next = insight -> next() )
  {
    m_result << next;
    rotate( next );
    if ( guidedSearch( insight ) )
    {
      return true;
    }
    rotate( _crot::GetInvRotID( next ) ); // revert
    m_result.back();
  }
  ++m_searchDepth;

  return false;
}

template< cube_size N >
bool Engine<N>::deepSearch( const Axis refA, const Layer refL )
{
  if ( getGuide() )
  {
    return guidedSearch( getGuide() );
  }
  -- m_searchDepth;
  for( Axis axis : { _X, _Y, _Z } )
  {
    for( Layer layer = axis == refA ? refL + 1 : 0; layer < _crot::NT; ++ layer )
    {
      const RotID rotID = _crot::GetRotID( axis, layer, 1 );
      RotID turned =  rotID;
      for( Turn turn: { 1, 2, 3} )
      {
        rotate( rotID );
        m_result << turned++;
        if ( deepSearch( axis, layer ) )
        {
          return true;
        }
        m_result.back();
      }
      rotate( rotID ); // fourth turn --> revert
    }
  }
  ++ m_searchDepth;
  return false;
}

template< cube_size N >
Sequence Engine<N>::solve( const int depth, const bool E )
{
  m_result.reset();
  for( auto P = m_insights; P != m_lastInsight; ++ P )
  {
    ( *P ) -> set( m_rubik );
  }
  if( startDeepSearch( depth ) && E )
  {
    m_rubik.rotate( m_result );
  }
  return m_result;
}

template<cube_size N> size_t Engine<N>::complexity( const DistID weight ) const
{
  size_t result = 0;
  for( auto P = m_insights; P != m_lastInsight; ++ P )
  {
    const DistID dist = weight + ( *P ) -> distance();
    result += dist * dist;
  }

  return result;
}

template< cube_size N >
Engine<N>::~Engine()
{
  delete[] m_insights;
}
*/

#endif  //  ! ENGINE__H_INCLUDED
