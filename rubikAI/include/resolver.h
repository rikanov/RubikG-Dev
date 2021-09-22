#ifndef RESOLVER__H
#define RESOLVER__H

#include <engine.h>
#include <sequence.h>

template< cube_size N >
class Resolver
{
  Rubik <N> & m_cubeToSolve;
  Engine<N>   m_engine;
  Sequence    m_path;
  DistID      m_searchDepth;

  void task();

public:
  Resolver( Rubik<N> & );
  void addInsight( const PosID *, const size_t, const CubeID trans = 0 );

  void fixCube()
  {
    m_engine.fixCube();
  }

  // iteratively deepening algorithm ( IDA )
  void  solve ( const bool closed = true, const bool show = true );
  bool  ida   ( const BitMapID, const DistID );

};

template< cube_size N >
Resolver<N>::Resolver( Rubik<N> & cube )
  :  m_cubeToSolve( cube )
  ,  m_engine()
  ,  m_path()
  ,  m_searchDepth( 0 )
{
  
}

template< cube_size N >
void Resolver<N>::solve( const bool closed, const bool show )
{
  if ( show )
  {
    m_cubeToSolve.print();
  }
  
  if ( m_engine.empty() )
  {
    return;
  }

  if ( closed )
  {
    m_engine.close();
  }

  m_engine.update();

  do
  {
    task();
    if ( show )
    {
      CRotations<N>::PrintSeq( m_path.reverse() );
      m_cubeToSolve.print();
      NL();
    }
  }
  while ( ! m_engine.closed() );

}

template< cube_size N >
void Resolver<N>::task()
{
  m_engine.toSolve( m_cubeToSolve );
  m_path.reset();
  for ( DistID depth = 0; depth < 15; ++ depth )
  {
    m_searchDepth = depth;
    const BitMap32ID aim = m_engine.target();
    BitMapID gradient = m_engine.gradient( depth );
    if ( 0 != gradient && ida( gradient, depth ) )
    {
      break;
    }
    m_engine.target( aim );
  }
  m_cubeToSolve.rotate( m_path.reverse() );
}

template< cube_size N >
bool Resolver<N>::ida( const BitMapID suggestedMoves, const DistID depth )
{
  if ( 0 == depth )
  {
    return ( suggestedMoves & 1 ) && m_engine.unambiguous();
  }

  bool result = false;

  BitMap moves ( suggestedMoves );
  for ( RotID next; moves >> next; )
  {
    m_engine.move( next );
    const BitMap32ID aim = m_engine.target();
    const BitMapID gradient = m_engine.progress( next, depth - 1 );
    if ( 0 < gradient && ida( gradient, depth - 1 ) )
    {
      m_path << next;
      result = true;
      break;
    }
    m_engine.target( aim );
    m_engine.move( CRotations<N>::GetInvRotID( next ) ); // revert
  }
  return result;
}

template< cube_size N >
void Resolver<N>::addInsight(const PosID * startPos, const size_t size, const CubeID trans )
{
  m_engine.addInsight( startPos, size, trans );
}

#endif  // ! RESOLVER__H
