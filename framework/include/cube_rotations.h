#ifndef ___CUBE_ROTATIONS__H
#define ___CUBE_ROTATIONS__H

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

  static RotID GetInvRotID ( const RotID rotID );
  static RotID GetInvRotID ( const Axis A, const Layer L, const Turn T );
  static RotID GetRotID    ( const Axis A, const Layer L, const Turn T );
  static RotID GetRotID    ( const RotID R, const CubeID C ); 

  static Axis  GetAxis  ( const RotID rotID );
  static Layer GetLayer ( const RotID rotID );
  static Turn  GetTurn  ( const RotID rotID );
  static CubeID GetTilt ( const RotID rotID );

  static CubeID Tilt( const CubeID, const RotID );
  static void Transform( Axis & axis, Layer & layer, Turn & turn, const CubeID cubeID );

  static RotID Random();
  static std::string ToString( const Axis  );
  static std::string ToString( const RotID );
  static std::string ToString( const Axis, const Layer, const Turn );
  
  static void Print( const RotID, const int err = 0 );
  static bool Print( const Sequence & S );
};

// include template source
#include "../source/cube_rotations.cpp"

#endif  //  ! ___CUBE_ROTATIONS__H
