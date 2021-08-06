#ifndef RAW_MAP__H
#define RAW_MAP__H

#include <atomic>
#include <mutex>
#include <cube_positions.h>
#include <cube_rotations.h>
#include <rubik.h>
#include <state_printer.h>

template< cube_size N >
class RawStateMap
{
  using _crot = CRotations<N>;
  
  std::mutex         m_deallocMutex;
  std::atomic<int> * m_refCount;
  
          size_t    m_size;
  mutable CacheID   m_stateID;
  const   CacheID * m_stateMap;
  const   CacheID * m_transMap;
  const   PosID   * m_startPos;
  mutable CubeID  * m_subspace;
  
  void nextState();
  void expandNodeTo    ( CacheID * ) const;
  void transformNodeTo ( CacheID * ) const;
  void createStateMap();
  void createTransMap(); 
  void refCopy( const RawStateMap<N> & );
  void dealloc();
  
public:
  RawStateMap( );
  RawStateMap( const PosID * P, const size_t size);
  RawStateMap( const RawStateMap<N> & RS );
  RawStateMap( RawStateMap<N> && RS );
  ~RawStateMap();
  
  void build( const PosID *, const size_t );
  
  const RawStateMap<N> & operator= ( const RawStateMap<N> & RS );
  
  void set( const Rubik<N> & R, const CubeID view = 0 ) const;
  
  void set( const CacheID & state ) const
  {
    m_stateID = state;
  }
  
  bool isBuilt() const
  {
    return m_refCount != nullptr;
  }
  
  size_t size() const
  {
    return m_size;
  }
  
  PosID getPosID ( const size_t id ) const
  {
    return m_startPos[ id ];
  }
  
  CacheID state() const
  {
    return m_stateID;
  }
  
  CacheID state( const CubeID view ) const
  {
    return m_transMap[ 24 * m_stateID + view ];
  }
  
  CubeID prior() const
  {
    return m_stateID / pow24( m_size - 1 );
  }
  
  void move( const RotID rotID ) const
  {
    m_stateID = m_stateMap[ _crot::AllRotIDs * m_stateID + rotID ];
  }
  
  CacheID projection( const CubeID cid ) const
  {
    return m_transMap[ 24 * m_stateID + cid ];
  }
	
  void print( const bool details = false ) const
  {
    PrintMap<N> ( m_stateID, m_startPos, m_size, details );
  }
};

template< cube_size N >
RawStateMap<N>::RawStateMap()
: m_refCount ( nullptr )
, m_size     ( 0 )
, m_stateID  ( 0 )
, m_stateMap ( nullptr )
, m_transMap ( nullptr )
, m_startPos ( nullptr )
, m_subspace ( nullptr )
{
}

template< cube_size N >
RawStateMap<N>::RawStateMap( const PosID * P, const size_t size)
: m_refCount ( nullptr )
, m_size     ( size )
, m_stateID  ( 0 )
, m_stateMap ( nullptr )
, m_transMap ( nullptr )
, m_startPos ( nullptr )
, m_subspace ( nullptr )
{
  build( P, size );
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
  
  if ( m_refCount && -- *m_refCount < 1 )
  {
    dealloc();
  }
  
  refCopy( RS );
  return * this;
}

template< cube_size N >
void RawStateMap<N>::set( const Rubik<N> & R, const CubeID view ) const
{ 
  m_stateID = 0;
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    m_subspace[ posIndex ] = Simplex::Composition( view, R.transpose( m_startPos[ posIndex ] ) );
    m_stateID += m_subspace[ posIndex ] * pow24( posIndex );
  }
}

template< cube_size N >
void RawStateMap<N>::refCopy( const RawStateMap<N> & RS )
{
  m_size     = RS.size();
  m_stateID  = RS.m_stateID; 
  
  m_refCount = RS.m_refCount;
  m_startPos = RS.m_startPos;
  m_stateMap = RS.m_stateMap;
  m_transMap = RS.m_transMap;
  m_subspace = RS.m_subspace;
  
  ++ *m_refCount;
}

template< cube_size N >
void RawStateMap<N>::build( const PosID * pos, const size_t size )
{
  if ( m_refCount && -- *m_refCount < 1 )
  {
    dealloc();
  }
  m_size     = size;
  m_startPos = pos;
  m_refCount = new std::atomic<int> ( 1 );
  m_subspace = new CubeID [ m_size + 1 ] {};
  createStateMap();
  createTransMap();
  m_stateID = 0;
}

template< cube_size N >
void RawStateMap<N>::nextState()
{
  CubeID * next = m_subspace;
  while ( 23 == * next )
  {
    *( next ++ ) = 0;
  }
  * next += 1;  
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
    transMap[ 24 * m_stateID + id ] = rotatedID;
  }  
}

template< cube_size N >
void RawStateMap<N>::expandNodeTo( CacheID * stateMap ) const
{   
  RotID   rotID = 1;
  all_rot( axis, layer, turn, N )
  {
    CacheID modified = m_stateID;
    for( int radix = 0; radix < m_size; ++ radix )
    {
      const PosID current = CPositions<N>::GetPosID( m_startPos[radix], m_subspace[radix] );
      if ( CPositions<N>::GetLayer( current, axis ) == layer )
      {
        const CubeID cid = Simplex::Composition( m_subspace[radix], Simplex::Tilt( axis, turn ) );
        modified += ( cid - m_subspace[radix] ) * pow24( radix );
      }
    }
    stateMap[ _crot::AllRotIDs * m_stateID + rotID ] = modified;
    ++ rotID;
  }
}

template< cube_size N >
void RawStateMap<N>::createStateMap()
{
  m_stateID = 0;
  CacheID * stateMap = new CacheID[ pow24( m_size ) * _crot::AllRotIDs ] {};
  for( m_stateID = 0; m_stateID < pow24( m_size ); ++ m_stateID )
  {
    expandNodeTo( stateMap );
    nextState();
  }  // exit criteria: m_stateID = 24 ^ size ==> m_subspace = { ++X, 0, 0, ... }
  m_stateMap = stateMap;
}

template< cube_size N >
void RawStateMap<N>::createTransMap()
{
  m_stateID = 0;
  CacheID * transMap = new CacheID[ pow24( m_size ) * 24 ] {};
  for( m_stateID = 0; m_stateID < pow24( m_size ); ++ m_stateID )
  {
    transformNodeTo( transMap );
    nextState();
  }  // exit criteria: m_stateID = 24 ^ size ==> m_subspace = { ++X, 0, 0, ... }
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
  if ( m_refCount && -- *m_refCount < 1 )
  {
    dealloc();
  }
}


template< cube_size N >
class RawStateMap2
{
  PosID * m_pattern;
  size_t  m_size;
  
  CacheID * m_stateMap;
  CacheID * m_transMap;
  
  mutable CacheID m_stateID;
  
public:
    
  RawStateMap2 ( const size_t buff, const PosID * pattern = nullptr )
   : m_pattern ( new PosID[ buff ] )
   , m_size    ( 0 )
   , m_stateMap( new CacheID[ pow24( buff ) * CRotations<N>::AllRotIDs ] )
   , m_transMap( new CacheID[ pow24( buff + 1 ) ] )
  {
    // pre-condition
    for( int i = 0; i < CRotations<N>::AllRotIDs; ++ i )
      m_stateMap[i] = 0;
    // iteratively increasing cache
    while ( pattern && m_size < buff )
      addCube( *( pattern ++ ) );
    // post-condition
    m_stateID = 0;
    for( int i = 0; i < 74; ++ i )
    {
      clog_( '\n', i, ':' );
      for( int j = 0; j < CRotations<N>::AllRotIDs; ++j )
      {
        clog_( m_stateMap[ i *  CRotations<N>::AllRotIDs + j ] );
      }
    }
  };
   
  ~RawStateMap2()
  {
    delete[] m_pattern;
    delete[] m_stateMap;
    delete[] m_transMap;
  }
  
  void addCube ( const PosID  );
  void extendBy( const CubeID );
  
  CacheID state() const
  {
    return m_stateID;
  }
  
  CacheID state( const CubeID view ) const
  {
    return m_transMap[ 24 * m_stateID + view ];
  }
  
  CubeID prior() const
  {
    return m_stateID / pow24( m_size - 1 );
  }
  
  void move( const RotID rotID ) const
  {
    m_stateID = m_stateMap[ CRotations<N>::AllRotIDs * m_stateID + rotID ];
  }
  
  void print( const bool details = false ) const
  {
    PrintMap<N> ( m_stateID, m_pattern, m_size, details );
  }
};

template< cube_size N >
void RawStateMap2<N>::addCube( const PosID posID )
{
  clog( m_size, (int) posID );
  m_pattern[ m_size ] = posID;
  
  m_stateID = 0;
  all_cubeid( cid )
  {
    extendBy( cid );
  }
  ++ m_size;
}

template< cube_size N >
void RawStateMap2<N>::extendBy( const CubeID cid )
{
  const CacheID subSize  = pow24( m_size );
  const CacheID extStand = cid * subSize;
  
  const PosID pos   = CPositions<N>::GetPosID( m_pattern[ m_size ], cid );
  
  long  turned[9] = {}, * pTurned = turned;
  RotID recalc[9] = {}, * pRecalc = recalc; 
  for( Axis axis : { _X, _Y, _Z } )
  {
    const Layer layer = CPositions<N>::GetLayer( pos, axis );
    for( Turn turn : { 1, 2, 3 } )
    {
      * ( pTurned ++ ) = Simplex::Composition( cid, Simplex::Tilt( axis, turn ) ) * subSize - extStand;
      * ( pRecalc ++ ) = CRotations<N>::GetRotID( axis, layer, turn );
    }
  }
  for( CacheID subID = 0; subID < subSize; ++ subID, ++ m_stateID )
  {
    const CacheID stateBase = m_stateID * CRotations<N>::AllRotIDs;
    const CacheID subBase   = subID     * CRotations<N>::AllRotIDs;
    for( RotID rotID = 0; rotID < CRotations<N>::AllRotIDs; ++ rotID )
    {
      m_stateMap[ stateBase + rotID ] = m_stateMap[ subBase + rotID ] + extStand;
    }
    pTurned = turned;
    pRecalc = recalc;
    for( int i = 0; i < 9; ++ i)
    {
      m_stateMap[ stateBase + *( pRecalc ++ ) ] += *( pTurned ++ );
    }
  }
}
#endif  //  ! RAW_MAP__H
