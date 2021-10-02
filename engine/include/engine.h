#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED


#include <insight.h>
#include <progress.h>
#include <snapper.h>
#include <text_output.h>

template< cube_size N >
class Engine: protected Progression
{
  static constexpr size_t StackSize = N * N * 10;

  BitMapID   * m_allowed;
  Snapper<N> * m_snapshots;

  // initializers
  void init();
  void toSolve( const Rubik<N> & R ) { m_snapshots -> toSolve( R ); }

  // iteratively deepening algorithm IDA
  void startIDA();
  void progress();
  void back();
  bool iterativelyDeepening();

public:
  Engine();
  ~Engine();

  void addInsight ( const PosID *, const size_t, const CubeID trans = 0, AcceptFunction af = Accept<N>::Normal );

  bool  empty    ( void ) const { return m_snapshots -> empty(); }
 
  Sequence searchPath( Rubik<N> & );

  void close();
  bool closed() const;
};


template< cube_size N >
Engine<N>::Engine()
 :  m_allowed  ( new BitMapID   [ CRotations<N>::AllRotIDs ] )
 ,  m_snapshots( new Snapper<N>() )
{
  init();
}

template< cube_size N >
void Engine<N>::init()
{
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
void Engine<N>::addInsight( const PosID * subspacePos, const size_t subspaceSize, const CubeID trans, AcceptFunction af )
{
  m_snapshots -> addInsight( subspacePos, subspaceSize, trans, af );
}

template< cube_size N >
void Engine<N>::back()
{
  pop();
  m_snapshots -> revert();
}

template< cube_size N >
void Engine<N>::close()
{
  m_snapshots -> close();
}

template< cube_size N >
bool Engine<N>::closed() const
{
  return m_snapshots -> closed();
}

template< cube_size N >
void Engine<N>::progress()
{
  for ( RotID next = 0; chooseBranch( next ); )
  {
    BitMap32ID target   = ( 1 << 24 ) - 1;
    BitMapID   gradient = m_allowed[ next ];

    const Insight<N> * end = m_snapshots -> snap( next, depth() - 1, gradient, target );

    if ( gradient > 0 && target > 0 )
    {
      push( gradient, next );
      return;
    }
    else
    {
      m_snapshots -> revertInterrupted( end );
    }
  };
}

template< cube_size N >
void Engine<N>::startIDA()
{
  reset();
  BitMap32ID target   = ( 1 << 24 ) - 1;
  BitMapID   gradient = m_allowed[ 0 ];

  m_snapshots -> snap( depth(), gradient, target );

  setNode( 0 == target ? 0: gradient );
}

template< cube_size N >
Sequence Engine<N>::searchPath( Rubik<N> & cube )
{
  Sequence result;
  toSolve( cube );
  startSearch();
  while ( heightLessThan( 13 ) )
  {
    if ( iterativelyDeepening() )
    {
      solution( result );
      break;
    }
    deepening();
  }
  NL();
  return result;
}

template< cube_size N >
bool Engine<N>::iterativelyDeepening()
{
  bool result = false;
  startIDA();
  while ( ! onTheTop() || ! onEmptyNode() )
  {
    if ( inSolvedState() )
    {
        result = true;
        break; // no more state to resolve
    }
    while ( ! onTheTop() && onEmptyNode() )
    {
      back();
    }
    progress();
  }
  return result;
}

template< cube_size N >
Engine<N>::~Engine()
{
  delete[] m_allowed;
  delete   m_snapshots;
}

#endif  //  ! ENGINE__H_INCLUDED
