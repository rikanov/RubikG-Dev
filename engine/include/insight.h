#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template< size_t N >
class Insight
{
  using _crot = CRotations< 2 * N - 3 >;

  CacheID       m_stateID;
  CubeID        m_prior;

  const size_t  m_size;
  const PosID * m_pos;

  const CacheIDmap<N> * m_map;

public:
  Insight( SubSpace P, const CubeID cid = 0 );
  ~Insight();

  void set( const Rubik<N> & );

  int rotate( Axis axis, Layer layer, Turn turn );

  void step( const size_t id )
  {
    rotate( m_map -> router( m_stateID, id ) );
  }
  
  CacheID state() const
  {
    return m_stateID;
  }

  int distance() const
  {
    return m_map -> distance( m_stateID );
  }

  void print() const;
};

template< size_t N >
Insight<N>::Insight( SubSpace P, const CubeID cid )
  : m_size  ( P.size() )
{
  CacheIDmapper<N> mapBuilder;
  CacheIDmap<N>    * map = new CacheIDmap<N>();

  PosID * pos = new PosID [ P.size() ];
  size_t i = 0;
  for( auto p: P )
  {
    pos[ i++ ] = CPositions<N>::GetPosID( p, cid );
  }
  mapBuilder.initialPosition( pos, P.size() );
  mapBuilder.createMap( *map );
  m_map = map;
  m_pos = pos;
}

template< size_t N >
void Insight<N>::set( const Rubik<N> & R )
{
  m_prior = R.getCubeID( m_pos[0] );
  CubeID * subset = new CubeID [ m_size ];
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    subset[ posIndex ] = R.getCubeID( m_pos[ posIndex ] );
  }
  m_stateID = GetCacheID( subset, m_size );
  delete[] subset;
}

template< size_t N >
int Insight<N>::rotate( Axis axis, Layer layer, Turn turn )
{
  clog( "state:", m_stateID );
  clog_( _crot::ToString( CExtRotations<N>::GetRotID( axis, layer, turn ) ), "-->" );
  const RotID rotID = CExtRotations<N>::GetRotID( axis, layer, turn, Simplex::Inverse( m_prior ) );
  clog( _crot::ToString( rotID ) );
  if ( ( layer  < N && layer         == CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ||
       ( layer >= N && layer - N + 1 >= CPositions<N>::GetLayer( m_pos[0], m_prior, axis ) ) ) 
  {
    clog_( Simplex::GetCube( m_prior).toString(), "-->" );
    m_prior = Simplex::Tilt( m_prior, axis, turn ); clog( Simplex::GetCube( m_prior).toString() );
  }
    
  m_stateID = m_map -> getState( m_stateID, rotID ) ;
  print();

  return distance();
}

template< size_t N >
Insight<N>::~Insight()
{
  delete   m_map;
  delete[] m_pos;
}

#include <insight_print.h>
#endif //  ! INSIGHT__H
