#ifndef ACCEPTANCE__H
#define ACCEPTANCE__H

#include <cube_positions.h>
#include <functional>

using AcceptFunction = std::function< BitMap32ID( PosID ) >;
enum Axis;

template< cube_size N >
class Accept
{
  static Axis OnSide( PosID );

public:
  static AcceptFunction RotAxis ( const Axis axis );
  static BitMap32ID     Normal  ( PosID posID );
};

template< cube_size N >
Axis Accept<N>::OnSide( PosID posID )
{
  const Coord pos = CPositions<N>::GetCoord( posID );
  Axis result = _NA;
  int alignSide = 0;
  for ( Axis axis: { _X, _Y, _Z } )
  {
    if ( pos.coord( axis ) == 0 || pos.coord( axis ) == N - 1 )
    {
      ++ alignSide;
      result = axis;
    }
  }
  return 1 == alignSide ? result : _NA;
}


template< cube_size N >
AcceptFunction Accept<N>::RotAxis( const Axis axis )
{
  if ( _NA == axis )
    return []( PosID ) { return 1; };

  return [axis]( PosID ) -> BitMap32ID {
    BitMap32ID result = 0;
    for ( Turn turn: { 0, 1, 2, 3 } )
      result |= ( 1 << Simplex::Tilt( axis, turn ) );

    return result;
  };
}


template< cube_size N >
BitMap32ID Accept<N>::Normal( PosID posID )
{
  return RotAxis(  OnSide( posID ) )( posID );
}

#endif  //  ! ACCEPTANCE__H
