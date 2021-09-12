#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED


#include <insight.h>

template< cube_size N >
class Engine
{
  static constexpr size_t INSIGHT_BOUND = 40;
  BitMapID    * m_allowed;
  Insight<N> ** m_insights;
  Insight<N> ** m_nextInsight;

  void init();

public:
  Engine();
  ~Engine();

  void operator << ( Insight<N> & );
  void operator << ( Insight<N> * );

  void     move     ( const RotID );
  BitMapID progress ( const RotID, const DistID );
};


template< cube_size N >
Engine<N>::Engine()
 :  m_allowed     ( nullptr )
 ,  m_insights    ( nullptr )
 ,  m_nextInsight ( nullptr )
{
  init();
}

template< cube_size N >
void Engine<N>::init()
{
  m_allowed     = new BitMapID     [ CRotations<N>::AllRotIDs ] {};
  m_insights    = new Insight<N> * [ INSIGHT_BOUND ] {};
  m_nextInsight = m_insights;

  constexpr BitMapID allRotations = ( 1ULL << CRotations<N>::AllRotIDs ) - 2;


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
void Engine<N>::operator << ( Insight<N> & next )
{
  *( m_nextInsight ++ ) = &next;
}

template< cube_size N >
void Engine<N>::operator << ( Insight<N> * next )
{
  *( m_nextInsight ++ ) = next;
}

template< cube_size N >
void Engine<N>::move( const RotID rotID )
{
  for ( auto pInsight = m_insights; pInsight != m_nextInsight; ++ pInsight )
    *pInsight -> move( rotID );
}

template< cube_size N >
BitMapID Engine<N>::progress( const RotID rotID, const DistID distID )
{
  BitMapID result = m_allowed[ rotID ];

  auto pInsight = m_insights;
  while ( result > 0 && pInsight != m_nextInsight )
    result &= *( pInsight ++ ) -> operate( rotID, distID );

  if ( 0 == result )
  {
    const RotID inv = CRotations<N>::GetInvRotID( rotID );
    while ( m_insights <= -- pInsight )
      *pInsight -> move( inv );
  }

  return result;
}

template< cube_size N >
Engine<N>::~Engine()
{
  delete[] m_allowed;
  delete[] m_insights;
}

#endif  //  ! ENGINE__H_INCLUDED
