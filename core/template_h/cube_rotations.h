#ifndef CUBE_ROTATIONS__H
#define CUBE_ROTATIONS__H

#include <simplex.h>
#include <sequence.h>
#include <random>

// Mapping between all the basic rotations and the set of one-byte size IDs:
// Axis  --> X,Y or Z represented by integers [ 0..2 ]
// Layer --> index of a single turning layer [ 0..N ) or the last layer of a slice [ 0..N-1 )
// Turn  --> 1: a simple clockwise turn  2: double turn 3: tripple (or inverse) turn
//
// to get ID we use a mixed radix system:
// ID = 0 --> no rotation
// ID = 3 * N * Axis + 3 * Layer + Turn 
// ID < 3 * N * 3

template< cube_size N >
class CRotations
{

public:
  static constexpr size_t AllRotIDs = 3 * N * 3 + 1;

  static RotID GetInvRotID ( const RotID rotID )                          { return rotID + 2 - 2 * ( ( rotID - 1 ) % 3 ); }
  static RotID GetInvRotID ( const Axis A, const Layer L, const Turn T )  { return A * 3 * N + L * 3 + 4 - T;             }
  static RotID GetRotID    ( const Axis A, const Layer L, const Turn T )  { return A * 3 * N + L * 3 + T ;                }
  static RotID GetRotID    ( const RotID R, const CubeID C ); 

  static Axis  GetAxis  ( const RotID rotID )  { return static_cast< Axis > ( ( rotID - 1 ) / ( 3 * N ) );   }
  static Layer GetLayer ( const RotID rotID )  { return ( ( rotID - 1 ) /3 ) % N;                            }
  static Turn  GetTurn  ( const RotID rotID )  { return ( rotID - 1 ) % 3 + 1;                               }
  static CubeID GetTilt ( const RotID rotID )  { return Simplex::Tilt( GetAxis( rotID ), GetTurn( rotID ) ); }

  static CubeID   Tilt  ( const CubeID, const RotID );
  static void Transform ( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID );

  static RotID Random();
  static std::string ToString( const Axis  );
  static std::string ToString( const RotID );
  static std::string ToString( const Axis, const Layer, const Turn );
  
  static void Print( const RotID, const int err = 0 );
  static bool Print( const Sequence & S );
};

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

#endif  //  ! CUBE_ROTATIONS__H
