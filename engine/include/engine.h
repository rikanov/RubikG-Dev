#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED


#include <insight.h>
#include <bitmap_set.h>
#include <rubik.h>
#include <text_output.h>

template< cube_size N >
class Engine
{
  static constexpr size_t StackSize = N * N * 10;
  static constexpr size_t INSIGHT_BOUND = 40;

  BitMapID     m_progress;
  BitMapID     m_allowed  [ CRotations<N>::AllRotIDs ] = {};
  DistID       m_depth;
  DistID       m_maxDepth;

  BitMap32ID   m_targetStack[ StackSize ] = {};
  BitMap32ID * m_target;

  BitMap       m_gradientStack[ StackSize ];
  BitMap     * m_gradient;

  CubeID       m_transposeSolution;
  RotID        m_solutionStack[ StackSize ] = {};
  RotID      * m_solution;

  Insight<N>   m_insights [ INSIGHT_BOUND ];
  Insight<N> * m_endOfInsights;

  // initializers
  void init();
  void toSolve( const Rubik<N> & );
  DistID approx( const BitMapID );

  // iteratively deepening algorithm IDA
  void startIDA();
  void progress();
  void back();
  bool iterativelyDeepening();

public:
  Engine();
  ~Engine();

  void addInsight ( const PosID *, const size_t, const CubeID trans = 0, AcceptFunction af = Accept<N>::Normal );

  void  update   ( void );
  bool  empty    ( void ) const { return m_insights == m_endOfInsights; }
 
  Sequence searchPath( Rubik<N> & );

  void close();
  bool closed() const;
};


template< cube_size N >
Engine<N>::Engine()
 :  m_progress( 0 )
 ,  m_target   ( m_targetStack   )
 ,  m_gradient ( m_gradientStack )
 ,  m_solution ( m_solutionStack )
 ,  m_endOfInsights ( nullptr )
{
  *m_target = ( 1 << 24 ) - 1;
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
DistID Engine<N>::approx( const BitMapID insight )
{/// FixMe
  DistID result = 0;
  const BitMapID orig = m_progress;
  m_progress |= insight;

  for ( m_depth = 0; 0 == result && m_depth < 12; ++ m_depth )
  {
    startIDA();
    if ( ! m_gradient -> empty() )
    {
      result = m_depth;
    }
  }

  m_progress = orig;
  m_depth = 0;
  return result;
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
    else if ( dist < min) // FixMe
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
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> update();
  }  
}

template< cube_size N >
void Engine<N>::back()
{
  ++ m_depth;
  -- m_solution;

  -- m_target;
  -- m_gradient;

  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> back();
  }
}

template< cube_size N >
void Engine<N>::close()
{
  m_progress = ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
}

template< cube_size N >
bool Engine<N>::closed() const
{
  return m_progress == ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
}

template< cube_size N >
Engine<N>::~Engine()
{
}

// template functions including
#include <engine_search.h>

#endif  //  ! ENGINE__H_INCLUDED
