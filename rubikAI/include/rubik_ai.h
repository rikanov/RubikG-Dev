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
  void addInsight( const PosID *, const size_t, const CubeID trans = 0 );

  void fixCube()
  {
    m_engine.fixCube();
  }

  // iteratively deepening algorithm ( IDA )
  Sequence solution( const bool applySolution = true );
  bool     ida     ( const BitMapID, const DistID );

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
Sequence RubikAI<N>::solution( const bool applySolution )
{
  if ( m_engine.empty() )
  {
    return Sequence();
  }
  m_engine.update();
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
  
  if ( applySolution  )
  {
    m_cubeToSolve.rotate( m_path.reverse() );
  }

  return m_path.reverse();
}

template< cube_size N >
bool RubikAI<N>::ida( const BitMapID suggestedMoves, const DistID depth )
{
  if ( 0 == depth )
  {
    return ( suggestedMoves & 1 ) && m_engine.unambiguous();
  }

  bool result = false;

  BitMap moves ( suggestedMoves );
  for ( RotID next; moves >> next; )
  {
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
void RubikAI<N>::addInsight(const PosID * startPos, const size_t size, const CubeID trans )
{
  m_engine.addInsight( startPos, size, trans );
}

#endif  // ! RUBIK_AI__H
