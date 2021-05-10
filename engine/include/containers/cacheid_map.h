#ifndef CACHEID_MAP__H
#define CACHEID_MAP__H


#include <initializer_list>
#include <base_types.h>
#include <qeueu.h>

template< cube_size N >
class CacheIDmap
{
  using _crot = CExtRotations<N>;

  CacheID * m_map;
  DistID  * m_dist;

  RotID  * m_orderedSteps;
  DistID * m_complexity;

  void clean();

public:
  CacheIDmap();
  CacheIDmap( const CacheIDmap & ) = delete;
  ~CacheIDmap();

  void init ( const size_t size );
  void connect( const CacheID start, const Axis axis, const Layer layer, const Turn turn, const CacheID result, const bool first );

  CacheID getState( CacheID cacheID, Axis axis, Layer layer, Turn turn ) const
  {
    return m_map[ _crot::GetRotID( axis, layer, turn) + _crot::AllRotIDs * cacheID ];
  }

  CacheID getState( CacheID cacheID, RotID rotID ) const
  {
    return m_map[ rotID + _crot::AllRotIDs * cacheID ];
  }

  size_t complexity( const CacheID id ) const
  {
    return m_complexity[ id ];
  }

  const RotID * router( const CacheID cacheID ) const
  {
    return m_orderedSteps + cacheID * _crot::AllRotIDs;
  }

  int distance( CacheID cacheID ) const
  {
    return m_dist[ cacheID ];
  }

};

template< cube_size N > CacheIDmap<N>::CacheIDmap()
  :  m_map  ( nullptr )
  ,  m_dist ( nullptr )
  ,  m_orderedSteps ( nullptr )
  ,  m_complexity   ( nullptr )
{
}

template< cube_size N >
void CacheIDmap<N>::init( const size_t size )
{
  clean();
  m_map  = new CacheID [ _pow24[ size - 1 ] * _crot::AllRotIDs ];
  m_dist = new DistID  [ _pow24[ size - 1 ] ] {};

  m_orderedSteps = new RotID  [ _pow24[ size - 1 ] * _crot::AllRotIDs ]{};
  m_complexity   = new DistID [ _pow24[ size - 1 ] ] {};
}

template< cube_size N >
void CacheIDmap<N>::connect( const CacheID start, const Axis axis, const Layer layer, const Turn turn, const CacheID result, const bool first )
{
  if ( first )
  {
    m_dist[ result ] = m_dist[ start ] + 1;
  }

  if ( m_dist[ start ] < m_dist[ result ] )
  {
    m_orderedSteps[ result * _crot::AllRotIDs + m_complexity[ result ] ++ ] = _crot::GetInvRotID( axis, layer, turn );
  }
  m_map[ result * _crot::AllRotIDs + _crot::GetInvRotID( axis, layer, turn ) ] = start;
}

template< cube_size N >
void CacheIDmap<N>::clean()
{
  delete[] m_map;
  delete[] m_dist;
  delete[] m_orderedSteps;
  delete[] m_complexity;
  m_map  = nullptr;
  m_dist = nullptr;
  m_orderedSteps = nullptr;
  m_complexity   = nullptr;
}

template< cube_size N >
CacheIDmap<N>::~CacheIDmap()
{
  clean();
}

#endif  //  ! CACHEID_MAP__H
