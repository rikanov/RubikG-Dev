#include <rubik.h>


 // Printer
//  -------

template< cube_size N >
void Rubik<N>::show( Orient F ) const
{
  std::cout << colorOf( F ) << FChar << ' ';
}

template< cube_size N >
void Rubik<N>::show( Orient right, Orient up, Layer x, Layer y ) const
{
  const Orient F = getOrient( right, up, x, y );
  show( F );
}

template< cube_size N >
void Rubik<N>::print( Orient right, Orient up ) const
{
  std::cout << std::endl;
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
void Rubik<N>::print( bool separator ) const
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
