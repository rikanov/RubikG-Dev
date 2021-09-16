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
  DistID      m_depth;

public:
  RubikAI( Rubik<N> & );
  void insight( const PosID *,const size_t );

  // iteratively deepening algorithm ( IDA )
  Sequence solve();
  bool ida( BitMapID );

};

template< cube_size N >
RubikAI<N>::RubikAI( Rubik<N> & cube )
  :  m_toSolve( cube )
  ,  m_engine()
  ,  m_path()
  ,  m_depth( 0 )
{
  
}

template< cube_size N >
Sequence RubikAI<N>::solve()
{
  m_path.reset();
  m_engine.toSolve( m_toSolve );
  for ( m_depth = 0; m_depth < 15; ++ m_depth )
  {
    BitMapID gradient = m_engine.gradient( m_depth );
    if ( 0 == gradient )
      continue;
    if ( ida( gradient ) )
      break;
  }
  return m_path;
}

template< cube_size N >
bool RubikAI<N>::ida( BitMapID suggestedMoves )
{
  if ( 1 == suggestedMoves )
  {
    return true;
  }
  if ( 0 == m_depth )
  {
    return false;
  }
  -- m_depth;
  BitMap moves ( suggestedMoves );
  for ( RotID next; moves >> next; )
  {
    BitMapID gradient = m_engine.progress( next, m_depth );
    if ( 0 == gradient )
    {
      continue;
    }
    if ( ida( gradient ) )
    {
      m_path << next;
      break;
    }
    m_engine.move( CRotations<N>::GetInvRotID( next ) ); // revert
  }
  ++ m_depth;
  return moves.isEmpty() == false;
}

template< cube_size N >
void RubikAI<N>::insight( const PosID * startPos, const size_t size )
{
}

#endif  // ! RUBIK_AI__H
