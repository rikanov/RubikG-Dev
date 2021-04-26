#ifndef ENGINE__H_INCLUDED
#define ENGINE__H_INCLUDED

#include <insight.h>

template< size_t N >
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
    {
      p->rotate( rotID );
      if ( p->distance() > m_highest->distance() )
      {
        return false;
      }
    }
    return true;
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

  void run( Insight<N> * );

public:
  Engine( Rubik<N> & );

  void operator << ( Insight<N> & );
  void operator << ( Insight<N> * );

  void exec();
};

template<size_t N> Engine<N>::Engine( Rubik <N> & rubik )
  : m_insights( new Insight<N> * [ s_Size ]{} )
  , m_lastInsight( m_insights )
  , m_rubik( rubik )
{
}

template< size_t N >
void Engine<N>::operator << ( Insight<N> & next )
{
  *( m_lastInsight ++ ) = &next;
}

template< size_t N >
void Engine<N>::operator << ( Insight<N> * next )
{
  *( m_lastInsight ++ ) = next;
}

template< size_t N >
void Engine<N>::exec()
{
  Insight<N> * highest = *m_insights;
  for ( Insight<N> * p = m_insights + 1; p < m_lastInsight; ++ p )
  {
    if ( p -> distance() > highest -> distance() )
      highest = p;
  }
}

template< size_t N >
void Engine<N>::run( Insight<N> * highest )
{
  for( const RotID * rot = highest -> router(); highest->gradient( *rot ) > 0 ; ++ rot  )
  {
    if ( rotate( rot ) )
    {
      ;
    }
  }
}

#endif  //  ! ENGINE__H_INCLUDED
