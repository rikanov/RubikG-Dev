/*
 * Oriented cube: desribe the rotational state of a (single) cube.
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

#ifndef ORIENTED_CUBE_HEADER
#define ORIENTED_CUBE_HEADER

#include <base_types.h>

// Oriented cube class, an auxiliary class for the sake of readable code. 
// It won't be used directly by algorithms but its mapping function is very useful for UI too.
// The basic idea: there are 24 different cube orientation, forming a non-Abelian group
// which is isomorph to P{4} permutation group. Mapping to integer: query the Right and Up
// Orients current values. ( 6 possible 'Right' Orients each of them has 4 possible 'Up' Orients )
// Their properties and group operation stored and handled in a singleton Simplex class

class OCube
{
  static const Orient FrontSides [ 6 /*Right*/ ][ 6 /*Up*/ ];

  Orient m_whereIs [7];
  Orient m_whatIs  [7];
  CubeID m_cubeID;

  std::string m_readable;

public:
  OCube( ) = default;
  void init ( Orient R, Orient U, CubeID groupID );

  Orient  whereIs   ( Orient ID ) const { return m_whereIs[ID]; }
  Orient  whatIs    ( Orient ID ) const { return m_whatIs[ID];  }
  CubeID  getCubeID (   void    ) const { return m_cubeID;      }

  const std::string& toString () const { return m_readable;         }
  char toChar (const size_t i )  const { return m_readable[i];      }
  operator        const char* () const { return m_readable.c_str(); }
  
  static Orient FrontSide ( Orient r, Orient u ) { return FrontSides[r][u]; }
};

#endif // !ORIENTED_CUBE_HEADER
