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

  ObjID setInsight  ( const PosID *, const size_t, const ObjID obj = 0 );
  void  toSolve     ( const Rubik<N> & );
  void  update      ( void );
  
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
ObjID Engine<N>::setInsight( const PosID * posID, const size_t size, const ObjID obj )
{
  ObjID modified = 0;
  if ( 0 == obj )
  {
    ( m_endInsights ++ ) -> init( posID, size );
    modified = ( m_endInsights - m_insights );
  }
  else
  {
    m_insights[ obj - 1 ].init( posID, size );
    modified = obj;
  }
  return modified;
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
void Engine<N>::update()
{
  for ( auto pInsight = m_insights; pInsight != m_endInsights; ++ pInsight )
  {
    pInsight -> update();
  }  
}

template< cube_size N >
void Engine<N>::move( const RotID rotID )
{
  for ( auto pInsight = m_insights; pInsight != m_endInsights; ++ pInsight )
  {
    pInsight -> move( rotID );
  }
}

template< cube_size N >
BitMapID Engine<N>::progress( const RotID rotID, const DistID distance )
{
  BitMapID result = m_allowed[ rotID ];

  Insight<N> * pInsight = m_insights;
  BitMap32ID aim = ( 1 << 24 ) - 1;
  while (  result > 0 && aim > 0 && pInsight != m_endInsights )
  {
    result &= pInsight -> operate( rotID, distance );
    aim    &= pInsight -> aim( distance );
    ++ pInsight;
  }
  if ( 0 == result || 0 == aim )
  {
    const RotID inv = CRotations<N>::GetInvRotID( rotID );
    while ( m_insights <= -- pInsight )
      pInsight -> move( inv );
  }

  return aim ? result : 0;
}

template< cube_size N >
BitMapID Engine<N>::gradient( const DistID distance )
{
  BitMapID result = m_allowed[ 0 ];

  const Insight<N> * pInsight = m_insights;
  BitMap32ID aim = ( 1 << 24 ) - 1;
  while ( result > 0 && pInsight != m_endInsights )
  {
    result &= pInsight -> gradient( distance );
    aim    &= pInsight -> aim( distance );
    ++ pInsight;
  }

  return aim ? result : 0;
}

template< cube_size N >
Engine<N>::~Engine()
{
}

#endif  //  ! ENGINE__H_INCLUDED
