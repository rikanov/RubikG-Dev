#ifndef ACCEPTANCE__H
#define ACCEPTANCE__H

#include <cube_positions.h>
#include <functional>

using AcceptFunction = std::function< BitMap32ID( PosID ) >;


template< cube_size N >
class Accept
{
  static Axis OnSide( PosID );

public:
  static AcceptFunction RotAxis ( const Axis axis );
  static BitMap32ID     Normal  ( PosID posID );
  static BitMap32ID     OnPlace ( PosID posID );

  static bool EqualOn( const Pattern<N> &, AcceptFunction, AcceptFunction );
  static bool StricterOn( const Pattern<N> &, AcceptFunction, AcceptFunction );
};

// include template source file
#include "acceptance.cpp"

#endif  //  ! ACCEPTANCE__H
