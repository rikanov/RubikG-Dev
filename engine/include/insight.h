#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>

template< size_t N >
class Insight
{
  using _crot = CExtRotations<N>;

  CacheID        m_stateID;
  CubeID         m_prior;

  const size_t   m_size;
  const CubeID * m_priorCache;

  const PosID  * m_pos;

  const CacheIDmap<N> * m_map;

  void initMap( SubSpace, const CubeID );
  void initPrior();

public:
  Insight( SubSpace P, const CubeID cid = 0 );
  ~Insight();
  Insight( const Insight<N> & ) = delete;

  void set( const Rubik<N> & );

  int rotate( const Axis axis, const Layer layer, const Turn turn );
  int rotate( const RotID rotID );

  CacheID state() const
  {
    return m_stateID;
  }
  
  size_t prior() const
  {
    return m_prior;
  }

  OCube priorCube() const
  {
    return Simplex::GetCube( m_prior );
  }

  size_t distance() const
  {
    return m_map -> distance( m_stateID );
  }

  size_t weight() const
  {
    return m_map -> weight( m_stateID );
  }

  void print( const bool details = false ) const;
};

template< size_t N >
Insight<N>::Insight( SubSpace P, const CubeID cid )
  : m_size  ( P.size() )
{
  initMap( P, cid );
  initPrior();
}
template<size_t N> void Insight<N>::initMap( SubSpace P, const CubeID cid )
{
  CacheIDmapper<N> * mapBuilder = new CacheIDmapper<N>;
  CacheIDmap<N>    * map = new CacheIDmap<N>();

  PosID * pos = new PosID [ P.size() ];
  size_t i = 0;
  for( auto p: P )
  {
    pos[ i++ ] = CPositions<N>::GetPosID( p, cid );
  }
  mapBuilder -> initialPosition( pos, P.size() );
  mapBuilder -> createMap( *map );
  m_map = map;
  m_pos = pos;
  delete mapBuilder;
}

template< size_t N >
void Insight<N>::initPrior()
{
  CubeID * priorCache = new CubeID [ _crot::AllRotIDs * 24 ];
  all_rot( axis, layer, turn, _crot::NT )
  {
    all_cubeid( prior )
    {
      const RotID rotID = CExtRotations<N>::GetRotID( axis, layer, turn, Simplex::Inverse( prior ) );

      if ( ( layer  < N && layer         == CPositions<N>::GetLayer( m_pos[0], prior, axis ) ) ||
           ( layer >= N && layer - N + 1 >= CPositions<N>::GetLayer( m_pos[0], prior, axis ) ) )
      {
        priorCache[ 24 * rotID + prior ] = Simplex::Tilt( prior, axis, turn );
      }
      else
      {
        priorCache[ 24 * rotID + prior ] = prior;
      }
    }
  }
  m_priorCache = priorCache;
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
int Insight<N>::rotate( const Axis axis, const Layer layer, const Turn turn )
{
  const RotID rotID = CExtRotations<N>::GetRotID( axis, layer, turn, Simplex::Inverse( m_prior ) );

  m_prior   = m_priorCache[ 24 * rotID + m_prior ] ;
  m_stateID = m_map -> getState( m_stateID, rotID ) ;
  return distance();
}

template< size_t N >
int Insight<N>::rotate( const RotID rotID )
{
  return rotate( _crot::GetAxis( rotID ), _crot::GetLayer( rotID ), _crot::GetTurn( rotID ) );
}

template< size_t N >
Insight<N>::~Insight()
{
  delete   m_map;
  delete[] m_pos;
  delete[] m_priorCache;
}

#include <insight_print.h>
#endif //  ! INSIGHT__H
