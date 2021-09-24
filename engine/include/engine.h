#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED


#include <insight.h>
#include <bitmap_set.h>
#include <rubik.h>
#include <text_output.h>

template< cube_size N >
class Engine
{
  static constexpr size_t INSIGHT_BOUND = 40;
  BitMapID     m_progress;
  BitMapID     m_allowed  [ CRotations<N>::AllRotIDs ] = {};
  BitMap32ID   m_target = ( 1 << 24 ) - 1;
  Insight<N>   m_insights [ INSIGHT_BOUND ];
  Insight<N> * m_endOfInsights;
  CubeID       m_transposeSolution;

  void init();
  void findStage();

public:
  Engine();
  ~Engine();

  void addInsight ( const PosID *, const size_t, const CubeID trans = 0, AcceptFunction af = Accept<N>::Normal );

  void  toSolve  ( const Rubik<N> & );
  void  update   ( void );
  bool  empty    ( void ) const { return m_insights == m_endOfInsights; }
  
  void     move     ( const RotID );
  BitMapID progress ( const RotID, const DistID );
  BitMapID gradient ( const DistID );

  bool unambiguous () const;
  void fixCube()
  {
    m_target = ( 1ULL << m_insights -> prior() );
  }
  
  BitMap32ID target() const
  {
    return m_target;
  }
  void target( const BitMap32ID tr )
  {
    m_target = tr;
  }
  
  void close()
  {
    m_progress = ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
  }

  bool closed() const
  {
    return m_progress == ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
  }
};


template< cube_size N >
Engine<N>::Engine()
 :  m_progress( 0 )
 ,  m_endOfInsights ( nullptr )
{
  init();
}

template< cube_size N >
void Engine<N>::init()
{
  m_endOfInsights = m_insights;
  m_transposeSolution = 0;
  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 1; // include the solved bit


  RotID rotID           = 0;
  BitMapID allow        = allRotations;
  m_allowed[ rotID ++ ] = allRotations;

  all_rot( axis, layer, turn, N )
  {
    if ( 1 == turn && 0 == layer )
      allow = allRotations;

    if ( 1 == turn )
      allow -= 7ULL << ( 3 * ( axis * N + layer ) + 1 );

    m_allowed[ rotID ++ ] = allow;
  }
}

template< cube_size N >
void Engine<N>::addInsight( const PosID * posID, const size_t size, const CubeID trans, AcceptFunction af )
{
  ( m_endOfInsights ++ ) -> init( posID, size, trans, af );
}

template< cube_size N >
void Engine<N>::toSolve( const Rubik<N> & R )
{
  DistID   min   = 0xFF;
  BitMapID next  = 1;
  BitMapID stage = 1;
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> toSolve( R, m_transposeSolution, pInsight == m_insights );
    const DistID dist = pInsight -> distance();
    if ( 0 == dist && pInsight -> prior() == m_insights -> prior() )
    {
      m_progress |= 1ULL << ( pInsight - m_insights );
    }
    else if ( dist < min )
    {
      min = dist;
      stage = next;
    }
    next <<= 1;
  }
  m_progress |= stage;
  BitMap::Print( m_progress, m_endOfInsights - m_insights );
}

template< cube_size N >
void Engine<N>::update()
{
  m_target = ( 1 << 24 ) - 1;
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> update();
  }  
}

template< cube_size N >
void Engine<N>::move( const RotID rotID )
{
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> move( rotID );
  }
}

template< cube_size N >
BitMapID Engine<N>::progress( const RotID rotID, const DistID distance )
{
  BitMapID gradient = m_allowed[ rotID ];
  BitMapID step = 1;

  for ( const Insight<N> * P = m_insights; gradient > 0 && m_target > 0 && P != m_endOfInsights; ++ P )
  {
    if ( m_progress & step )
    {
      gradient &= P -> gradient( distance );
      m_target &= P -> aim( distance );
    }
    step <<= 1;
  }

  if ( 0 == distance && ( gradient & 1 ) )
  {
    return gradient;
  }
  
  return m_target ? gradient : 0; 
}

template< cube_size N >
bool Engine<N>::unambiguous() const
{
  return m_insights -> aim( 0 ) == m_target;
}

template< cube_size N >
Engine<N>::~Engine()
{
}

#endif  //  ! ENGINE__H_INCLUDED
