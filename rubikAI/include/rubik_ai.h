#ifndef RUBIK_AI__H
#define RUBIK_AI__H

#include <engine.h>
#include <sequence.h>

template< cube_size N >
class RubikAI
{
  Rubik <N> & m_cubeToSolve;
  Engine<N>   m_engine;
  Sequence    m_path;
  DistID      m_depth;

public:
  RubikAI( Rubik<N> & );
  ObjID insight( const PosID *, const size_t, const ObjID obj );

  // iteratively deepening algorithm ( IDA )
  Sequence solve();
  bool ida( const BitMapID );

};

template< cube_size N >
RubikAI<N>::RubikAI( Rubik<N> & cube )
  :  m_cubeToSolve( cube )
  ,  m_engine()
  ,  m_path()
  ,  m_depth( 0 )
{
  
}

template< cube_size N >
Sequence RubikAI<N>::solve()
{
  m_path.reset();
  m_engine.toSolve( m_cubeToSolve );
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
bool RubikAI<N>::ida( const BitMapID suggestedMoves )
{
  bool result = false;
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
    const BitMapID gradient = m_engine.progress( next, m_depth );
    if ( 0 == gradient )
    {
      continue;
    }
    if ( ida( gradient ) )
    {
      m_path << next;
      result = true;
      break;
    }
    m_engine.move( CRotations<N>::GetInvRotID( next ) ); // revert
  }
  ++ m_depth;
  return result;
}

template< cube_size N >
ObjID RubikAI<N>::insight( const PosID * startPos, const size_t size, const ObjID obj )
{
  return m_engine.setInsight( startPos, size, obj );
}

#endif  // ! RUBIK_AI__H
