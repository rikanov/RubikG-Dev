
template< cube_size N >
BitMapID Engine<N>::progress( const RotID rotID, const DistID distance )
{
  BitMapID gradient = m_allowed[ rotID ];
  BitMapID step = 1;

  for ( const Insight<N> * P = m_insights; gradient > 0 && m_target > 0 && P != m_endOfInsights; ++ P )
  {
    if ( m_progress & step )
    {
      gradient &= P -> gradient( distance );
      m_target &= P -> aim( distance );
    }
    step <<= 1;
  }

  if ( 0 == distance && ( gradient & 1 ) )
  {
    return gradient;
  }
  
  return m_target ? gradient : 0; 
}

template< cube_size N >
Sequence Engine<N>::searchPath( Rubik<N> & cube )
{
  m_solutionPath.reset();
  toSolve( cube );
  for ( DistID depth = 0; depth < 12; ++ depth )
  {
    const BitMap32ID aim = m_target;
    BitMapID gradient = progress( 0, depth );
    if ( 0 != gradient && ida( gradient, depth ) )
    {
      break;
    }
    m_target = aim;
  }
  return m_solutionPath.reverse();
}

template< cube_size N >
bool Engine<N>::ida( const BitMapID suggestedMoves, const DistID depth )
{
  if ( 0 == depth )
  {
    return ( suggestedMoves & 1 ) && unambiguous();
  }

  bool result = false;

  BitMap moves ( suggestedMoves );
  for ( RotID next; moves >> next; )
  {
    move( next );
    const BitMap32ID aim = m_target;
    const BitMapID gradient = progress( next, depth - 1 );
    if ( 0 < gradient && ida( gradient, depth - 1 ) )
    {
      m_solutionPath << next;
      result = true;
      break;
    }
    m_target = aim;
    move( CRotations<N>::GetInvRotID( next ) ); // revert
  }
  return result;
}
