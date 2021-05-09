#ifndef INSIGHT__H
#define INSIGHT__H

#include <cache_generator.h>
#include <rubik.h>


template< cube_size N >
class Insight
{
  using _crot = CExtRotations<N>;

  CacheID        m_stateID;
  CubeID         m_prior;

  const size_t   m_size;
  const PosID  * m_pos;

  std::shared_ptr< const CubeID [] >     m_priorCache;
  std::shared_ptr< const RotID  [] >     m_transRotation;
  std::shared_ptr< const CacheIDmap<N> > m_map;

  mutable const RotID  * m_nextSuggested;
  const CubeID           m_base;
  const CubeID           m_view;

  void initMap( SubSpace, const Axis toRoll );
  void initPos( SubSpace );
  void initPos( const PosID * P );
  void initPrior();
  void initRotIDs();

public:
  Insight( SubSpace P, const CubeID cid = 0, const Axis toRoll = _NA );
  Insight( const Insight<N> &, const CubeID cid );
  ~Insight();
  Insight( const Insight<N> & ) = delete;

  void set( const Rubik<N> & );

  int rotate( const Axis axis, const Layer layer, const Turn turn );
  int rotate( const RotID rotID );

  CacheID state() const
  {
    return m_stateID;
  }
  
  CubeID prior() const
  {
    return m_prior;
  }

  OCube priorCube() const
  {
    return Simplex::GetCube( m_prior );
  }

  DistID distance() const
  {
    return m_map -> distance( m_stateID );
  }

  RotID start() const
  {
    m_nextSuggested = m_map -> router( m_stateID );
    return next();
  }

  RotID next() const
  {
    return  *m_nextSuggested == 0 ? 0 : _crot::GetRotID( *( m_nextSuggested ++ ), m_prior);
  }

  void print( const bool details = false ) const;
};

template< cube_size N >
Insight<N>::Insight( SubSpace P, const CubeID cid, const Axis toRoll )
  : m_size  ( P.size() )
  , m_pos( nullptr )
  , m_map( nullptr )
  , m_base( cid )
  , m_view( 0 )
{
  initPos( P );
  initMap( P, toRoll );
  initRotIDs();
  initPrior();
}

template< cube_size N >
Insight<N>::Insight( const Insight<N> & orig, const CubeID cid )
  : m_size( orig.m_size )
  , m_priorCache ( orig.m_priorCache )
  , m_transRotation( orig.m_transRotation )
  , m_map ( orig.m_map )
  , m_base( cid )
  , m_view( cid )
{
  initPos( orig.m_pos );
}

template< cube_size N >
void Insight<N>::initPos( SubSpace P )
{
  PosID * pos = new PosID [ P.size() ];
  size_t i = 0;
  for( auto p: P )
  {
    pos[ i++ ] = CPositions<N>::GetPosID( p, m_base );
  }

  m_pos = pos;
}

template< cube_size N >
void Insight<N>::initPos( const PosID* P )
{
  PosID * pos = new PosID [ m_size ];
  for( size_t id = 0; id < m_size; ++id )
  {
    pos[id] = CPositions<N>::GetPosID( P[id], m_base );
  }
  m_pos = pos;
}

template< cube_size N > 
void Insight<N>::initMap( SubSpace P, const Axis toRoll )
{
  CacheIDmapper<N> * mapBuilder = new CacheIDmapper<N>;
  mapBuilder -> initialPosition( m_pos, P.size() );
  mapBuilder -> accept( toRoll) ;
  m_map =  mapBuilder -> createMap();
  delete mapBuilder;
}

template< cube_size N >
void Insight<N>::initPrior()
{
  CubeID * priorCache = new CubeID [ _crot::AllRotIDs * 24 ];
  all_rot( axis, layer, turn, _crot::NT )
  {
    all_cubeid( prior )
    {
      const RotID rotID = m_transRotation[ prior * _crot::AllRotIDs + _crot::GetRotID( axis, layer, turn ) ];

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
  m_priorCache.reset( priorCache );
}

template< cube_size N >
void Insight<N>::initRotIDs()
{
  RotID * transRotation = new RotID [ 24 * _crot::AllRotIDs ];
  all_rot( axis, layer, turn, _crot::NT )
  {
    all_cubeid( prior )
    {
      transRotation[ prior * _crot::AllRotIDs + _crot::GetRotID( axis, layer, turn ) ] = _crot::GetRotID( axis, layer, turn, Simplex::Composition( Simplex::Inverse( prior ), Simplex::Inverse( m_view ) ) );
    }
  }
  m_transRotation.reset( transRotation );
}

template< cube_size N >
void Insight<N>::set( const Rubik<N> & R )
{
  CubeID * subset = new CubeID [ m_size ];
  for( size_t posIndex = 0; posIndex < m_size; ++ posIndex )
  {
    subset[ posIndex ] = Simplex::Composition( m_view, R.transpose( m_pos[ posIndex ] ) );
  }
  m_prior = subset[0];
  m_stateID = GetCacheID( subset, m_size );
  delete[] subset;
}

template< cube_size N >
int Insight<N>::rotate( const Axis axis, const Layer layer, const Turn turn )
{
  return rotate( _crot::GetRotID( axis, layer, turn ) );
}

template< cube_size N >
int Insight<N>::rotate( const RotID rotID )
{
  const RotID rotIDt = m_transRotation[ m_prior * _crot::AllRotIDs  + rotID ];

  m_prior   = m_priorCache[ 24 * rotIDt + m_prior ] ;
  m_stateID = m_map -> getState( m_stateID, rotIDt ) ;
  return distance();
}

template< cube_size N >
Insight<N>::~Insight()
{
  delete[] m_pos;
}

#include <insight_print.h>
#endif //  ! INSIGHT__H
