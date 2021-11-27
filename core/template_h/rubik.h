
#ifndef CUBE_FRAMEWORK_HEADER
#define CUBE_FRAMEWORK_HEADER

#include <cube_rotations.h>
#include <cube_positions.h>
#include <sequence.h>

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template< cube_size N >
class Rubik: public Sequence
{
  static constexpr int Fsize = CPositions<N>::GetSize();
  CubeID * frameworkSpace;

  void setRotate( const RotID rotID );
  void rotate  ( const Axis, const Layer, const Turn turn = 1 );

public:
  
  // Constructors
  Rubik( void );
  Rubik( const Rubik<N>& );
  Rubik( Rubik<N>&& f );

  // Operations
  void reset   ();
  void refresh ();
  void history ( const size_t step );
  void revert  ( const size_t step );
  void rotate  ( const RotID rotID );
  void rotate  ( const Sequence & seq );
  void shuffle ( int depth = 0 );
  
  // Operators
  bool      operator== ( const Rubik<N>& X ) const;
  const Rubik<N>& operator=  ( const Rubik<N>& B ) ;
  // Destructor
  ~Rubik( );
  
  // Query functions
  CubeID  getCubeID    ( PosID id, RotID rot ) const { return frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ]; }
  CubeID  getCubeID    ( PosID id )            const { return frameworkSpace[id];                                   }
  OCube   getCube      ( PosID id )            const { return Simplex::GetCube( getCubeID ( id ) );                 }
  
  inline PosID whatIs    ( PosID id ) const ;
  inline PosID whereIs   ( PosID id ) const ;

  CubeID transpose ( PosID id, CubeID trans = 0 ) const ;

  Coord   whatIs    ( Coord C )  const { return CPositions<N>::getCoord( whatIs ( CPositions<N>::GetPosID( C ) ) ); }
  Coord   whereIs   ( Coord C )  const { return CPositions<N>::getCoord( whereIs( CPositions<N>::GetPosID( C ) ) ); }
  bool    integrity ( void  )  const ;
  bool    isSolved  ( void  )  const;
  Orient  getOrient ( const Orient front, const Orient up, int x, int y) const; // left-bottom corner: x = 0, y = 0

  // Printer
  void show ( Orient, Orient, Layer, Layer ) const;
  void show ( Orient ) const;
  void print( Orient, Orient ) const;
  void print( bool separator = true ) const;
};

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
  for ( RotID rotID = start( step ); rotID; rotID = next() )
  {
    setRotate( rotID );
  }
}

template< cube_size N >
void Rubik<N>::revert( const size_t step )
{
  setState( step );
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
  store( rotID );
}
template< cube_size N >
void Rubik<N>::rotate( const Sequence & seq )
{
  for( RotID next = seq.start(); next; next = seq.next() )
  {
    rotate( next );
  }
}

template< cube_size N > 
void Rubik<N>::shuffle( int depth )
{
  static std::random_device randomDevice;
  static std::default_random_engine engine( randomDevice() );
  static std::uniform_int_distribution<int> dist( 2 * N * N, 3 * N * N);
  int counter = depth == 0 ? dist( engine ) : depth; 
  while ( 0 < counter-- )
  {
    Rubik<N>::rotate( CRotations<N>::Random() );
  }
}

 // Query functions
//  ---------------
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
Orient Rubik<N>::getOrient ( const Orient right, const Orient up, int x, int y ) const
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

#include <rubik_print.h>
#endif
