#ifndef CACHEID_MAP__H
#define CACHEID_MAP__H


#include <initializer_list>
#include <base_types.h>
#include <qeueu.h>

template< cube_size N >
class CacheIDmap
{
  using _crot = CExtRotations<N>;

  CacheID  * m_map;
  DistID   * m_dist;
  BitMapID * m_suggestion;

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

  int distance( CacheID cacheID ) const
  {
    return m_dist[ cacheID ];
  }
  unsigned long router( const CacheID cacheID ) const
  {
    return m_suggestion[ cacheID ];
  }
};

template< cube_size N > CacheIDmap<N>::CacheIDmap()
  :  m_map  ( nullptr )
  ,  m_dist ( nullptr )
  ,  m_suggestion ( nullptr )
{
}

template< cube_size N >
void CacheIDmap<N>::init( const size_t size )
{
  clean();
  m_map  =       new CacheID  [ pow24( size - 1 ) * _crot::AllRotIDs ];
  m_dist =       new DistID   [ pow24( size - 1 ) ] {};
  m_suggestion = new BitMapID [ pow24( size - 1 ) ] {};

}

template< cube_size N >
void CacheIDmap<N>::connect( const CacheID from, const Axis axis, const Layer layer, const Turn turn, const CacheID to, const bool first )
{
  if ( first )
  {
    m_dist[ to ] = m_dist[ from ] + 1;
  }

  if ( m_dist[ from ] < m_dist[ to ] )
  {
    m_suggestion[to] |= ( 1 << _crot::GetInvRotID( axis, layer, turn ) );
  }
  m_map[ to * _crot::AllRotIDs + _crot::GetInvRotID( axis, layer, turn ) ] = from;
}

template< cube_size N >
void CacheIDmap<N>::clean()
{
  delete[] m_map;
  delete[] m_dist;
  delete[] m_suggestion;
  m_map  = nullptr;
  m_dist = nullptr;
  m_suggestion = nullptr;
}

template< cube_size N >
CacheIDmap<N>::~CacheIDmap()
{
  clean();
}

#endif  //  ! CACHEID_MAP__H
