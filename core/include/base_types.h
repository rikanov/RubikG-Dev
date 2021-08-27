/*
 * Simple, typed IDs of the Rubik's Cubes. Axis-Layer-Turn (A-L-T) to describe layer rotations. CubeID, PosID, RotID to store spatial operations 
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

#ifndef BASE_TYPES__HEADER
#define BASE_TYPES__HEADER

#include <cstdint>
#include <def_colors.h>
#include <string>

typedef uint8_t byte;

 // ID types
//  --------
typedef uint8_t       cube_size;
typedef uint_fast8_t  CubeID;
typedef uint8_t       RotID;
typedef uint8_t       PosID;
typedef uint8_t       DistID;
typedef uint32_t      CacheID;
typedef uint64_t      Cache64ID;
typedef uint32_t      BitMap32ID;
typedef uint64_t      BitMapID;

using SubSpace =      const std::initializer_list <PosID>;

 // Components
//  ----------
enum Axis 
{
  _X, _Y, _Z, _NA
};

typedef uint8_t Layer;
typedef uint8_t Turn;

struct Coord 
{
  Layer x ;
  Layer y ;
  Layer z ; 

  Coord(): x( 0 ), y( 0 ), z( 0 )
  {}
  Coord( Layer x, Layer y, Layer z): x( x ), y( y ), z( z ) 
  {}  
  std::string toString() const 
  { 
    return "< " + std::to_string( x ) + ' ' + std::to_string( y ) + ' ' + std::to_string( z ) + " >";
  }
};

 // Orientations
//  ------------
enum Orient
{
  _F, _R, _U, _D, _L, _B, _NF
};

 // Auxiliary inline functions
// ---------------------------
inline size_t pow24( const int pow )
{
  static constexpr size_t _pow24[] = { 1, 24, 576, 13824, 331776, 7962624, 191102976 };
  return _pow24[ pow ];
}

inline void operator++ ( Orient& id )
{
  id = Orient( int( id ) + 1 );
}

inline Orient SideOpposite( Orient S )
{
  return (S == _NF) ? _NF : Orient( 5 - S );
}

inline bool Coaxial( Orient a, Orient b )
{ 
  return  a == b || a == SideOpposite( b ); 
}

inline Color::Modifier colorOf( Orient F )
{
  switch ( F )
  {
    case _F:
      return Color::red;
      
    case _R:
      return Color::yellow;
      
    case _U:
      return Color::blue;
      
    case _L:
      return Color::white;
      
    case _D:
      return Color::green;
      
    case _B:
      return Color::light;
      
    default:
      return Color::black;
  }
}

inline char Token( Orient F ) 
{ 
  static const char Tokens [7] = "FRUDLB";
  return Tokens[ F ];
}

 // Auxiliary macros
// -----------------
#define all_orient(i) for( Orient i = _F; i <= _B; ++i )
#define all_cubeid(i) for( CubeID i = 0 ; i < 24 ; ++i )

#define all_layer( A, L, N )     \
   for ( Axis A: { _X, _Y, _Z } ) \
     for ( Layer L = 0; L < N; ++L )

#define all_rot( A, L, T, N )      \
   for ( Axis A: { _X, _Y, _Z } )   \
     for ( Layer L = 0; L < N; ++L ) \
       for ( Turn T: { 1, 2, 3 } )
         
#define all_rotid ( rotID ) \
   for ( RotID rotID = 0; rotID < CRotations<N>::AllRotIDs; ++ rotID )

#define for_vector( x, y, z, N )   \
   for ( Layer x = 0; x < N; ++x )  \
     for ( Layer y = 0; y < N; ++y ) \
        for ( Layer z = 0; z < N; ++z )

#endif
