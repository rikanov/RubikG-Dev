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
  BitMapID     m_allowed  [ CRotations<N>::AllRotIDs ] = {};
  Insight<N>   m_insights [ INSIGHT_BOUND ];
  Insight<N> * m_endInsights;

  void init();

public:
  Engine();
  ~Engine();

  void addInsight  ( const PosID *, const size_t );
  void toSolve     ( const Rubik<N> & );
  
  void     move     ( const RotID );
  BitMapID progress ( const RotID, const DistID );
  BitMapID gradient ( const DistID );
};


template< cube_size N >
Engine<N>::Engine()
 :  m_endInsights ( nullptr )
{
  init();
}

template< cube_size N >
void Engine<N>::init()
{
  m_endInsights = m_insights;

  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 1;


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
void Engine<N>::addInsight( const PosID *, const size_t )
{
  
}

template< cube_size N >
void Engine<N>::toSolve( const Rubik<N> & R )
{
  for ( auto pInsight = m_insights; pInsight != m_endInsights; ++ pInsight )
  {
    pInsight -> toSolve( R );
  }
}

template< cube_size N >
void Engine<N>::move( const RotID rotID )
{
  for ( auto pInsight = m_insights; pInsight != m_endInsights; ++ pInsight )
    ( *pInsight ) -> move( rotID );
}

template< cube_size N >
BitMapID Engine<N>::progress( const RotID rotID, const DistID distance )
{
  BitMapID result = m_allowed[ rotID ];

  auto pInsight = m_insights;
  while ( result > 0 && pInsight != m_endInsights )
    result &= *( pInsight ++ ) -> operate( rotID, distance );

  if ( 0 == result )
  {
    const RotID inv = CRotations<N>::GetInvRotID( rotID );
    while ( m_insights <= -- pInsight )
      ( *pInsight ) -> move( inv );
  }

  return result;
}

template< cube_size N >
BitMapID Engine<N>::gradient( const DistID distance )
{
  BitMapID result = m_allowed[ 0 ];

  auto pInsight = m_insights;
  while ( result > 0 && pInsight != m_endInsights )
    result &= *( pInsight ++ ) -> gradient( distance );

  return result;
}

template< cube_size N >
Engine<N>::~Engine()
{
}

#endif  //  ! ENGINE__H_INCLUDED
