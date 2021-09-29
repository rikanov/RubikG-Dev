#include <test.h>
#include <bitmap_set.h>
#include <cube_rotations.h>

template< cube_size N >
static bool testRotationActing( const int testNumber )
{
  bool result =true;
  const std::string tCase = std::string( "Rotations acting on positions of R#" ) + std::to_string(N);
  UnitTests::tcase( tCase );
  for ( int i = 0; i < testNumber; ++ i )
  {
    const PosID P   = CPositions<N>::Random();
    const Coord pos = CPositions<N>::GetCoord( P );
    BitMapID rotations = CRotations<N>::ActOn( P );
    BitMap set( rotations );
    NL();
    clog( Color::white, (int) P, pos.toString() );
    all_rot( axis, layer, turn, N )
    {
      if ( pos.coord( axis ) == layer )
      {
        RotID rotID = 0;
        const bool ok = set >> rotID && ( CRotations<N>::GetRotID( axis, layer, turn ) == rotID );
        clog_( Color::cyan, CRotations<N>::ToString( CRotations<N>::GetRotID( axis, layer, turn ) ) );
        clog_( Color::yellow, "-->", Color::cyan, CRotations<N>::ToString( rotID ) );
        UnitTests::stamp( ok, result );
      }
    }
    UnitTests::stamp( set.empty(), result );
  }
  UnitTests::tail( tCase, result );
  return result;
}

bool UnitTests::unit_CRotations() const
{
  bool success = true;
  head( "cube rotation" );

  tcase( "RotID conversion" );
  all_rot( axis, layer, turn, 5 )
  {
    const RotID rotID = CRotations<5>::GetRotID( axis, layer, turn );
    clog_( Color::gray, (int) rotID, Color::white, "-->\t", Color::bold, CRotations<5>::ToString( rotID ) );
    bool s = ( axis == CRotations<5>::GetAxis( rotID ) ) && ( layer == CRotations<5>::GetLayer( rotID ) ) && ( turn == CRotations<5>::GetTurn( rotID ) );
    stamp( s, success );
  }
  tail( "RotID conversion", success );
   
  tcase( "RotID transformation" );
  const std::initializer_list< RotID > rotBase = {
    CRotations<4>::GetRotID( _X, 0, 1 ),
    CRotations<4>::GetRotID( _X, 1, 1 ),
    CRotations<4>::GetRotID( _X, 1, 3 ),
    CRotations<4>::GetRotID( _X, 3, 1 ),
    CRotations<4>::GetRotID( _X, 3, 2 ),
    
    CRotations<4>::GetRotID( _Y, 0, 1 ),
    CRotations<4>::GetRotID( _Y, 1, 1 ),
    CRotations<4>::GetRotID( _Y, 1, 3 ),
    CRotations<4>::GetRotID( _Y, 3, 1 ),
    CRotations<4>::GetRotID( _Y, 3, 2 ),
    
    CRotations<4>::GetRotID( _Z, 0, 1 ),
    CRotations<4>::GetRotID( _Z, 1, 1 ),
    CRotations<4>::GetRotID( _Z, 1, 3 ),
    CRotations<4>::GetRotID( _Z, 3, 1 ),
    CRotations<4>::GetRotID( _Z, 3, 2 )
  };
  
  const RotID expected[] = {
  // [_X1] aka RFD(13):
     1,  4,  6, 10, 11, 36, 33, 31, 27, 26, 13, 16, 18, 22, 23,
  
  // [_X2] aka RDB(20):
     1,  4,  6, 10, 11, 24, 21, 19, 15, 14, 36, 33, 31, 27, 26,
  
  // [_X3] aka RBU( 9):
     1,  4,  6, 10, 11, 25, 28, 30, 34, 35, 24, 21, 19, 15, 14,

  // [_Y1] aka BUR( 7):
    25, 28, 30, 34, 35, 13, 16, 18, 22, 23, 12,  9,  7,  3,  2,
  
  // [_Y2] aka LUB(23):
    12,  9,  7,  3,  2, 13, 16, 18, 22, 23, 36, 33, 31, 27, 26,
  
  // [_Y3] aka FUL(16):
    36, 33, 31, 27, 26, 13, 16, 18, 22, 23,  1,  4,  6, 10, 11,
  
  // [_Z1] aka ULF( 1):
    24, 21, 19, 15, 14,  1,  4,  6, 10, 11, 25, 28, 30, 34, 35,
  
  // [_Z2] aka LDF( 3):
    12,  9,  7,  3,  2, 24, 21, 19, 15, 14, 25, 28, 30, 34, 35,
  
  //[_Z3] aka DRF( 2):
    13, 16, 18, 22, 23, 12,  9,  7,  3,  2, 25, 28, 30, 34, 35
  };
  const RotID * nextExpected = expected;
  const std::string sign[] = { "_X", "_Y", "_Z" };

  for ( const Axis axis: { _X, _Y, _Z } )
  {
    for( const Turn turn: { 1, 2, 3 } )
    {
      const CubeID trans = Simplex::Tilt( axis, turn );
      const std::string testCase = std::string( "[" ) + sign[ axis ] + std::to_string( turn ) + std::string( "] aka ") + Simplex::GetCube( trans ).toString();
      tcase( testCase );
      bool successRot = true;
      for( const RotID testRot: rotBase )
      {
        const RotID result = CRotations<4>::GetRotID( testRot, trans );
        clog_( Color::cyan, numR( testRot, 3 ), CRotations<4>::ToString( testRot ), Color::white, "-->" );
        clog_( Color::cyan, numR( result,  3 ), CRotations<4>::ToString( result ) );
        stamp( result == *( nextExpected++ ), successRot );
      }
      tail( testCase, successRot );
      success &= successRot;
      NL();
    }
  }
  
  success &= testRotationActing <2> ( 5 );
  success &= testRotationActing <3> ( 5 );
  success &= testRotationActing <4> ( 5 );
  success &= testRotationActing <5> ( 5 );

  finish( "Cube rotations", success );
  return success;
}
