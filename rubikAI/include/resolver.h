#ifndef RESOLVER__H
#define RESOLVER__H

#include <engine.h>
#include <sequence.h>

template< cube_size N >
class Resolver
{
  Rubik <N> & m_cubeToSolve;
  Engine<N>   m_engine;

public:
  Resolver( Rubik<N> & );
  void addInsight( const PosID *, const size_t, const CubeID trans = 0, AcceptFunction af = Accept<N>::Normal  );

  // iteratively deepening algorithm ( IDA )
  void  solve ( const bool closed = true, const bool show = true );
};

template< cube_size N >
Resolver<N>::Resolver( Rubik<N> & cube )
  :  m_cubeToSolve( cube )
  ,  m_engine()
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
    const Sequence path = m_engine.searchPath( m_cubeToSolve );
    m_cubeToSolve.rotate( path );
    if ( show )
    {
      CRotations<N>::PrintSeq( path );
      m_cubeToSolve.print();
      NL();
    }
  }
  while ( ! m_engine.closed() );

}

template< cube_size N >
void Resolver<N>::addInsight(const PosID * startPos, const size_t size, const CubeID trans, AcceptFunction af )
{
  m_engine.addInsight( startPos, size, trans, af );
}

#endif  // ! RESOLVER__H
