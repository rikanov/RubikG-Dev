/*
 * Cube frameworks: store only the rotational state of all cublets of an N x N Rubics cube. (without their position in the Rubics cube)
 *
 * Copyright (C) 2020  Robert Ikanov <robert.ikanov@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CUBE_FRAMEWORK_HEADER
#define CUBE_FRAMEWORK_HEADER

#include <cube_rotations.h>
#include <cube_positions.h>

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template< cube_size N >
class Rubik
{
  static constexpr int Fsize = CPositions<N>::GetSize();
  CubeID * frameworkSpace;

public:
  
  // Constructors
  Rubik( void );
  Rubik( const Rubik<N>&, const Rubik<N>& );
  Rubik( const Rubik<N>& );
  Rubik( Rubik<N>&& f );
  
  // Operations
  Rubik<N> inverse    ( void ) const;
  void     rotate     ( const Axis, const Layer, const Turn turn = 1 );
  void     rotate     ( const RotID rotID, const RotStyle RS = normal );
  void     rotate     ( const RotID * rotIDs, const RotStyle RS = normal );
  void     shuffle    ( int depth = 0 );
  
  static Rubik<N> Transform  ( const Rubik<N>& A, const Rubik<N>& C ) { return Rubik<N>( A.inverse(), C ); } // transform( A, C ) returns with B where A + B = C
  
  // Operators
  bool      operator== ( const Rubik<N>& X ) const;
  Rubik<N>  operator+  ( const Rubik<N>& B ) { return Rubik<N> ( *this, B ); }
  Rubik<N>  operator-  ( const Rubik<N>& B ) { return Transform( B, *this ); }
  const Rubik<N>& operator=  ( const Rubik<N>& B ) ;
  // Destructor
  ~Rubik( );
  
  // Query functions
  CubeID  getCubeID    ( PosID id, RotID rot ) const { return frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ]; }
  CubeID  getCubeID    ( PosID id )            const { return frameworkSpace[id];                                   }
  OCube   getCube      ( PosID id )            const { return Simplex::GetCube( getCubeID ( id ) );                 }
  
  inline PosID whatIs    ( PosID id ) const ;
  inline PosID whereIs   ( PosID id ) const ;
  inline RotID transpose ( PosID id ) const ;

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
Rubik<N>::Rubik( )
 : frameworkSpace( new CubeID [ Fsize ] () )
{
  
}

// Conmposition of two cubes --> construct a new one
template< cube_size N >
Rubik<N>::Rubik( const Rubik<N> & cf1, const Rubik<N> & cf2 )
 : frameworkSpace( new CubeID [ Fsize ] )
{
  for ( PosID id = 0; id < Fsize; ++id )
  {
    const PosID position = cf2.whatIs( id );
    frameworkSpace[ id ] = Simplex::Composition( cf1.getCubeID( position ), cf2.getCubeID( id ) );
  }
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

// inverse
template< cube_size N > 
Rubik<N> Rubik<N>::inverse() const
{
  Rubik<N> inv;
  for ( PosID id = 0; id < Fsize; ++id )
  {
    const CubeID rotinv = Simplex::Inverse( frameworkSpace[ id ] );
    const int position = CPositions<N>::GetPosID( id, rotinv );
    inv.frameworkSpace[ position ] = rotinv;
  }
  return inv;
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
void Rubik<N>::rotate( const RotID rotID, const RotStyle RS )
{
  if ( RS == normal )
  {
    const Axis  axis  = CRotations<N>::GetAxis  ( rotID );
    const Layer layer = CRotations<N>::GetLayer ( rotID );
    const Turn  turn  = CRotations<N>::GetTurn  ( rotID );
    rotate( axis, layer, turn ); 
  }
  else // RS == extended
  {
    const Axis  axis  = CExtRotations<N>::GetAxis  ( rotID );
    const Layer layer = CExtRotations<N>::GetLayer ( rotID );
    const Turn  turn  = CExtRotations<N>::GetTurn  ( rotID );
    if ( layer < N )
    {
      rotate( axis, layer, turn );
    }
    else for ( Layer next = 0; next <= layer - ( N - 1 ); ++ next )
    {
      rotate( axis, next, turn );
    }
  }
}

template< cube_size N >
void Rubik<N>::rotate(const RotID* rotIDs, const RotStyle RS)
{
  clog( "\nRotations:" );
  for ( const RotID * P = rotIDs; *P != 0; ++ P )
  {
    rotate( *P, RS ); clog( RS == normal ? CRotations<N>::ToString( *P ) : CExtRotations<N>::ToString( *P ) );
  }
  NL();
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
RotID Rubik<N>::transpose( PosID id ) const
{
  CubeID rot = 0;
  while ( frameworkSpace[ CPositions<N>::GetPosID( id, rot ) ] != rot )
  {
    ++ rot;
  }
  return rot;
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
  const int    index  = CPositions<N>::GetPosID ( x, y, N - 1, inv );
  
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
