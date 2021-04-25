/*
 * In brief: simple auxiliary template functions to handle basic rotation IDs in order to reach lower memory usage
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

#ifndef CUBE_ROTATIONS__H
#define CUBE_ROTATIONS__H

#include <simplex.h>
#include <random>

// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [ 0..2 ]
// Layer --> index of a single turning layer [ 0..N ) or the last layer of a slice [ 0..N-1 )
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 0 --> no rotation
// ID = 3 * N * Axis + 3 * Layer + Turn 
// ID < 3 * N * 3

static inline Orient GetBaseOrient( Axis axis )
{
  switch( axis )
  {
    case _X:
      return _L;
    case _Y:
      return _D;
    case _Z:
      return _B;
    default:
      return _NF;
  }
}

template< size_t N >
class CRotations
{

public:
  static constexpr size_t AllRotIDs = 3 * N * 3 + 1;

private:
  static CRotations<N> * Singleton;

  std::random_device                 m_randomDevice;
  std::default_random_engine         m_randomEngine;
  std::uniform_int_distribution<int> m_distribution; 

  RotID  m_rotID  [ 3 /*axes*/][ N /*layers*/ ][ 4 /*turns*/ ] = {};
  RotID  m_tRotID [ AllRotIDs ][ 24 ] = {};
  Axis   m_axis   [ AllRotIDs ]       = {};
  Layer  m_layer  [ AllRotIDs ]       = {};
  Turn   m_turn   [ AllRotIDs ]       = {};
  
  CRotations()
    : m_randomEngine( m_randomDevice() )
    , m_distribution( 1, (int) 3 * N * 3 )
  {}

  void  init();
  void  transformRotIDs();

  RotID   getRotID ( RotID R, CubeID C )                 const  { return m_tRotID[R][C];                    }
  RotID   getRotID ( Axis A, Layer L, Turn T )           const  { return m_rotID [A][L][T];                 }
  RotID   getRotID ( Axis A, Layer L, Turn T, CubeID C ) const  { return getRotID( getRotID( A, L, T), C ); }

  Axis    getAxis  ( RotID rotID ) const  { return m_axis  [ rotID ]; }
  Layer   getLayer ( RotID rotID ) const  { return m_layer [ rotID ]; }
  Turn    getTurn  ( RotID rotID ) const  { return m_turn  [ rotID ]; }
  
  public:

  static void Instance();
  static void OnExit();

  static RotID GetInvRotID ( Axis A, Layer L, Turn T )           { return Singleton -> getRotID( A, L, 4 - T ); }
  static RotID GetRotID    ( Axis A, Layer L, Turn T )           { return Singleton -> getRotID( A, L, T );     }
  static RotID GetRotID    ( Axis A, Layer L, Turn T, CubeID C ) { return Singleton -> getRotID( A, L, T, C );  }
  static RotID GetRotID    ( RotID R, CubeID C )                 { return Singleton -> getRotID( R, C );        }

  static Axis  GetAxis  ( RotID rotID )  { return Singleton -> getAxis  ( rotID ); }
  static Layer GetLayer ( RotID rotID )  { return Singleton -> getLayer ( rotID ); }
  static Turn  GetTurn  ( RotID rotID )  { return Singleton -> getTurn  ( rotID ); }

  static void Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID );

  static RotID Random();
  static std::string ToString( Axis  );
  static std::string ToString( RotID );
  static std::string ToString( Axis, Layer, Turn );
};
template< size_t N >
CRotations<N> * CRotations<N>::Singleton = nullptr;

template< size_t N >
void CRotations<N>::Instance()
{
  if ( nullptr == Singleton )
  {
    Singleton = new CRotations<N>();
    Singleton -> init();
  }
}

template< size_t N >
void CRotations<N>::OnExit()
{
  delete Singleton;
  Singleton = nullptr;
}

template< size_t N >
void CRotations<N>::init()
{
  RotID rotID = 0;
  all_rot( axis, layer, turn, N )
  {
    m_rotID [axis][layer][turn] = ++ rotID;
    m_axis  [rotID] = axis;
    m_layer [rotID] = layer;
    m_turn  [rotID] = turn;
    transformRotIDs();
  }
}

template< size_t N >
void CRotations<N>::Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID )
{
  const Orient base  = GetBaseOrient( axis );
  const Orient trans = Simplex::GetCube( cubeID ).whereIs( base );
  switch( trans )
  {
    case _L:
      axis = _X;
      break;

    case _D:
      axis = _Y;
      break;

    case _B:
      axis = _Z;
      break;

    case _R:
      axis  = _X;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    case _U:
      axis  = _Y;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    case _F:
      axis  = _Z;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    default:
      clog( "invalid Orient to get axis" );
      return;
  }
}

template< size_t N >
void CRotations<N>::transformRotIDs()
{
  all_rot( axis, layer, turn, N )
  {
    const RotID  rotID = GetRotID( axis, layer, turn);
    const Orient base  = GetBaseOrient( axis );
    all_cubeid( cubeID )
    {
      Orient trans = Simplex::GetCube( cubeID ).whereIs( base );
      Axis  tAxis  = axis;
      Layer tLayer = layer;
      Turn  tTurn  = turn;
      Transform( axis, layer, turn, cubeID );
      m_tRotID[ rotID ][ cubeID ] = GetRotID( tAxis, tLayer, tTurn );
    }
  }
}

template< size_t N >
RotID CRotations<N>::Random()
{
  return CRotations<N>::Singleton -> m_distribution( CRotations<N>::Singleton -> m_randomEngine );
}

template< size_t N >
std::string CRotations<N>::ToString( Axis A )
{
  switch ( A )
  {
    case _X:
      return "X";
    case _Y:
      return "Y";
    case _Z:
      return "Z";
    default:
      return "#ERR";
  }
}

template< size_t N >
std::string CRotations<N>::ToString( Axis axis, Layer layer, Turn turn )
{
  return std::string( "{ _" + ToString ( axis ) + ", " + std::to_string( layer ) + ", " + std::to_string( turn ) + " }" );
}


template< size_t N >
std::string CRotations<N>::ToString( RotID rotID )  
{
  return ToString ( GetAxis ( rotID ), GetLayer ( rotID ), GetTurn ( rotID ) );
}

template< size_t N >
class CExtRotations
{
public:

  static constexpr size_t NT = N > 3 ? 2 * N - 3 : N;
  static constexpr size_t AllRotIDs = CRotations<NT>::AllRotIDs;

  static void Instance()
  {
    CRotations<NT>::Instance();
  }

  static void Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID )
  {
    if ( layer < N )
    {
      CRotations<N>::Transform( axis, layer, turn, cubeID );
      return;
    }

    const Orient base  = GetBaseOrient( axis );
    const Orient trans = Simplex::GetCube( cubeID ).whereIs( base );

    switch( trans )
    {
      case _L:
        axis = _X;
        break;

      case _D:
        axis = _Y;
        break;

      case _B:
        axis = _Z;
        break;

      case _R:
        axis  = _X;
        layer = 3 * N - 4 - layer;
        break;

      case _U:
        axis  = _Y;
        layer = 3 * N - 4 - layer;
        break;

      case _F:
        axis  = _Z;
        layer = 3 * N - 4 - layer;
        break;

      default:
        clog( "invalid Orient to get axis" );
        return;
    }
  }
  
  static std::string ToString( const RotID rotID )
  {
    return CRotations<NT>::ToString( rotID );
  }

  static std::string ToString( const Axis axis, const Layer layer, const Turn turn )
  {
    return CRotations<NT>::ToString( axis, layer, turn );
  }

  static RotID GetRotID( Axis axis, Layer layer, Turn turn )
  {
    return CRotations<NT>::GetRotID( axis, layer, turn );
  }

  static RotID GetRotID( Axis axis, Layer layer, Turn turn, const CubeID cubeID )
  {
    Transform( axis, layer, turn, cubeID );
    return GetRotID( axis, layer, turn );
  }

  static RotID GetRotID( const RotID rotID, const CubeID cubeID )
  {
    Axis  axis  = GetAxis ( rotID );
    Layer layer = GetLayer( rotID );
    Turn  turn  = GetTurn ( rotID );
    return GetRotID( axis, layer, turn, cubeID );
  }

  static Axis GetAxis( const RotID rotID )
  {
    return CRotations<NT>::GetAxis( rotID );
  }

  static Layer GetLayer( const RotID rotID )
  {
    return CRotations<NT>::GetLayer( rotID );
  }

  static Turn GetTurn( const RotID rotID )
  {
    return CRotations<NT>::GetTurn( rotID );
  }

};

#endif // ! CUBE_ROTATIONS__H
