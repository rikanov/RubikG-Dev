#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED

#include <insight.h>

template< cube_size N >
class Engine
{
  static constexpr size_t s_Size = CPositions<N>::GetSize();
  Insight <N> ** m_insights;
  Insight <N> ** m_lastInsight;
  Insight <N> *  m_highest;
  Rubik   <N> &  m_rubik;

  bool rotate( const RotID rotID )
  {
    for ( Insight<N> * p = m_insights; p < m_lastInsight; ++ p )
      p->rotate( rotID );
  }

  size_t weight() const
  {
    size_t result = 0;
    for ( Insight<N> * p = m_insights; p < m_lastInsight; ++ p )
    {
      result += p -> weight();
    }
    return result;
  }

  size_t seek();

public:
  Engine( Rubik<N> & );

  void operator << ( Insight<N> & );
  void operator << ( Insight<N> * );

  void exec();
};

template< cube_size N > 
Engine<N>::Engine( Rubik <N> & rubik )
  : m_insights( new Insight<N> * [ s_Size ]{} )
  , m_lastInsight( m_insights )
  , m_rubik( rubik )
{
}

template< cube_size N >
void Engine<N>::operator << ( Insight<N> & next )
{
  next.set( m_rubik );
  *( m_lastInsight ++ ) = &next;
}

template< cube_size N >
void Engine<N>::operator << ( Insight<N> * next )
{
  next -> set( m_rubik );
  *( m_lastInsight ++ ) = next;
}

template< cube_size N >
void Engine<N>::exec()
{
  m_highest = *m_insights;
  for ( Insight<N> * p = m_insights + 1; p < m_lastInsight; ++ p )
  {
    if ( p -> distance() > m_highest -> distance() )
      m_highest = p;
  }
  seek();
}

template< cube_size N >
size_t Engine<N>::seek()
{
  size_t result = m_highest -> weight();
  for( const RotID * rot = m_highest -> router(); w > 0 && m_highest -> gradient( *rot ) > 0 ; ++ rot  )
  {
    rotate( rot );
    const size_t wBranch = ( m_highest -> distance() > 0 ) ? seek() : weight() ;
    
    if ( result > wBranch )
    {
      result = wBranch;
    }
    
    if ( result == 0 )
    {
      break;
    }

    rotate(  );
  }
  return result;
}

#endif  //  ! ENGINE__H_INCLUDED
