#include <rubik_api.h>


template< cube_size N >
OCube RubikAPI<N>::getCube( const PosID id ) const
{
  return Simplex::GetCube( RubikGroup<N>::getCubeID ( id ) );
}

template< cube_size N >
Orient RubikAPI<N>::getOrient( const Orient right, const Orient up, const Layer x, const Layer y ) const
{
  if ( Coaxial ( right, up ) || x < 0 || x >= N || y < 0 || y >= N ) // invalid setting
  {
    return _NF;
  }

  const CubeID trans  = Simplex::GetGroupID     ( right, up );
  const CubeID inv    = Simplex::Inverse        ( trans );
  const Orient orient = OCube::FrontSide        ( right, up ); // = Simplex::GetCube( trans ).whatIs( _F );
  const PosID  index  = CPositions<N>::GetPosID ( x, y, N - 1, inv );

  return getCube ( index ).whatIs( orient );
}

template< cube_size N >
void RubikAPI<N>::show( const Orient F ) const
{
  if ( _NF == F )
  {
    slog_( "", "  " );
  }
  else
  {
    clog_( colorOf( F ), FChar );
  }
}

template< cube_size N >
void RubikAPI<N>::show( const Orient right, const Orient up, const Layer x, const Layer y ) const
{
  const Orient F = getOrient( right, up, x, y );
  show( F );
}

template< cube_size N >
void RubikAPI<N>::print( const Orient right, const Orient up ) const
{
  NL();
  for ( Layer y = N - 1; y >= 0; --y )
  {
    for ( Layer x = 0; x < N; ++x )
    {
      show( right, up, x, y );
    }
    NL();
  }
  NL();
}

template< cube_size N >
void RubikAPI<N>::print( const bool separator ) const
{
  const int SideSize = separator ? N + 1 : N;
  // print UP side
  for ( Layer y = SideSize; y > 0; --y )
  {
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _R, _B, x, y - 1 );
    }
    NL();
  }
  // print middle sides Left - Front - Right - Back
  Orient orientations [] = { _F, _R, _B, _L };
  for ( Layer y = SideSize; y > 0; --y )
  {
    for ( Orient right: orientations )
    {
      for ( Layer x = 0; x < SideSize; ++x )
      {
        show( right, _U, x, y - 1 );
      }
    }
    NL();
  }
  // print DOWN side
  for ( Layer y = SideSize ; y > 0; --y )
  {
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _NF );
    }
    for ( Layer x = 0; x < SideSize; ++x )
    {
      show( _R, _F, x, y - 1 );
    }
    NL();
  }
  NL( Color::gray );
}
