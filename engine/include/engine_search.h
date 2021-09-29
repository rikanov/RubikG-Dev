#ifndef ENGINE_SEARCH__H
#define ENGINE_SEARCH__H

#include <engine.h>

template< cube_size N >
void Engine<N>::progress()
{
  -- m_depth;
  for ( RotID next = 0; m_gradient -> next( next ); )
  {
    BitMap32ID target   = ( 1 << 24 ) - 1;
    BitMapID   gradient = m_allowed[ next ];

    BitMapID step = 1;
    Insight<N> * pInsight = m_insights;

    while (  gradient > 0 && target > 0 && pInsight != m_endOfInsights )
    {
      if ( m_progress & step )
      {
        pInsight -> move( next );

        target   &= pInsight -> aim     ( m_depth );
        gradient &= pInsight -> gradient( m_depth );
      }
      ++ pInsight;
      step <<= 1;
    }

    if ( gradient > 0 && target > 0 )
    {
      *( m_solution ++ ) = next;
      *( ++ m_target ) = target;
      ( ++ m_gradient ) -> set( gradient );
      return;
    }

    while ( m_insights != pInsight )
    {
      ( -- pInsight ) -> back();
    }
  }
  ++ m_depth;
}

template< cube_size N >
void Engine<N>::startIDA()
{
  *( m_target + 1 ) = ( 1 << 24 ) - 1;
  ++ m_target;

  BitMapID gradient = m_allowed[ 0 ];

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
void Engine<N>::finishIDA()
{
  -- m_target;
  -- m_gradient;
}

template< cube_size N >
Sequence Engine<N>::searchPath( Rubik<N> & cube )
{
  Sequence result;
  toSolve( cube );
  m_solution = m_solutionStack;
  for ( m_maxDepth = 0; m_maxDepth < 12; ++ m_maxDepth )
  {clog( "max depth:", (int) m_maxDepth );
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
  startIDA();

  RotID next = 0; 
  while ( m_depth < m_maxDepth || ! m_gradient -> empty() )
  {
    if ( 0 == m_depth && m_gradient -> next() == 0 )
    {
        result = true;
        break; // no more state to resolve
    }
    while ( m_depth < m_maxDepth && m_gradient -> empty() )
    {
      back();
    }
    progress();
  }

  finishIDA();
  return result;
}

#endif  //  ! ENGINE_SEARCH__H
