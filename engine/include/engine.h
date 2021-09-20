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
  BitMap32ID   m_target = ( 1 << 24 ) - 1;
  Insight<N>   m_insights [ INSIGHT_BOUND ];
  Insight<N> * m_endOfInsights;
  CubeID       m_transposeSolution;

  void init();

public:
  Engine();
  ~Engine();

  void addInsight ( const PosID *, const size_t, const CubeID trans = 0 );

  void  toSolve  ( const Rubik<N> & );
  void  update   ( void );
  bool  empty    ( void ) const { return m_insights == m_endOfInsights; }
  
  void     move     ( const RotID );
  BitMapID progress ( const RotID, const DistID );
  BitMapID gradient ( const DistID );

  bool     unambiguous () const;
  void     fixCube();
};


template< cube_size N >
Engine<N>::Engine()
 :  m_endOfInsights ( nullptr )
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

template<cube_size N> void Engine<N>::fixCube()
{clog( "prior:", Simplex::GetCube( m_insights -> prior() ).toString(), Simplex::GetCube( m_transposeSolution ).toString(), CPositions<N>::GetCoord( m_insights -> priorPos() ).toString() );
  m_target = 1 <<  m_insights -> prior();
  BitMap::Print( m_target, 24, 8 );
}

template< cube_size N >
void Engine<N>::addInsight( const PosID * posID, const size_t size, const CubeID trans )
{
  ( m_endOfInsights ++ ) -> init( posID, size, trans );
}

template< cube_size N >
void Engine<N>::toSolve( const Rubik<N> & R )
{
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> toSolve( R, m_transposeSolution, pInsight == m_insights );
  }
}

template< cube_size N >
void Engine<N>::update()
{
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
  BitMapID result = m_allowed[ rotID ];

  Insight<N> * pInsight = m_insights;
  BitMap32ID aim = m_target;
  while ( result > 0 && aim > 0 && pInsight != m_endOfInsights )
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
  BitMap32ID aim = m_target;
  while ( result > 0 && aim > 0 && pInsight != m_endOfInsights )
  {
    result &= pInsight -> gradient( distance );
    aim    &= pInsight -> aim( distance );
    ++ pInsight;
  }

  return aim ? result : 0;
}

template< cube_size N >
bool Engine<N>::unambiguous() const
{
  const Insight<N> * pInsight = m_insights;
  for ( CubeID priorID = m_insights -> prior(); pInsight != m_endOfInsights &&  pInsight -> prior() == priorID; ++ pInsight )
  {
  }
  return pInsight == m_endOfInsights;
}

template< cube_size N >
Engine<N>::~Engine()
{
}

#endif  //  ! ENGINE__H_INCLUDED
