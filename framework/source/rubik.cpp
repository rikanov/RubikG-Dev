#include <rubik.h>


/// ----------------------------------- Template definitions starts here ------------------------------------- ///

 // Constructors
//  ------------

// default
template< cube_size N >
Rubik<N>::Rubik()
 : frameworkSpace( new CubeID [ Fsize ] () )
{

}

// copy
template< cube_size N >
Rubik<N>::Rubik( const Rubik<N>& C )
 : frameworkSpace( new CubeID [ Fsize ] )
{
  for ( int i = 0; i < Fsize; ++ i )
    frameworkSpace[i] = C.frameworkSpace[i];
}

template< cube_size N >
Rubik<N>::Rubik( Rubik<N> && f )
{
  frameworkSpace = f.frameworkSpace;
  f.frameworkSpace = nullptr;
}

 // Operations
//  ----------

template< cube_size N >
void Rubik<N>::reset()
{
  for ( PosID posID = 0; posID < Fsize; ++ posID )
    frameworkSpace[ posID ] = 0;
}

template< cube_size N >
void Rubik<N>::history( const size_t step )
{
  reset();
  for ( auto P = begin(); P != Array<RotID>::at( step ); ++ P )
  {
    setRotate( *P );
  }
}

template< cube_size N >
void Rubik<N>::revert( const size_t step )
{
  Stack<RotID>::set( step );
  refresh();
}

template< cube_size N >
void Rubik<N>::refresh()
{
  history( 0 );
}

// assignement
template< cube_size N >
const Rubik<N>& Rubik<N>::operator = ( const Rubik<N>& C )
{
  for ( int i = 0; i < Fsize; ++ i )
    frameworkSpace[i] = C.frameworkSpace[i];
  return *this;
}

// equlity
template< cube_size N >
bool Rubik<N>::operator == ( const Rubik<N>& C ) const
{
  for ( int i = 0; i < Fsize; ++ i )
  {
    if ( frameworkSpace[i] != C.frameworkSpace[i] )
      return false;
  }
  return true;
}

// solved
template< cube_size N >
bool Rubik<N>::isSolved() const
{
  for ( int i = 0; i < Fsize; ++ i )
  {
    if ( frameworkSpace[i] != 0 )
      return false;
  }
  return true;
}

// clockwise rotation one layer (side) with 90 degree turns
template< cube_size N >
void Rubik<N>::rotate( const Axis axis, const Layer layer, const Turn turn )
{
  const int cubes = CPositions<N>::LayerSize( layer );
  CubeID state  [ N * N ]; // to store rotational states temporarily.

  const CubeID twist = Simplex::Tilt( axis, turn );
  for ( int index = 0; index < cubes; ++index )
  {
    const CubeID & f = frameworkSpace [ CPositions<N>::GetLayer( axis, layer, index ) ];
    state[index]     = Simplex::Composition( f, twist );
  }
  for ( int index = 0; index < cubes; ++index ) // apply stored assignments
  {
    frameworkSpace[ CPositions<N>::GetPosID( CPositions<N>::GetLayer( axis, layer, index ), twist ) ] = state[ index ];
  }
}

// rotation by using RotID
template< cube_size N >
void Rubik<N>::setRotate( const RotID rotID )
{
  const Axis  axis  = CRotations<N>::GetAxis  ( rotID );
  const Layer layer = CRotations<N>::GetLayer ( rotID );
  const Turn  turn  = CRotations<N>::GetTurn  ( rotID );
  rotate( axis, layer, turn );
}

template< cube_size N >
void Rubik<N>::rotate( const RotID rotID )
{
  setRotate( rotID );
  push( rotID );
}
template< cube_size N >
void Rubik<N>::rotate( const Sequence & seq )
{
  for( auto P = seq.begin(); P != seq.end(); ++ P )
  {
    rotate( *P );
  }
}

template< cube_size N >
size_t Rubik<N>::shuffle( size_t depth )
{
  Stack<RotID>::reset();
  static std::random_device randomDevice;
  static std::default_random_engine engine( randomDevice() );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = depth == 0 ? dist( engine ) : depth;
  while ( 0 < counter-- )
  {
    Rubik<N>::rotate( CRotations<N>::Random() );
  }
  return Stack<RotID>::size();
}

 // Query functions
//  ---------------
template< cube_size N >
CubeID Rubik<N>::getCubeID( PosID id, RotID rot ) const
{
  return frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ];
}

template< cube_size N >
CubeID Rubik<N>::getCubeID( PosID id ) const
{
  return frameworkSpace[id];
}

template< cube_size N >
OCube Rubik<N>::getCube( PosID id ) const
{
  return Simplex::GetCube( getCubeID ( id ) );
}

template< cube_size N >
Coord Rubik<N>::whatIs( Coord C ) const
{
  return CPositions<N>::getCoord( whatIs ( CPositions<N>::GetPosID( C ) ) );
}

template< cube_size N >
Coord Rubik<N>::whereIs( Coord C ) const
{
  return CPositions<N>::getCoord( whereIs( CPositions<N>::GetPosID( C ) ) );
}

template< cube_size N >
PosID Rubik<N>::whatIs( PosID id ) const
{
  return CPositions<N>::GetPosID( id, Simplex::Inverse( frameworkSpace [ id ] ) );
}

template< cube_size N >
PosID Rubik<N>::whereIs( PosID id ) const
{
  CubeID rot = 0;
  while ( frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ] != rot )
  {
    ++ rot;
  }
  return CPositions<N>::GetPosID( id, rot );
}

template< cube_size N >
CubeID Rubik<N>::transpose( PosID id, CubeID trans ) const
{
  id = CPositions<N>::GetPosID( id, trans );
  CubeID rot = 0;
  while ( frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ] != rot )
  {
    ++ rot;
  }
  return Simplex::Composition( trans,rot );
}

template< cube_size N >
Orient Rubik<N>::getOrient( const Orient right, const Orient up, Layer x, Layer y ) const
{
  if ( Coaxial ( right, up ) || x < 0 || x >= N || y < 0 || y >= N ) // invalid setting
  {
    return _NF;
  }

  const CubeID trans  = Simplex::GetGroupID     ( right, up );
  const CubeID inv    = Simplex::Inverse        ( trans );
  const Orient orient = OCube::FrontSide        ( right, up ); // = Simplex::GetCube( trans ).whatIs( _F );
  const PosID  index  = CPositions<N>::GetPosID ( x, y, N - 1, inv );

  return getCube ( index ).whatIs( orient );
}

template< cube_size N >
bool Rubik<N>::integrity() const
{
  static const Orient orientations [6][2] = { { _F, _U }, { _R, _U }, { _B, _U }, { _L, _U }, { _R, _B }, { _R, _F } };

  bool result = true;
  int counter[6] = {};
  for ( auto side : orientations )
  {
    for ( Layer x = 0; x < N; ++x )
    {
      for ( Layer y = 0; y < N; ++y )
      {
        ++counter[ Rubik<N>::getOrient( side[0], side[1], x, y ) ];
      }
    }
  }
  for( int idx = 0; idx < 6; ++ idx )
  {
    result &= counter[idx] == N * N ;
  }

  return result;
}

 // Destructor
//  ----------
template< cube_size N >
Rubik<N>::~Rubik()
{
  delete[] frameworkSpace;
  frameworkSpace = nullptr;
}

