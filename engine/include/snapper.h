#ifndef SNAPPER__H
#define SNAPPER__H

#include <insight.h>
#include <rubik.h>

template< cube_size N >
class Snapper
{
  static constexpr size_t InsightsBound = 40;

  BitMapID     m_progress;
  CubeID       m_transposeSolution;
  Insight<N> * m_insights;
  Insight<N> * m_endOfInsights;

public:
  Snapper();
  ~Snapper();

  void addInsight ( const PosID *, const size_t, const CubeID, AcceptFunction );
  void toSolve    ( const Rubik<N> & );
  void revert     ( void );
  void revertInterrupted ( const Insight<N>* );
  void close      ( void );
  bool closed     ( void ) const;
  bool empty      ( void ) const;

  void               snap( const DistID, BitMapID &, BitMap32ID & ) const;
  const Insight<N> * snap( const RotID, const DistID, BitMapID &, BitMap32ID & );
};

template< cube_size N >
Snapper<N>::Snapper()
 :  m_progress( 0 )
 ,  m_insights      ( new Insight <N> [ InsightsBound ] )
 ,  m_endOfInsights ( m_insights )
{
}

template< cube_size N >
void Snapper<N>::addInsight( const PosID * subspacePos, const size_t subspaceSize, const CubeID trans, AcceptFunction af )
{
   m_endOfInsights -> init( subspacePos, subspaceSize, trans, af );
   m_endOfInsights -> build();
   ++ m_endOfInsights;
}

template< cube_size N >
void Snapper<N>::toSolve( const Rubik<N> & R )
{
  m_transposeSolution = 0;
  DistID   min   = 0xFF;
  BitMapID next  = 1;
  BitMapID stage = 1;
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    pInsight -> toSolve( R, m_transposeSolution, pInsight == m_insights );
    const DistID dist = pInsight -> distance();
    if ( 0 == dist && pInsight -> prior() == m_insights -> prior() )
    {
      m_progress |= 1ULL << ( pInsight - m_insights );
    }
    else if ( dist < min) // FixMe
    {
      min = dist;
      stage = next;
    }
    next <<= 1;
  }
  m_progress |= stage;
  BitMap::Print( m_progress, m_endOfInsights - m_insights );
}

template< cube_size N >
void Snapper<N>::snap( const DistID depth, BitMapID & gradient, BitMap32ID & target ) const
{
  BitMapID active = 1;
  for ( Insight<N> * pInsight = m_insights; gradient > 0 && target > 0 && pInsight != m_endOfInsights; ++ pInsight )
  {
    if ( m_progress & active )
    {
      target   &= pInsight -> aim     ( depth );
      gradient &= pInsight -> gradient( depth );
    }
    active <<= 1;
  }
}

template< cube_size N >
const Insight<N> * Snapper<N>::snap( const RotID next, const DistID depth, BitMapID & gradient, BitMap32ID & target )
{
  BitMapID active = 1;
  Insight<N> * pInsight = m_insights;
  while ( gradient > 0 && target > 0 && pInsight != m_endOfInsights )
  {
    if ( m_progress & active )
    {
      pInsight -> move( next );

      target   &= pInsight -> aim     ( depth );
      gradient &= pInsight -> gradient( depth );
    }
    ++ pInsight;
    active <<= 1;
  }
  return pInsight;
}

template< cube_size N >
void Snapper<N>::revert()
{
  BitMapID active = 1;
  for ( auto pInsight = m_insights; pInsight != m_endOfInsights; ++ pInsight )
  {
    if ( m_progress & active )
      pInsight -> back();

    active <<= 1;
  }
}

template< cube_size N >
void Snapper<N>::revertInterrupted( const Insight<N>* end )
{
  BitMapID active = 1;
  for ( auto revInsight = m_insights; revInsight != end; ++ revInsight )
  {
    if ( m_progress & active )
      revInsight -> back();

    active <<= 1;
  }
}

template< cube_size N >
void Snapper<N>::close()
{
  m_progress = ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
}

template< cube_size N >
bool Snapper<N>::closed() const
{
  return m_progress == ( 1ULL << ( m_endOfInsights - m_insights ) ) - 1;
}

template< cube_size N >
bool Snapper<N>::empty() const
{
  return m_insights == m_endOfInsights;
}

template< cube_size N >
Snapper<N>::~Snapper()
{
  delete[] m_insights;
}


#endif  //  ! SNAPPER__H
