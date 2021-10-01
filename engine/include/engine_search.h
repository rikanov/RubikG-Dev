#ifndef ENGINE_SEARCH__H
#define ENGINE_SEARCH__H

#include <engine.h>

template< cube_size N >
void Engine<N>::progress()
{
  for ( RotID next = 0; chooseBranch( next ); )
  {
    BitMap32ID target   = ( 1 << 24 ) - 1;
    BitMapID   gradient = m_allowed[ next ];

    Insight<N> * pInsight = m_insights;

    BitMapID active = 1;
    while (  gradient > 0 && target > 0 && pInsight != m_endOfInsights )
    {
      if ( m_progress & active )
      {
        pInsight -> move( next );

        target   &= pInsight -> aim     ( depth() - 1 );
        gradient &= pInsight -> gradient( depth() - 1 );
      }
      ++ pInsight;
      active <<= 1;
    }

    if ( gradient > 0 && target > 0 )
    {
      push( gradient, next );
      return;
    }

    active = 1;
    for ( auto revInsight = m_insights; revInsight != pInsight; ++ revInsight )
    {
      if ( m_progress & active )
        revInsight -> back();
      
      active <<= 1;
    }
  };
}

template< cube_size N >
void Engine<N>::startIDA()
{
  BitMap32ID target   = ( 1 << 24 ) - 1;
  BitMapID   gradient = m_allowed[ 0 ];

  BitMapID active = 1;
  for ( const Insight<N> * P = m_insights; gradient > 0 && target > 0 && P != m_endOfInsights; ++ P )
  {
    if ( m_progress & active )
    {
       gradient &= P -> gradient( depth() );
       target   &= P -> aim( depth() );
    }
    active <<= 1;
  }
  
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

#endif  //  ! ENGINE_SEARCH__H
