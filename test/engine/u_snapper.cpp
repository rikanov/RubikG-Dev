#include <test.h>
#include <snapper2.h>


bool UnitTests::unit_Snapper() const
{
  bool success = true;
  head( "Snapper" );

  const std::string path = "test/test_data/cubes/";

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

    Rubik<2> testCube2;
    testCube2.load( path + "2/17_20.rub" );
    cdraw ( '-', 30 );
    testCube2.revert( 17 );
    testCube2.print();

    Snapper2<2> test2;
    test2.toSolve( &testCube2 );
    test2.newTask( left,  4 );
    test2.newTask( right, 4 );
    test2.start();

    testCube2.revert( 18 );
    testCube2.print();
    test2.start();

    testCube2.load( path + "2/19_22.rub" );
    cdraw ( '-', 30 );
    testCube2.revert( 19 );
    test2.start();

    testCube2.revert( 20 );
    testCube2.print();
    test2.start();

    testCube2.load( path + "2/20_23.rub" );
    cdraw ( '-', 30 );
    testCube2.revert( 20 );
    test2.start();

    testCube2.revert( 21 );
    testCube2.print();
    test2.start();
    /*bool ok = true;
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
    testCube.save( "test_cube_2.rub" );*/
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
    Rubik<3> testCube3;
    Snapper2<3> test3;
    test3.toSolve( &testCube3 );

    test3.newTask( cross, 5 );
    test3.newTask( bind,  2 );

    test3.newTask( block1, 2 );
    test3.newTask( block2, 2 );
    test3.newTask( block3, 2 );
    test3.newTask( block4, 2 );
    test3.newTask( cross2,  5, 0, Accept<3>::RotAxis( _Z ) );
    test3.newTask( corners, 5, 0, Accept<3>::RotAxis( _Z ) );

    testCube3.load( path + "3/task_1/21_26.rub" );
    cdraw ( '-', 30 );
    testCube3.revert( 21 );
    test3.start(2);
    testCube3.revert( 22 );
    test3.start(2);

    testCube3.load( path + "3/task_1/25_28.rub" );
    cdraw ( '-', 30 );
    testCube3.revert( 25 );
    test3.start(2);
    testCube3.revert( 26 );
    test3.start(2);

    testCube3.load( path + "3/task_2/27_30.rub" );
    cdraw ( '-', 30 );
    testCube3.revert( 27 );
    test3.start(3);
    testCube3.revert( 28 );
    test3.start(3);
/*    Rubik<3> testCube;
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

    testCube.save( "test_cube_3.rub" ); */

  }

  finish( "Snapper", success );

  return success;
}
