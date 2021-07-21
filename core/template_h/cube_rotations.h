#ifndef CUBE_ROTATIONS__H
#define CUBE_ROTATIONS__H

#include <simplex.h>
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

static void permute3( BitMapID & rotationSet, const uint8_t length, const uint8_t c, const uint8_t b, const uint8_t a )
{
  const BitMapID mask = ( 1ULL << length ) - 1;
  const BitMapID A = ( rotationSet               ) & mask;
  const BitMapID B = ( rotationSet >>     length ) & mask;
  const BitMapID C = ( rotationSet >> 2 * length ) & mask;
  rotationSet =  A << ( a * length );
  rotationSet += B << ( b * length );
  rotationSet += C << ( c * length );
}

static void reverseSequence( BitMapID & rotationSet, int a, int b )
{
  for( ; a - b > 1; --a, ++b )
  {
    if( rotationSet & ( 1ULL << a ) != rotationSet & ( 1ULL << b ) )
    {
      rotationSet ^= 1ULL << a;
      rotationSet ^= 1ULL << b;
    }
  }
}

enum RotStyle
{
  normal,
  extended
};

static Orient GetBaseOrient( Axis axis )
{
  switch( axis )
  {
    case _X:
      return _L;
    case _Y:
      return _D;
    case _Z:
      return _B;
    default:
      return _NF;
  }
}

template< cube_size N >
class CRotations
{

public:
  static constexpr size_t AllRotIDs = 3 * N * 3 + 1;

  static RotID GetInvRotID ( const RotID rotID )                          { return rotID + 2 - 2 * ( ( rotID - 1 ) % 3 ); }
  static RotID GetInvRotID ( const Axis A, const Layer L, const Turn T )  { return A * 3 * N + L * 3 + 4 - T;             }
  static RotID GetRotID    ( const Axis A, const Layer L, const Turn T )  { return A * 3 * N + L * 3 + T ;                }
  static RotID GetRotID    ( const RotID R, const CubeID C ); 

  static Axis  GetAxis  ( const RotID rotID )  { return static_cast< Axis > ( ( rotID - 1 ) / ( 3 * N ) ); }
  static Layer GetLayer ( const RotID rotID )  { return ( ( rotID - 1 ) /3 ) % N;                          }
  static Turn  GetTurn  ( const RotID rotID )  { return ( rotID - 1 ) % 3 + 1;                             }

  static void Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID );

  static RotID Random();
  static std::string ToString( const Axis  );
  static std::string ToString( const RotID );
  static std::string ToString( const Axis, const Layer, const Turn );
};

template< cube_size N >
void CRotations<N>::Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID )
{
  const Orient base  = GetBaseOrient( axis );
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
class CExtRotations
{
public:

  static constexpr size_t NT = N > 3 ? 2 * N - 3 : N;
  static constexpr size_t AllRotIDs = CRotations<NT>::AllRotIDs;

  static void Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID );
  
  static std::string ToString( const RotID rotID )
  {
    return CRotations<NT>::ToString( rotID );
  }

  static std::string ToString( const Axis axis, const Layer layer, const Turn turn )
  {
    return CRotations<NT>::ToString( axis, layer, turn );
  }

  static RotID GetRotID( const Axis axis, const Layer layer, const Turn turn )
  {
    return CRotations<NT>::GetRotID( axis, layer, turn );
  }

  static RotID GetInvRotID( const Axis axis, const Layer layer, const Turn turn )
  {
    return CRotations<NT>::GetInvRotID( axis, layer, turn );
  }

  static RotID GetInvRotID( const RotID rotID )
  {
    return CRotations<NT>::GetInvRotID( rotID );
  }

  static RotID GetRotID( Axis axis, Layer layer, Turn turn, const CubeID cubeID )
  {
    Transform( axis, layer, turn, cubeID );
    return GetRotID( axis, layer, turn );
  }

  static RotID GetRotID( const RotID rotID, const CubeID cubeID )
  {
    Axis  axis  = GetAxis ( rotID );
    Layer layer = GetLayer( rotID );
    Turn  turn  = GetTurn ( rotID );
    return GetRotID( axis, layer, turn, cubeID );
  }

  static Axis GetAxis( const RotID rotID )
  {
    return CRotations<NT>::GetAxis( rotID );
  }

  static Layer GetLayer( const RotID rotID )
  {
    return CRotations<NT>::GetLayer( rotID );
  }

  static Turn GetTurn( const RotID rotID )
  {
    return CRotations<NT>::GetTurn( rotID );
  }

};

template< cube_size N >
void CExtRotations<N>::Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID )
{
  if ( layer < N )
  {
    CRotations<N>::Transform( axis, layer, turn, cubeID );
    return;
  }

  const Orient base  = GetBaseOrient( axis );
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
      layer = 3 * N - 4 - layer;
      break;

    case _U:
      axis  = _Y;
      layer = 3 * N - 4 - layer;
      break;

    case _F:
      axis  = _Z;
      layer = 3 * N - 4 - layer;
      break;

    default:
      clog( "invalid Orient to get axis" );
      return;
  }
}

#endif // ! CUBE_ROTATIONS__H
