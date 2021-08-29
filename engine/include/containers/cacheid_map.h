#ifndef CACHEID_MAP__H
#define CACHEID_MAP__H


#include <initializer_list>
#include <base_types.h>
#include <qeueu.h>

template< cube_size N >
class GroupIDmap
{
  using _crot = CExtRotations<N>;

  GroupID  * m_map;
  DistID   * m_dist;
  BitMapID * m_suggestion;

  void clean();

public:
  GroupIDmap();
  GroupIDmap( const GroupIDmap & ) = delete;
  ~GroupIDmap();

  void init ( const size_t size );
  void connect( const GroupID start, const Axis axis, const Layer layer, const Turn turn, const GroupID result, const bool first );

  GroupID getState( GroupID cacheID, Axis axis, Layer layer, Turn turn ) const
  {
    return m_map[ _crot::GetRotID( axis, layer, turn) + _crot::AllRotIDs * cacheID ];
  }

  GroupID getState( GroupID cacheID, RotID rotID ) const
  {
    return m_map[ rotID + _crot::AllRotIDs * cacheID ];
  }

  int distance( GroupID cacheID ) const
  {
    return m_dist[ cacheID ];
  }
  unsigned long router( const GroupID cacheID ) const
  {
    return m_suggestion[ cacheID ];
  }
};

template< cube_size N > GroupIDmap<N>::GroupIDmap()
  :  m_map  ( nullptr )
  ,  m_dist ( nullptr )
  ,  m_suggestion ( nullptr )
{
}

template< cube_size N >
void GroupIDmap<N>::init( const size_t size )
{
  clean();
  m_map  =       new GroupID  [ pow24( size - 1 ) * _crot::AllRotIDs ];
  m_dist =       new DistID   [ pow24( size - 1 ) ] {};
  m_suggestion = new BitMapID [ pow24( size - 1 ) ] {};

}

template< cube_size N >
void GroupIDmap<N>::connect( const GroupID from, const Axis axis, const Layer layer, const Turn turn, const GroupID to, const bool first )
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
void GroupIDmap<N>::clean()
{
  delete[] m_map;
  delete[] m_dist;
  delete[] m_suggestion;
  m_map  = nullptr;
  m_dist = nullptr;
  m_suggestion = nullptr;
}

template< cube_size N >
GroupIDmap<N>::~GroupIDmap()
{
  clean();
}

#endif  //  ! CACHEID_MAP__H
