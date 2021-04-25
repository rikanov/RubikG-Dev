#ifndef CACHE_GENERATOR__H
#define CACHE_GENERATOR__H

#include <rubik.h>
#include <containers.h>

inline 
CacheID GetCacheID( const CubeID * P, const size_t size )
{
  CacheID result = 0;
  const CubeID inv0 = Simplex::Inverse( P[0] );
  for( size_t i = 1; i < size; ++i )
  {
    result += Simplex::Composition( P[i], inv0 ) * _pow24[ i - 1 ];
  }
  return result;
}

inline
void SetCacheID( CubeID * P, CacheID cacheID, const size_t size )
{
  P[0] = 0;
  for( size_t i = 1; i < size; ++i, cacheID /= 24 )
  {
    P[i] = cacheID % 24;
  }
}

inline
void SetCacheID( CubeID * P, CacheID cacheID, const size_t size, const CubeID prior )
{
  P[0] = prior;
  for( size_t i = 1; i < size; ++i, cacheID /= 24 )
  {
    P[i] = Simplex::Composition( cacheID % 24, prior);
  }
}

template< size_t N >
class CacheIDmapper
{

  using _crot = CExtRotations< N >;

protected:
  size_t m_size; // number of cubies in the subspace
  const PosID * m_position; // initial positions of the cubies

private:
  Qeueu  * m_qeueu;

  CacheID  m_parentID;
  CubeID * m_parent;
  CubeID * m_child;

private:
  void clean();
  void setParent();
  void cloneParent();
  void nextChild( const Axis axis, const Layer layer, const Turn turn );

public:
  CacheIDmapper();
  CacheIDmapper( const CacheIDmapper & ) = delete;
  ~CacheIDmapper();

  void initialPosition( const PosID * pos, const size_t size );

  bool acceptID ( CacheID cacheID )   { return *m_qeueu << cacheID;         }
  bool accept   ( const CubeID * P )  { return *m_qeueu << getCacheID( P ); }
  
  void createMap( CacheIDmap<N> & result );
  
private:
  void addLayerRotations( CacheIDmap<N> & result );
  void addSliceRotations( CacheIDmap<N> & result );

  CacheID getCacheID( const CubeID * P ) const   { return GetCacheID( P, m_size ); }

};

template< size_t N > CacheIDmapper<N>::CacheIDmapper()
: m_size ( 0 )
, m_position ( nullptr )
, m_qeueu    ( nullptr )
, m_parent   ( nullptr )
, m_child    ( nullptr )
{
}

template< size_t N >
void CacheIDmapper<N>::initialPosition( const PosID * pos, const size_t size )
{
  clean();

  m_size    = size;
  m_parent  = new CubeID [ size ];
  m_child   = new CubeID [ size ];
  m_qeueu   = new Qeueu ( size - 1 );
  
  m_position = pos;
  acceptID( 0 ) ;
}


template< size_t N >
void CacheIDmapper<N>::createMap( CacheIDmap<N> & result )
{
  result.init( m_size );
  while( *m_qeueu >> m_parentID )
  {
    setParent();
    addLayerRotations( result );
    addSliceRotations( result );
  }
  clean();
}

template< size_t N >
void CacheIDmapper<N>::addLayerRotations( CacheIDmap<N> & result )
{
  all_rot( axis, layer, turn, N )
  {
    cloneParent();
    nextChild( axis, layer, turn );
    const CacheID nextID = getCacheID( m_child );
    result.connect( m_parentID, axis, layer, turn, nextID, *m_qeueu << nextID );
  }
}

template< size_t N >
void CacheIDmapper<N>::addSliceRotations( CacheIDmap<N> & result )
{
  if ( N < 4 )
  {
    return;
  }
  for( Axis axis: { _X, _Y, _Z } )
  {
    for( Turn turn : { 1, 2, 3 } )
    {
      cloneParent();
      nextChild( axis, 0, turn );
      for( Layer layer = 1; layer < N - 2; ++layer )
      {
        nextChild( axis, layer, turn );
        const CacheID nextID = getCacheID( m_child );
        result.connect( m_parentID, axis, layer + N - 1, turn, nextID, *m_qeueu << nextID );
      }
    }
  }
}

template< size_t N >
void CacheIDmapper<N>::nextChild ( const Axis axis, const Layer layer, const Turn turn )
{
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    if ( CPositions<N>::GetLayer( m_position[posIndex], m_child[posIndex], axis ) == layer )
    {
      m_child[posIndex] = Simplex::Composition( m_child[posIndex], Simplex::Tilt( axis, turn) );
    }
  }
}

template< size_t N >
void CacheIDmapper<N>::setParent()
{
  SetCacheID ( m_parent, m_parentID, m_size );
}

template< size_t N >
void CacheIDmapper<N>::cloneParent()
{
  for( size_t i = 0; i < m_size; ++i )
  {
    m_child[i] = m_parent[i];
  }
}

template< size_t N >
void CacheIDmapper<N>::clean()
{
  delete[] m_parent;
  delete[] m_child;
  delete   m_qeueu;

  m_parent = nullptr;
  m_child  = nullptr;
  m_qeueu  = nullptr;
}

template< size_t N >
CacheIDmapper<N>::~CacheIDmapper()
{
  clean();
}

#endif // ! CACHE_GENERATOR__H
