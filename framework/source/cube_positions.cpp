
#include <simplex.h>
#include <random>
#include <cube_positions.h>


/// ----------------------------------- Template definitions starts here ------------------------------------- ///
template< cube_size N >
CPositions<N> * CPositions<N>::Singleton = nullptr;

template< cube_size N >
CPositions<N>::CPositions( )
{
  Singleton = this;
  initPositions();
}

template< cube_size N >
void CPositions<N>::Instance( void )
{
  if ( Singleton == nullptr ) new CPositions<N>;
}

template< cube_size N >
void CPositions<N>::OnExit( void )
{
  delete Singleton; Singleton = nullptr;
}

template< cube_size N > inline
PosID * CPositions<N>::GetNode( CubeID id )
{
  return Singleton->m_routerPositions[ id ];
}

template< cube_size N >
PosID * CPositions<N>::GetNode( Layer x, Layer y, Layer z )
{
  return GetNode( GetPosID ( x, y, z) );
}

template< cube_size N > inline
bool CPositions<N>::ValID( PosID id )
{
  return 0 <= id && id < GetSize();
}

template< cube_size N > inline
PosID CPositions<N>::GetPosID( PosID id, CubeID rot )
{
  return Singleton -> m_routerPositions[ id ][ rot ];
}

template< cube_size N > inline
PosID CPositions<N>::GetPosID( const Coord & C )
{
  return GetPosID( C.x, C.y, C.z);
}

template< cube_size N > inline
PosID CPositions<N>::GetPosID( Layer x, Layer y, Layer z )
{
  return Singleton -> m_coordToIndex[ x ][ y ][ z ];
}

template< cube_size N > inline
PosID CPositions<N>::GetPosID( Layer x, Layer y, Layer z, CubeID rot )
{
  return GetNode( x, y, z ) [ rot ];
}

template< cube_size N > inline
PosID CPositions<N>::GetLayer( Axis a, Layer l, size_t id )
{
  return Singleton -> m_frameworkLayer [ a ][ l ][ id ];
}

template< cube_size N > inline
Layer CPositions<N>::GetLayer( PosID p, Axis a )
{
  return Singleton -> m_indexToCoord [ p ][ a ];
}

template< cube_size N > inline
Layer CPositions<N>::GetLayer( PosID p, CubeID r, Axis a )
{
  return Singleton -> m_indexToCoord [ Singleton -> m_routerPositions[ p ][ r ] ][ a ];
}

// an auxiliary function for inner calculations:
//    start from ( x y z )
//    take a turn corresponding to rot value
//    return the given ( x' y' z' ) vector
template< cube_size N >
Coord CPositions<N>::rotate( Layer x, Layer y, Layer z, CubeID id)
{
  Coord result;
  OCube rot = Simplex::GetCube( id );
  Layer coord[6];
  coord[ _F ] = N - 1 - z;
  coord[ _R ] = N - 1 - x;
  coord[ _U ] = N - 1 - y;
  coord[ _B ] = z;
  coord[ _L ] = x;
  coord[ _D ] = y;

  result.x = coord[ rot.whatIs ( _L ) ];
  result.y = coord[ rot.whatIs ( _D ) ];
  result.z = coord[ rot.whatIs ( _B ) ];
  return result;
}

template< cube_size N > inline
BitMapID CPositions<N>::ActOn( const PosID posID )
{
  const Coord pos = CPositions<N>::GetCoord( posID );
  return ( 14ULL << ( 3 * pos.x ) ) | ( 14ULL << ( 3 * N + 3 * pos.y ) ) | ( 14ULL << ( 6 * N + 3 * pos.z ) );
}

template< cube_size N >
void CPositions<N>::initIndices()
{
  Layer index = 0,
  idX [N] = {}, idY [N] = {}, idZ [N] = {}; // slice indices
  for_vector ( x, y, z, N )
  {
    m_indexToCoord [ index ][ _X ] = x;
    m_indexToCoord [ index ][ _Y ] = y;
    m_indexToCoord [ index ][ _Z ] = z;

    m_coordToIndex [ x ][ y ][ z ] = index++;
  } // GetPosID() is fully working now
}

template< cube_size N >
void CPositions<N>::initPositions()
{
  initIndices();
  for_vector ( x, y, z, N )
  {
    if ( GetPosID( x, y, z ) != 0xFF )
    {
      all_cubeid ( id )
      {
        const Coord C = rotate ( x, y, z, id );
        m_routerPositions[ GetPosID( x, y, z ) ][ id ] = GetPosID( C );
      }
    }
  }
}

template< cube_size N > inline
Coord CPositions<N>::GetCoord( PosID p )
{
  return Coord(
    Singleton->m_indexToCoord [ p ][ _X ],
    Singleton->m_indexToCoord [ p ][ _Y ],
    Singleton->m_indexToCoord [ p ][ _Z ]
    );
}

template< cube_size N >
PosID CPositions<N>::Random()
{
 static std::random_device                 randomDevice;
 static std::default_random_engine         randomEngine( randomDevice() );
 static std::uniform_int_distribution<int> distribution( 0, AllPosIDs - 1 );

 return distribution( randomEngine );
}
