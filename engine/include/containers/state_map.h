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
  CacheID state() const;
  CacheID projectedState() const;
  
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
  if ( size > 5 )
  {
    m_mapPatches[ m_patches ++ ].build( pos, 4 );
    addPatches( pos + 4, size - 4 );
  }
  if ( size == 5 )
  {
    m_mapPatches[ m_patches ++ ].build( pos, 3 );
    addPatches( pos + 3, 2 );
  } 
  if ( size < 5 )
  {
    m_mapPatches[ m_patches ++ ].build( pos, size );
  }
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
CacheID StateMap<N>::state() const
{
  CacheID result = 0;
  for( unsigned int next = 0; next < m_patches; ++ next )
  {
    result += m_mapPatches[ next ].state();
    result *= pow24( m_mapPatches[ next + 1 ].size() );
  }
  return result; 
}

template< cube_size N >
CacheID StateMap<N>::projectedState() const
{
  CacheID result = 0;
  const CubeID prior = m_mapPatches[0].prior();
  for( unsigned int next = 0; next < m_patches; ++ next )
  {
    result += m_mapPatches[ next ].projection( prior );
    result *= pow24( m_mapPatches[ next + 1 ].size() );
  }
  return result; 
}

template< cube_size N >
void StateMap<N>::print( const bool details ) const
{
  const CacheID stateID = state();
  PosID pos[ CPositions<N>::GetSize() ];
  size_t size = 0;
  for( size_t index = 0, next = 0; next < m_patches; ++ next )
  {
    size += m_mapPatches[ next ].size();
    for( int pIndex = 0; pIndex < m_mapPatches[ next ].size(); ++ next )
    {
      pos[ index ++ ] = m_mapPatches[ next ].getPosID( pIndex );
    }
  }
  PrintMap<N>( stateID, pos, size, details);
}

#endif  //  ! STAE_MAP__H
