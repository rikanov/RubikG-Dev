#include <test.h>
#include <cube_rotations.h>

bool UnitTests::unit_CExtRotations() const
{
  bool success = true;
  head( "Extended rotation" );

  tcase( "RotID conversion" );
  all_rot( axis, layer, turn, 5 )
  {
    const RotID rotID = CExtRotations<4>::GetRotID( axis, layer, turn );
  const RotID low  = rotID - rotID % ( 5 * 3 );
  const RotID high = rotID % 3 == 0 ? rotID : rotID + 3 - rotID % 3;
    clog_( Color::gray, (int) rotID, Color::white, "-->\t", Color::bold, CRotations<5>::ToString( rotID ), (int) low, (int) high );
    bool s = ( axis == CExtRotations<4>::GetAxis( rotID ) ) && ( layer == CExtRotations<4>::GetLayer( rotID ) ) && ( turn == CExtRotations<4>::GetTurn( rotID ) );
    stamp( s, success );
  }
  tail( "RotID conversion", success );
  
  tcase( "RotID transformation" );
  const std::initializer_list< RotID > rotBase = {
    CExtRotations<4>::GetRotID( _X, 0, 1 ),
    CExtRotations<4>::GetRotID( _X, 1, 1 ),
    CExtRotations<4>::GetRotID( _X, 1, 3 ),
    CExtRotations<4>::GetRotID( _X, 4, 1 ),
    CExtRotations<4>::GetRotID( _X, 4, 2 ),
    
    CExtRotations<4>::GetRotID( _Y, 0, 1 ),
    CExtRotations<4>::GetRotID( _Y, 1, 1 ),
    CExtRotations<4>::GetRotID( _Y, 1, 3 ),
    CExtRotations<4>::GetRotID( _Y, 4, 1 ),
    CExtRotations<4>::GetRotID( _Y, 4, 2 ),
    
    CExtRotations<4>::GetRotID( _Z, 0, 1 ),
    CExtRotations<4>::GetRotID( _Z, 1, 1 ),
    CExtRotations<4>::GetRotID( _Z, 1, 3 ),
    CExtRotations<4>::GetRotID( _Z, 4, 1 ),
    CExtRotations<4>::GetRotID( _Z, 4, 2 )
  };
  
  const RotID expected[] = {
  // [_X1] aka RFD(13):
     1,  4,  6, 13, 14, 42, 39, 37, 43, 44, 16, 19, 21, 28, 29,

  // [_X2] aka RDB(20):
     1,  4,  6, 13, 14, 27, 24, 22, 28, 29, 42, 39, 37, 43, 44,

  // [_X3] aka RBU( 9):
     1,  4,  6, 13, 14, 31, 34, 36, 43, 44, 27, 24, 22, 28, 29,

  // [_Y1] aka BUR( 7):
    31, 34, 36, 43, 44, 16, 19, 21, 28, 29, 12,  9,  7, 13, 14,

  // [_Y2] aka LUB(23):
    12,  9,  7, 13, 14, 16, 19, 21, 28, 29, 42, 39, 37, 43, 44,

  // [_Y3] aka FUL(16):
    42, 39, 37, 43, 44, 16, 19, 21, 28, 29,  1,  4,  6, 13, 14,

  // [_Z1] aka ULF( 1):
    27, 24, 22, 28, 29,  1,  4,  6, 13, 14, 31, 34, 36, 43, 44,

  // [_Z2] aka LDF( 3):
    12,  9,  7, 13, 14, 27, 24, 22, 28, 29, 31, 34, 36, 43, 44,

  // [_Z3] aka DRF( 2):
    16, 19, 21, 28, 29, 12,  9,  7, 13, 14, 31, 34, 36, 43, 44
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
        const RotID result = CExtRotations<4>::GetRotID( testRot, trans );
        clog_( ( testRot < 10 ? "  " : " " ), Color::cyan, (int) testRot, CExtRotations<4>::ToString( testRot ), Color::white, "-->" );
        clog_( ( result  < 10 ? "  " : " " ), Color::cyan, (int) result,  CExtRotations<4>::ToString( result ) );
        stamp( result == *( nextExpected++ ), successRot );
      }
      tail( testCase, successRot );
      success &= successRot;
      NL();
    }
  }
  
  finish( "Extended rotations", success );
  return success;
}
