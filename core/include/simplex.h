/*
 * Simplex: to handle (a single) cube rotation group and mapping them to integer (CubeID) values to make algorithms efficient
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

#ifndef SIMPLEX_HEADER
#define SIMPLEX_HEADER

#include <oriented_cube.h>

class Simplex
{
    static Simplex * Singleton;

private: 
  // functions
  Simplex();
  void init();
  void initGroup();

private: 
  // array datamembers
  OCube   m_simplexGroup     [ 24 /* ID */ ];
  CubeID  m_tiltCube         [ 24 ] [ 3 ] [ 4 ] = {};
  CubeID  m_tilt             [  3 ] [ 4 ]       = {};
  CubeID  m_simplexGroupID   [ 6 /*Right*/ ][ 6 /*Up*/ ];
  CubeID  m_composition      [ 24 /* A */  ][ 24 /*B*/ ]; // X = AB
  CubeID  m_inverse          [ 24 ];

  const OCube & getCube     ( CubeID ID )                    const  { return m_simplexGroup [ID];                      }
  const OCube & getCube     ( Orient r, Orient u )           const  { return m_simplexGroup[ m_simplexGroupID[r][u] ]; }
  CubeID        getGroupID  ( Orient r, Orient u )           const  { return m_simplexGroupID [r][u];                  }
  CubeID        composition ( CubeID a, CubeID b )           const  { return m_composition    [a][b];                  }
  CubeID        tilt        ( Axis a, Turn t = 1 )           const  { return m_tilt           [a][t];                  }
  CubeID        tilt        ( CubeID c, Axis a, Turn t = 1 ) const  { return m_tiltCube    [c][a][t];                  }
  CubeID        inverse     ( CubeID a )                     const  { return m_inverse        [a];                     }
  CubeID        random      ( )                              const  ;

public:
  // create a singleton object
  static void            Instance    ( )                              { if ( Singleton == nullptr ) new Simplex;  }
  static void            OnExit      ( )                              { delete Singleton; Singleton = nullptr;    }
  static const OCube &   GetCube     ( CubeID ID )                    { return Singleton -> getCube     ( ID );   }
  static const OCube &   GetCube     ( Orient r, Orient u )           { return Singleton -> getCube     ( r, u ); }
  static CubeID          GetGroupID  ( Orient r, Orient u )           { return Singleton -> getGroupID  ( r, u ); }
  static CubeID          Composition ( CubeID a, CubeID b )           { return Singleton -> composition ( a, b ); }
  static CubeID          Tilt        ( Axis a, Turn t = 1 )           { return Singleton -> tilt        ( a, t ); }
  static CubeID          Tilt        ( CubeID c, Axis a, Turn t = 1 ) { return Singleton -> tilt     ( c, a, t ); }
  static CubeID          Inverse     ( CubeID a )                     { return Singleton -> inverse     ( a );    }
  static CubeID          Random      ( )                              { return Singleton -> random();             }
};

#endif //SIMPLEX_HEADER
