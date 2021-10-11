#include <test.h>
#include <snapper2.h>


bool UnitTests::unit_Snapper() const
{
  bool success = true;
  head( "Snapper" );


  tcase( "Cube size 2" );
  bool ok2 = true;
  {
    const PosID left[] = {
      CPositions<2>::GetPosID( 0, 0, 0 ),
      CPositions<2>::GetPosID( 0, 0, 1 ),
      CPositions<2>::GetPosID( 0, 1, 1 ),
      CPositions<2>::GetPosID( 0, 1, 0 ),
    };


    const PosID right[] = {
      CPositions<2>::GetPosID( 1, 0, 0 ),
      CPositions<2>::GetPosID( 1, 0, 1 ),
      CPositions<2>::GetPosID( 1, 1, 1 ),
      CPositions<2>::GetPosID( 1, 1, 0 ),
    };
    bool ok = true;
    Rubik<2> testCube;
    testCube.rotate( CRotations<2>::GetRotID( _X, 0, 1 ) );
    testCube.rotate( CRotations<2>::GetRotID( _Y, 1, 2 ) );
    testCube.rotate( CRotations<2>::GetRotID( _X, 0, 1 ) );
    testCube.shuffle();
    testCube.print();

    Snapper2<2> test;
    test.toSolve( &testCube );
    test.newTask( left, 4 );
    test.newTask( right, 4 );
    test.start();
  }

  finish( "Snapper", success );

  return success;
}
