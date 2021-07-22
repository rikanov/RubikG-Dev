#ifndef RAW_MAP__H
#define RAW_MAP__H

#include <atomic>
#include <mutex>
#include <cube_positions.h>
#include <cube_rotations.h>
#include <rubik.h>

template< cube_size N >
class RawStateMap
{
  using _crot = CRotations<N>;
  
  std::mutex         m_deallocMutex;
  std::atomic<int> * m_refCount;
  
        size_t    m_size;
        CacheID   m_state;
  const CacheID * m_stateMap;
  const CacheID * m_transMap;
  const PosID   * m_startPos;
        CubeID  * m_subspace;
  
  void init();
  void nextState();
  void expandNodeTo    ( CacheID * ) const;
  void transformNodeTo ( CacheID * ) const;
  void createStateMap();
  void createTransMap(); 
  void refCopy( const RawStateMap<N> & );
  void dealloc();
  
public:
  RawStateMap( const PosID * P, const size_t size);
  RawStateMap( const RawStateMap<N> & RS );
  RawStateMap( RawStateMap<N> && RS );
  ~RawStateMap();
  
  const RawStateMap<N> & operator= ( const RawStateMap<N> & RS );
  
  void set( const Rubik<N> & R, const CubeID view = 0 );
  
  size_t size() const
  {
    return m_size;
  }
  
  CacheID state() const
  {
    return m_state;
  }
  
  CubeID prior() const
  {
    return m_state / pow24( m_size - 1 );
  }
  
  void move( const RotID rotID )
  {
    m_state = m_stateMap[ _crot::AllRotIDs * m_state + rotID ];
  }
  
  CacheID projection( const CubeID cid ) const
  {
    return m_transMap[ 24 * m_state + cid ];
  }
	
  void print( const bool details = false ) const;
};

  
template< cube_size N >
RawStateMap<N>::RawStateMap( const PosID * P, const size_t size)
: m_refCount ( new std::atomic<int> ( 1 ) )
, m_size     ( size )
, m_state    ( 0 )
, m_stateMap ( nullptr )
, m_transMap ( nullptr )
, m_subspace ( nullptr )
, m_startPos ( P )
{
  init();
}

template< cube_size N >
RawStateMap<N>::RawStateMap( const RawStateMap<N> & RS )
{
  refCopy( RS );
}

template< cube_size N >
RawStateMap<N>::RawStateMap( RawStateMap<N> && RS )
{
  refCopy( RS );
}

template< cube_size N >
const RawStateMap<N> & RawStateMap<N>::operator= ( const RawStateMap<N> & RS )
{
  if ( RS.m_refCount == m_refCount ) // same object
  {
    return * this;
  }
  
  if ( -- *m_refCount < 1 )
  {
    dealloc();
  }
  
  refCopy( RS );
  return * this;
}

template< cube_size N >
void RawStateMap<N>::set( const Rubik<N> & R, const CubeID view )
{ 
  m_state = 0;
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    m_subspace[ posIndex ] = Simplex::Composition( view, R.transpose( m_startPos[ posIndex ] ) );
    m_state += m_subspace[ posIndex ] * pow24( posIndex );
  }
}

template< cube_size N >
void RawStateMap<N>::refCopy( const RawStateMap<N> & RS )
{
  m_size     = RS.size();
  m_state    = RS.m_state; 
  
  m_refCount = RS.m_refCount;
  m_startPos = RS.m_startPos;
  m_stateMap = RS.m_stateMap;
  m_transMap = RS.m_transMap;
  m_subspace = RS.m_subspace;
  
  ++ *m_refCount;
}

template< cube_size N >
void RawStateMap<N>::init()
{
  m_subspace = new CubeID [ m_size + 1 ] {};
  createStateMap();
  createTransMap();
}

template< cube_size N >
void RawStateMap<N>::nextState()
{
  CubeID * next = m_subspace;
  while ( 23 == * next )
  {
    *( next ++ ) = 0;
  }
  * next = ( * next ) + 1;  
}

template< cube_size N >
void RawStateMap<N>::transformNodeTo( CacheID * transMap ) const
{
  all_cubeid ( id )
  {
    CacheID rotatedID = 0;
    for( int radix = 0; radix < m_size; ++ radix )
    {
      rotatedID += Simplex::Composition( m_subspace[ radix ], id ) * pow24( radix );
    }
    transMap[ 24 * m_state + id ] = rotatedID;
  }  
}

template< cube_size N >
void RawStateMap<N>::expandNodeTo( CacheID * stateMap ) const
{   
  RotID   rotID = 1;
  CacheID modified = m_state;
  all_rot( axis, layer, turn, N )
  {
    for( int radix = 0; radix < m_size; ++ radix )
    {
      const PosID current = CPositions<N>::GetPosID( m_startPos[radix], m_subspace[radix] );
      if ( CPositions<N>::GetLayer( current, axis ) == layer )
      {
	    const CubeID cid = Simplex::Composition( m_subspace[radix], Simplex::Tilt( axis, turn ) );
	    modified += ( cid - m_subspace[radix] ) * pow24( radix );
      }
    }
    stateMap[ _crot::AllRotIDs * m_state + rotID ] = modified;
    ++ rotID;
  }
}

template< cube_size N >
void RawStateMap<N>::createStateMap()
{
  CacheID * stateMap = new CacheID[ pow24( m_size ) * _crot::AllRotIDs ] {};
  for( m_state = 0; m_state < pow24( m_size ); ++ m_state )
  {
    expandNodeTo( stateMap );
    nextState();
  }  // exit criteria: m_state = 24 ^ size ==> m_subspace = { ++X, 0, 0, ... }
  m_stateMap = stateMap;
}

template< cube_size N >
void RawStateMap<N>::createTransMap()
{
  CacheID * transMap = new CacheID[ pow24( m_size ) * 24 ] {};
  for( m_state = 0; m_state < pow24( m_size ); ++ m_state )
  {
    transformNodeTo( transMap );
    nextState();
  }  // exit criteria: m_state = 24 ^ size ==> m_subspace = { ++X, 0, 0, ... }
  m_transMap = transMap;  
}

template< cube_size N >
void RawStateMap<N>::dealloc()
{
  m_deallocMutex.lock();
  delete   m_refCount; m_refCount = nullptr;
  delete[] m_stateMap; m_stateMap = nullptr;
  delete[] m_transMap; m_transMap = nullptr;
  delete[] m_subspace; m_subspace = nullptr;
  m_deallocMutex.unlock();
}

template< cube_size N >
RawStateMap<N>::~RawStateMap()
{
  if ( -- *m_refCount < 1 )
  {
    dealloc();
  }
}
  
#include <raw_map_printer.h>
#endif  //  ! RAW_MAP__H
