#include <test.h>
#include <progress.h>

bool UnitTests::unit_Progress() const
{
  bool success = true;
  head( "Progress" );

  const PosID cross[] = {
    CPositions<3>::GetPosID( 1, 2, 2 ),
    CPositions<3>::GetPosID( 0, 1, 2 ),
    CPositions<3>::GetPosID( 1, 1, 2 ),
    CPositions<3>::GetPosID( 2, 1, 2 ),
    CPositions<3>::GetPosID( 1, 0, 2 )
  };

  const PosID bind[] = {
    CPositions<3>::GetPosID( 1, 0, 1 ),
    CPositions<3>::GetPosID( 1, 0, 2 ),
  };

  Rubik<3> r;
  r.rotate( CRotations<3>::GetRotID(_Y, 1, 2 ) );
  r.rotate( CRotations<3>::GetRotID(_X, 1, 2 ) );
  r.rotate( CRotations<3>::GetRotID(_Y, 2, 1 ) );
  r.rotate( CRotations<3>::GetRotID(_Z, 1, 3 ) );
  Progress<3> test;
  test.toSolve( &r );
  test.addGuide( _Scheduled, 5, cross, Accept<3>::Normal );
  test.addGuide( _Scheduled, 2, bind,  Accept<3>::Normal );
  test.startIDA( 5 );

  finish( "Progress", success );

  return success;
}

