#include <acceptance.h>


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

template< cube_size N > inline
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

template< cube_size N > inline
BitMap32ID Accept<N>::Normal( PosID posID )
{
  return RotAxis(  OnSide( posID ) )( posID );
}

template< cube_size N > inline
BitMap32ID Accept<N>::OnPlace( PosID posID )
{
  BitMap32ID result = 0;
  all_cubeid ( cid )
  {
    if ( CPositions<N>::GetPosID( posID, cid ) == posID )
    {
      result |= ( 1 << cid );
    }
  }
  return result;
}

template< cube_size N >
bool Accept<N>::EqualOn(const Pattern<N> & pattern, AcceptFunction a1, AcceptFunction a2 )
{
  for ( auto posID: pattern )
  {
    if ( a1( posID ) != a2( posID ) )
    {
      return false;
    }
  }
  return true;
}

template< cube_size N >
bool Accept<N>::StricterOn( const Pattern<N> & pattern, AcceptFunction a1, AcceptFunction a2 )
{
  for ( auto posID: pattern )
  {
    if ( ( a1( posID ) & a2( posID ) ) != a1( posID ) )
    {
      return false;
    }
  }
  return true;
}
