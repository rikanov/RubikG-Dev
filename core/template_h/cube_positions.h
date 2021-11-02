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

#ifndef CUBE_POSITIONS_H
#define CUBE_POSITIONS_H

#include <text_output.h>
#include <simplex.h>
#include <random>

static constexpr bool SHOW_LOG = false; // setting true is only for debuging purposes

/// ----------------------------------- Template declarations starts here ------------------------------------- ///
template< cube_size N >
class CPositions
{
  static constexpr size_t FrameworkSize [] = { 0, 1, 8 - 0, 27 - 1, 64 - 8, 125 - 27, 216 - 64, 341 - 125, 512 - 216, 721 - 341, 1000 - 512 }; //  N > 1 :  N ^ 3 - ( N - 2 ) ^ 3
  static CPositions * Singleton; 
  
  PosID  m_routerPositions [ FrameworkSize [ N ] ][ 24 ] = {};
  PosID  m_frameworkLayer  [ 3 ] [ N ] [ N * N ]         = {}; // [axis] [layer index] [cubes] 
  PosID  m_coordToIndex    [ N ] [ N ] [ N ]             = {};
  Layer  m_indexToCoord    [ FrameworkSize [ N ] ][ 3 ]  = {}; // [ PosID ] [ Axis ]
  
  CPositions();
  
  Coord rotate ( Layer x, Layer y, Layer z, CubeID rot ); 
  Coord rotate ( const Coord C, CubeID rot )  
  { 
    return rotate( C.x, C.y, C.z, rot ); 
  }
  
  void  initPositions();
  void  initIndices  ();
  
  static PosID* GetNode  ( CubeID id )             { return Singleton->m_routerPositions[ id ]; }
  static PosID* GetNode  ( int x, int y, int z )   { return GetNode( GetPosID ( x, y, z) );   }

public:
  static constexpr size_t GetSize ( ) { return FrameworkSize [ N ]; }
  
  static   void    Instance  ( void )                                  { if ( Singleton == nullptr ) new CPositions<N>;          }
  static   void    OnExit    ( void )                                  { delete Singleton; Singleton = nullptr;                  }
  static   bool    ValID     ( PosID id )                              { return 0 <= id && id < GetSize();                       }
  static   PosID   GetPosID  ( PosID id, CubeID rot )                  { return Singleton -> m_routerPositions[ id ][ rot ];     }
  static   PosID   GetPosID  ( const Coord & C )                       { return GetPosID( C.x, C.y, C.z);                        }
  static   PosID   GetPosID  ( Layer x, Layer y, Layer z )             { return Singleton -> m_coordToIndex[ x ][ y ][ z ];      }
  static   PosID   GetPosID  ( Layer x, Layer y, Layer z, CubeID rot ) { return GetNode( x, y, z ) [ rot ];                      }
  static   PosID   GetLayer  ( Axis a, Layer l, byte id )              { return Singleton -> m_frameworkLayer [ a ][ l ][ id ];  }
  static   Layer   LayerSize ( Layer l )                               { return l == 0 || l == N - 1 ? N * N : 4 * ( N - 1 );    }
  static   Layer   GetLayer  ( PosID p, Axis a )                       { return Singleton -> m_indexToCoord [ p ][ a ];          }
  static   Layer   GetLayer  ( PosID p, CubeID r, Axis a )             { return Singleton -> m_indexToCoord [ GetPosID( p, r ) ][ a ]; }
  static   Coord   GetCoord  ( PosID );
  static   PosID   Random    ();
  static BitMapID  ActOn     ( const PosID posID );
};

/// ----------------------------------- Template definitions starts here ------------------------------------- ///
template< cube_size N >
CPositions<N> * CPositions<N>::Singleton = nullptr;

template< cube_size N >
CPositions<N>:: CPositions( )
{
  Singleton = this;
  initPositions();
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

template< cube_size N >
BitMapID CPositions<N>::ActOn( const PosID posID )
{
  const Coord pos = CPositions<N>::GetCoord( posID );
  return ( 14ULL << ( 3 * pos.x ) ) | ( 14ULL << ( 3 * N + 3 * pos.y ) ) | ( 14ULL << ( 6 * N + 3 * pos.z ) );
}

template< cube_size N >
void CPositions<N>::initIndices()
{
  int index = 0,
  idX [N] = {}, idY [N] = {}, idZ [N] = {}; // slice indices
  for_vector ( x, y, z, N )
  {
    const bool innerCube = x != 0 && x != N - 1 && y != 0 && y != N - 1 && z != 0 && z != N - 1 ;
    if ( !innerCube )
    {
      m_frameworkLayer [ _X ][ x ][ idX[x] ++ ] = index;
      m_frameworkLayer [ _Y ][ y ][ idY[y] ++ ] = index;
      m_frameworkLayer [ _Z ][ z ][ idZ[z] ++ ] = index;
      
      m_indexToCoord [ index ][ _X ] = x;
      m_indexToCoord [ index ][ _Y ] = y;
      m_indexToCoord [ index ][ _Z ] = z;
      
      m_coordToIndex [ x ][ y ][ z ] = index++;
    }
    else
    {
      m_coordToIndex[ x ][ y ][ z ] = 0xFF;
    }
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
      if ( SHOW_LOG ) clog ( '\n', N, 'X', N,"  ... ", (int)x, (int)y, (int)z, "\t  ", (int) GetPosID( x, y, z) );
      all_cubeid ( id )
      {
        const Coord C = rotate ( x, y, z, id );
        if ( SHOW_LOG ) clog ( Simplex::GetCube ( id ).toString(), " --> ", C.toString() , "\t| ", (int) GetPosID( C ) );
        m_routerPositions[ GetPosID( x, y, z ) ][ id ] = GetPosID( C );
      }
    }
  }
}

template< cube_size N >
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
 static std::uniform_int_distribution<int> distribution( 0, FrameworkSize[ N ] - 1 ); 

 return distribution( randomEngine );
}
#endif // CUBE_POSITIONS_H
