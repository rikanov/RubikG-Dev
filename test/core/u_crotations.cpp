#include <test.h>
#include <cube_rotations.h>

// ToDo: add transformed rotations to unit tests!
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
  
  finish( "Cube rotations", success );
  return success;
}