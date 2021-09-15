#ifndef RUBIK_AI__H
#define RUBIK_AI__H

#include <engine.h>
#include <sequence.h>

template< cube_size N >
class RubikAI
{
  Rubik <N> & m_toSolve;
  Engine<N>   m_engine;
  Sequence    m_path;

public:
  RubikAI( Rubik<N> & );

  // iteratively deepening algorithm ( IDA )
  Sequence solve();

};

template< cube_size N >
RubikAI<N>::RubikAI( Rubik<N> & cube )
  :  m_toSolve( cube )
  ,  m_engine()
  ,  m_path()
{
  
}

template< cube_size N >
Sequence RubikAI<N>::solve()
{
  m_path.reset();
  m_engine.toSolve( m_toSolve );
  return m_path;
}

#endif  // ! RUBIK_AI__H
