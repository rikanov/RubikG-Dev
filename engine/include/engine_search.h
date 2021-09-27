#ifndef ENGINE_SEARCH__H
#define ENGINE_SEARCH__H

#include <engine.h>


template< cube_size N >
void Engine<N>::revert()
{
  -- m_target;
  -- m_gradient;
}

template< cube_size N >
void Engine<N>::progress( const RotID rotID )
{
  *( m_target + 1 ) = *m_target;
  ++ m_target;

  BitMapID gradient = m_allowed[ rotID ];

  BitMapID step = 1;
  for ( const Insight<N> * P = m_insights; gradient > 0 && *m_target > 0 && P != m_endOfInsights; ++ P )
  {
    if ( m_progress & step )
    {
       gradient &= P -> gradient( m_depth );
      *m_target &= P -> aim( m_depth );
    }
    step <<= 1;
  }
  ( ++ m_gradient ) -> set( 0 == *m_target ? 0: gradient );
}

template< cube_size N >
Sequence Engine<N>::searchPath( Rubik<N> & cube )
{
  Sequence result;
  toSolve( cube );
  m_solution = m_solutionStack;
  for ( m_maxDepth = 1; m_maxDepth < 12; ++ m_maxDepth )
  {
    if ( iterativelyDeepening() )
    {
      result.set( m_solutionStack, m_maxDepth );
      break;
    }
  }
  return result;
}

template< cube_size N >
bool Engine<N>::iterativelyDeepening()
{
  bool result = false;
  m_depth = m_maxDepth;
  progress( 0 );

  while ( m_depth < m_maxDepth || ! m_gradient -> empty() )
  {
    if ( 0 == m_depth && solved() )
    {
      result = true;
      break;
    }
    if ( 0 == m_depth )
    {
      back();
      continue;
    }
    for ( RotID next = 0; m_gradient -> next( next ); )
    {
      move ( next );
      if ( ! m_gradient -> empty() )
      {
        break;
      }
      back();
    }
    if ( m_depth < m_maxDepth && m_gradient -> empty() )
    {
      back();
    }
  }
  revert();
  return result;
}

#endif  //  ! ENGINE_SEARCH__H
