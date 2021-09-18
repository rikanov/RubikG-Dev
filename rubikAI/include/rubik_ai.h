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
  DistID      m_searchDepth;

public:
  RubikAI( Rubik<N> & );
  Insight<N> & addInsight( const PosID *, const size_t );
  Insight<N> & addInsight( const PosID *, const size_t, ObjID & );
  Insight<N> & setInsight( const PosID *, const size_t, const ObjID obj );

  // iteratively deepening algorithm ( IDA )
  Sequence solve( const bool applySolution = true );
  bool     ida  ( const BitMapID, const DistID );

};

template< cube_size N >
RubikAI<N>::RubikAI( Rubik<N> & cube )
  :  m_cubeToSolve( cube )
  ,  m_engine()
  ,  m_path()
  ,  m_searchDepth( 0 )
{
  
}

template< cube_size N >
Sequence RubikAI<N>::solve( const bool applySolution )
{
  m_engine.update();
  m_engine.toSolve( m_cubeToSolve );
  m_path.reset();

  for ( DistID depth = 0; depth < 15; ++ depth )
  {
    m_searchDepth = depth;
    BitMapID gradient = m_engine.gradient( depth );
    if ( 0 == gradient )
    {
      continue;
    }
    if ( ida( gradient, depth ) )
    {
      break;
    }
  }
  
  if ( applySolution  )
  {
    m_cubeToSolve.rotate( m_path.reverse() );
  }

  return m_path.reverse();
}

template< cube_size N >
bool RubikAI<N>::ida( const BitMapID suggestedMoves, const DistID depth )
{
  bool result = false;
  if ( suggestedMoves & 1 )
  {
    return true;
  }
  if ( 0 == depth )
  {
    return false;
  }

  BitMap moves ( suggestedMoves );
  for ( RotID next; moves >> next; )
  {
    const BitMapID gradient = m_engine.progress( next, depth );
    if ( 0 == gradient )
    {
      continue;
    }
    if ( ida( gradient, depth - 1 ) )
    {
      m_path << next;
      result = true;
      break;
    }
    m_engine.move( CRotations<N>::GetInvRotID( next ) ); // revert
  }
  return result;
}

template< cube_size N >
Insight<N> & RubikAI<N>::addInsight(const PosID * startPos, const size_t size )
{
  return m_engine.addInsight( startPos, size );
}

template< cube_size N >
Insight<N> & RubikAI<N>::addInsight(const PosID * startPos, const size_t size, ObjID & obj )
{
  return m_engine.addInsight( startPos, size, obj );
}

template< cube_size N >
Insight<N> & RubikAI<N>::setInsight( const PosID * startPos, const size_t size, const ObjID obj )
{
  return m_engine.setInsight( startPos, size, obj );
}

#endif  // ! RUBIK_AI__H
