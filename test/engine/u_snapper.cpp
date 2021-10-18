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


  tcase( "Cube size 3" );
  bool ok3 = true;
  {
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
    const PosID block1[] = {
      CPositions<3>::GetPosID( 2, 0, 2 ),
      CPositions<3>::GetPosID( 2, 0, 1 ),
      CPositions<3>::GetPosID( 1, 0, 2 )
    };
    const PosID block2[] = {
      CPositions<3>::GetPosID( 2, 2, 2 ),
      CPositions<3>::GetPosID( 2, 2, 1 ),
      CPositions<3>::GetPosID( 1, 2, 2 )
    };
    const PosID block3[] = {
      CPositions<3>::GetPosID( 0, 2, 2 ),
      CPositions<3>::GetPosID( 0, 2, 1 ),
      CPositions<3>::GetPosID( 0, 1, 2 ),
    };
    const PosID block4[] = {
      CPositions<3>::GetPosID( 0, 0, 2 ),
      CPositions<3>::GetPosID( 0, 0, 1 ),
      CPositions<3>::GetPosID( 1, 0, 2 ),
    };

    const PosID cross2[] = {
      CPositions<3>::GetPosID( 1, 2, 0 ),
      CPositions<3>::GetPosID( 0, 1, 0 ),
      CPositions<3>::GetPosID( 1, 1, 0 ),
      CPositions<3>::GetPosID( 2, 1, 0 ),
      CPositions<3>::GetPosID( 1, 0, 0 ),
    };

    const PosID corners[] = {
      CPositions<3>::GetPosID( 0, 0, 0 ),
      CPositions<3>::GetPosID( 0, 2, 0 ),
      CPositions<3>::GetPosID( 1, 1, 0 ),
      CPositions<3>::GetPosID( 2, 0, 0 ),
      CPositions<3>::GetPosID( 2, 2, 0 ),
    };

    Rubik<3> testCube;
    testCube.shuffle();
    testCube.print();

    Snapper2<3> test;
    test.toSolve( &testCube );

    test.newTask( cross, 5 );
    test.newTask( bind,  2 );
    test.start();

    test.newTask( block1, 2);
    test.start();

    test.newTask( block2, 2);
    test.start();

    test.newTask( block3, 2);
    test.start();

    test.newTask( block4, 2);
    test.start();
    const size_t st = testCube.steps();

    test.newTask( cross2, 5, 0, Accept<3>::RotAxis( _Z ) );
    test.start();

    test.newTask( corners, 5, 0, Accept<3>::RotAxis( _Z ) );
    test.start();

    testCube.save( "testCube.txt" );

    Rubik<3> testCube65;
    testCube65.load( "testCube_65.txt" );
    testCube65.history(65);
    testCube65.print();
    test.toSolve( &testCube65 );
    test.start();
    test.printState();
  }

  finish( "Snapper", success );

  return success;
}
