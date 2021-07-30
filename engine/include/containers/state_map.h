#ifndef STAE_MAP__H
#define STAE_MAP__H

#include<raw_map.h>
#include<state_printer.h>

static constexpr unsigned int MAX_NUMBER_OF_PATCHES = 10;

template< cube_size N >
class StateMap
{
  RawStateMap<N> m_mapPatches[ MAX_NUMBER_OF_PATCHES ];
  unsigned int   m_patches = 0;
  
  void addPatches ( const PosID *, const size_t );
  
public:
  StateMap() = default;
  
  void build ( const PosID *, const size_t );
  void add   ( const RawStateMap<N> & rwsm );
  void move  ( const RotID rotID );
  Cache64ID state() const;
  Cache64ID projectedState() const;
  
  void print( const bool details = false ) const;
};

template< cube_size N >
void StateMap<N>::build( const PosID * pos, const size_t size )
{
  m_patches = 0;
  addPatches( pos, size );
}

template< cube_size N >
void StateMap<N>::addPatches( const PosID * pos, const size_t size )
{
  if ( size == 0 )
  {
    return;
  }
  
  size_t part;
  if ( size > 5 )
  {
    part = 4;
  }
  if ( size == 5 )
  {
    part = 3;
  } 
  if ( size < 5 )
  {
    part = size;
  }
  
  m_mapPatches[ m_patches ++ ].build( pos, part );
  addPatches( pos + part, size - part );
}

template< cube_size N >
void StateMap<N>::add( const RawStateMap<N> & rwsm )
{
  m_mapPatches[ m_patches ++ ] = rwsm;
}

template< cube_size N >
void StateMap<N>::move( const RotID rotID )
{
  for( unsigned int next = 0; next < m_patches; ++ next )
    m_mapPatches[ next ].move( rotID );
}

template< cube_size N >
Cache64ID StateMap<N>::state() const
{
  CacheID result = 0;
  for( int next = m_patches - 1; 0 <= next; -- next )
  {
    result *= pow24( m_mapPatches[ next ].size() );
    result += m_mapPatches[ next ].state();
  }
  return result; 
}

template< cube_size N >
Cache64ID StateMap<N>::projectedState() const
{
  const CubeID prior = m_mapPatches[0].prior();
  CacheID result = 0;
  for( int next = m_patches - 1; 0 <= next; -- next )
  {
    result *= pow24( m_mapPatches[ next ].size() );
    result += m_mapPatches[ next ].projection( prior );
  }
  return result; 
}

template< cube_size N >
void StateMap<N>::print( const bool details ) const
{
  const CacheID stateID = state();
  PosID pos[ CPositions<N>::GetSize() ];
  size_t size = 0;
  
  // merge raw map patches into a whole
  for( size_t index = 0, next = 0; next < m_patches; ++ next )
  {
    size += m_mapPatches[ next ].size();
    for( int pIndex = 0; pIndex < m_mapPatches[ next ].size(); ++ pIndex )
    {
      pos[ index ++ ] = m_mapPatches[ next ].getPosID( pIndex );
    }
  }
  
  // show map
  PrintMap<N>( stateID, pos, size, details);
}

#endif  //  ! STAE_MAP__H
