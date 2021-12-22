#include <cube_rotations.h>


template< cube_size N >
RotID CRotations<N>::GetInvRotID( const RotID rotID )
{
  return rotID + 2 - 2 * ( ( rotID - 1 ) % 3 );
}

template< cube_size N >
RotID CRotations<N>::GetInvRotID( const Axis A, const Layer L, const Turn T )
{
  return A * 3 * N + L * 3 + 4 - T;
}

template< cube_size N >
RotID CRotations<N>::GetRotID( const Axis A, const Layer L, const Turn T )
{
  return A * 3 * N + L * 3 + T;
}

template< cube_size N >
Axis CRotations<N>::GetAxis( const RotID rotID )
{
  return static_cast< Axis > ( ( rotID - 1 ) / ( 3 * N ) );
}

template< cube_size N >
Layer CRotations<N>::GetLayer( const RotID rotID )
{
  return ( ( rotID - 1 ) /3 ) % N;
}

template< cube_size N >
Turn CRotations<N>::GetTurn( const RotID rotID )
{
  return ( rotID - 1 ) % 3 + 1;
}

template< cube_size N >
CubeID CRotations<N>::GetTilt( const RotID rotID )
{
  return Simplex::Tilt( GetAxis( rotID ), GetTurn( rotID ) );
}

template< cube_size N >
void CRotations<N>::Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID )
{
  const Orient base  = GetAxisBase( axis );
  const Orient trans = Simplex::GetCube( cubeID ).whereIs( base );
  switch( trans )
  {
    case _L:
      axis = _X;
      break;

    case _D:
      axis = _Y;
      break;

    case _B:
      axis = _Z;
      break;

    case _R:
      axis  = _X;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    case _U:
      axis  = _Y;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    case _F:
      axis  = _Z;
      layer = N - 1 - layer;
      turn  = 4 - turn;
      break;

    default:
      clog( "invalid Orient to get axis" );
      return;
  }
}

template< cube_size N >
RotID CRotations<N>::GetRotID( const RotID rotID, const CubeID cubeID )
{
  RotID R = rotID - 1;
  Turn  turn  = R % 3 + 1;
  R /= 3;
  Layer layer = R % N;
  Axis  axis  = static_cast< Axis > ( R / N );
  Transform( axis, layer, turn, cubeID );
  return GetRotID( axis, layer, turn );
}

template< cube_size N >
CubeID CRotations<N>::Tilt( const CubeID cubeID, const RotID rotID  )
{
  return Simplex::Tilt( cubeID, GetAxis( rotID ), GetTurn( rotID ) );
}

template< cube_size N >
RotID CRotations<N>::Random()
{
 static std::random_device                 randomDevice;
 static std::default_random_engine         randomEngine( randomDevice() );
 static std::uniform_int_distribution<int> distribution( 1, AllRotIDs - 1 );

  return distribution( randomEngine );
}

template< cube_size N >
std::string CRotations<N>::ToString( const Axis A )
{
  switch ( A )
  {
    case _X:
      return "X";
    case _Y:
      return "Y";
    case _Z:
      return "Z";
    default:
      return "#ERR";
  }
}

template< cube_size N >
std::string CRotations<N>::ToString( const Axis axis, const Layer layer, const Turn turn )
{
  return std::string( "{ _" + ToString ( axis ) + ", " + std::to_string( layer ) + ", " + std::to_string( turn ) + " }" );
}


template< cube_size N >
std::string CRotations<N>::ToString( const RotID rotID )
{
  return ToString ( GetAxis ( rotID ), GetLayer ( rotID ), GetTurn ( rotID ) );
}

template< cube_size N >
void CRotations<N>::Print( const RotID rotID, const int err )
{
  switch ( CRotations<N>::GetAxis( rotID ) )
  {
    case _X:
      clog_ ( Color::yellow );
      break;
    case _Y:
      clog_ ( Color::blue );
      break;
    case _Z:
      clog_ ( Color::red );
      break;
    default:
      clog_( Color::flash, "Error: rot id =", (int) rotID ); // error
  }
  clog_( CRotations<N>::ToString( rotID ), Color::off );
  switch ( err )
  {
    case 1:
      clog_( Color::red, " ERROR: repeating rotation", Color::off );
      break;
    case 2:
      clog_( Color::red, " ERROR: axis & layer equality", Color::off );
      break;
    case 3:
      clog_( Color::red, " ERROR: unnecessary transposition", Color::off );
      break;
    default:
      break;
  }
  NL();
}

template< cube_size N >
bool CRotations<N>::Print( const Sequence & S )
{
  bool result = true;
  size_t c1 = 0, c2 = 0;
  clog( "steps:", S.size() );
  for( auto P = S.begin(); P != S.end(); ++ P )
  {
    if ( S.end() - 1 == P )
    {
      Print( *P );
      continue;
    }
    const RotID next = *( P + 1 );
    if ( *P == next )
    {
      Print( *P, 1 );
      result = false;
      continue;
    }
    if ( GetAxis( *P ) == GetAxis( next ) && GetLayer( *P ) == GetLayer( next ) )  // Axes and Layers equal
    {
      Print( *P, 2 );
      result = false;
      continue;
    }
    if ( GetAxis( *P ) == GetAxis( next ) )
    {
      ++ c1;
      c2 = GetTurn( *P ) == GetTurn( next ) ? c2 + 1 : 0;
    }
    else
    {
      c1 = 0;
      c2 = 0;
    }
    if ( c1 > 0 && ( N - 1 == c1 || N - 2 == c2 ) )
    {
      Print( *P, 3 );
      result = false;
      continue;
    }
    Print( *P );
  }
  return result;
}

