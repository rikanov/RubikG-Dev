/*
 * Cube positions: this singleton class creates a graph to determine cublet positions in an N x N Rubiks cube according to the rotation state of the cubes so each node has 24 branches
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

#ifndef ___CUBE_POSITIONS_H
#define ___CUBE_POSITIONS_H

#include <base_types.h>

static constexpr bool SHOW_LOG = false; // setting true is only for debuging purposes

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template< cube_size N >
class CPositions
{
  static constexpr size_t FrameworkSize [] = { 0, 1, 8 - 0, 27 - 1, 64 - 8, 125 - 27, 216 - 64, 341 - 125, 512 - 216, 721 - 341, 1000 - 512 }; //  N > 1 :  N ^ 3 - ( N - 2 ) ^ 3
public:
  static constexpr size_t AllPosIDs = FrameworkSize[ N ];
protected:
  static CPositions * Singleton; 
  
  PosID  m_routerPositions [ AllPosIDs ][ 24 ] = {};
  PosID  m_coordToIndex    [ N ] [ N ] [ N ]             = {};
  Layer  m_indexToCoord    [ AllPosIDs ][ 3 ]  = {}; // [ PosID ] [ Axis ]
  
  CPositions();
  
  Coord rotate ( Layer x, Layer y, Layer z, CubeID rot ); 
  Coord rotate ( const Coord C, CubeID rot )  
  { 
    return rotate( C.x, C.y, C.z, rot ); 
  }
  
  void  initPositions();
  void  initIndices  ();
  
  static PosID * GetNode  ( CubeID id );
  static PosID * GetNode  ( Layer x, Layer y, Layer z );

public:
  static constexpr size_t GetSize ( ) { return AllPosIDs; }
  
  static   void    Instance  ( void );
  static   void    OnExit    ( void );
  static   bool    ValID     ( PosID id );
  static   PosID   GetPosID  ( PosID id, CubeID rot );
  static   PosID   GetPosID  ( const Coord & C );
  static   PosID   GetPosID  ( Layer x, Layer y, Layer z );
  static   PosID   GetPosID  ( Layer x, Layer y, Layer z, CubeID rot );
  static   PosID   GetLayer  ( Axis a, Layer l, size_t id );
  static   Layer   GetLayer  ( PosID p, Axis a );
  static   Layer   GetLayer  ( PosID p, CubeID r, Axis a );
  static   Coord   GetCoord  ( PosID );
  static   PosID   Random    ();
  static BitMapID  ActOn     ( const PosID posID );
};

// include template source
#include "../source/cube_positions.cpp"

#endif // ___CUBE_POSITIONS_H
