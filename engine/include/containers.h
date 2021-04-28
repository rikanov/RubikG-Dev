#ifndef CONTAINERS__H
#define CONTAINERS__H

#include <initializer_list>

constexpr size_t _pow24[] = { 1, 24, 576, 13824, 331776, 7962624 };

using SubSpace = const std::initializer_list <PosID>;

// an excluding FIFO object: any CacheID value can be pushed in only once a life time
class Qeueu
{
  const size_t m_size;

  CacheID * m_qeueudCubes ;
  CacheID * m_qeuIn ;
  CacheID * m_qeuOut;
  bool    * m_used;
public:
  
  Qeueu( const int& size )
  : m_size( size )
  , m_qeueudCubes( new CacheID [ _pow24[ size ] ] )
  {
    m_qeuIn = m_qeuOut = m_qeueudCubes;
    m_used = new bool [ _pow24[ m_size ] ] { false };
  }

  bool operator << ( const CacheID& id )
  {
    if ( m_used[ id ] == false )
    {
      m_used[ id ] = true;
      *( m_qeuIn ++ ) = id;
      return true;
    }
    return false;
  }

  bool operator >> ( CacheID& id ) 
  {
      id = *( m_qeuOut ++ );
      return m_qeuOut <= m_qeuIn;
  }

  int count() const
  {
    return m_qeuIn - m_qeuOut;
  }

  ~Qeueu()
  {
    delete[] m_qeueudCubes;
    delete[] m_used;
  }
};

template< cube_size N >
class CacheIDmap
{
  using _crot = CExtRotations<N>;

  CacheID * m_map;
  DistID  * m_dist;

  RotID  * m_orderedSteps;
  DistID * m_complexity;
  size_t * m_weight;

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

  size_t complexity( const CacheID id ) const
  {
    return m_complexity[ id ];
  }

  const RotID * router( const CacheID cacheID ) const
  {
    return m_orderedSteps + cacheID * _crot::AllRotIDs;
  }

  int distance( CacheID cacheID ) const
  {
    return m_dist[ cacheID ];
  }

  int gradient( const CacheID cacheID, const RotID rotID ) const
  {
    return m_dist[ cacheID ] - m_dist[ getState(cacheID, rotID ) ];
  }


  size_t weight( CacheID cacheID ) const
  {
    return m_weight[ cacheID ];
  }

};

template< cube_size N > CacheIDmap<N>::CacheIDmap()
  :  m_map  ( nullptr )
  ,  m_dist ( nullptr )
  ,  m_orderedSteps ( nullptr )
  ,  m_complexity ( nullptr )
  ,  m_weight ( nullptr )
{
}

template< cube_size N >
void CacheIDmap<N>::init( const size_t size )
{
  clean();
  m_map  = new CacheID [ _pow24[ size - 1 ] * _crot::AllRotIDs ];
  m_dist = new DistID  [ _pow24[ size - 1 ] ] {};

  m_orderedSteps = new RotID  [ _pow24[ size - 1 ] * _crot::AllRotIDs ];
  m_complexity   = new DistID [ _pow24[ size - 1 ] ] {};
  m_weight       = new size_t [ _pow24[ size - 1 ] ] {};
}

template< cube_size N >
void CacheIDmap<N>::connect( const CacheID start, const Axis axis, const Layer layer, const Turn turn, const CacheID result, const bool first )
{
  if ( first )
  {
    m_dist[ result ] = m_dist[ start ] + 1;
  }
  if ( m_dist[ start ] + 1 == m_dist[ result ] )
  {
    m_weight[ result ] += m_weight[ start ] > 0 ? m_weight[ start ] : 1;
  }
  m_map[ result * _crot::AllRotIDs + _crot::GetRotID( axis, layer, 4-turn ) ] = start;
  m_orderedSteps[ result * _crot::AllRotIDs + m_complexity[ result ] ++ ] = _crot::GetRotID( axis, layer, 4-turn );
}

template< cube_size N >
void CacheIDmap<N>::clean()
{
  delete[] m_map;
  delete[] m_dist;
  delete[] m_orderedSteps;
  delete[] m_complexity;
  delete[] m_weight;

  m_map  = nullptr;
  m_dist = nullptr;
  m_orderedSteps = nullptr;
}

template< cube_size N >
CacheIDmap<N>::~CacheIDmap()
{
  clean();
}

#endif // ! CONTAINERS__H
