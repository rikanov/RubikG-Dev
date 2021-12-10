#include <test.h>
#include <progress.h>

bool UnitTests::unit_Progress() const
{
  bool success = true;
  head( "Progress" );

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
    testCube2.shuffle();
    testCube2.print();
    Progress<2> test2;

    test2.toSolve( testCube2 );
    test2.addGuide( _Scheduled, 4, left );
    test2.addGuide( _Scheduled, 4, right );

    test2.solve( 10 );
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

    Progress<3> test;
    test.toSolve( testCube );

    test.addGuide( _Scheduled, 5, cross );
    test.addGuide( _Scheduled, 2, bind );
    test.solve( 10 );

    test.addGuide( _Optional, 2, block1 );
    test.addGuide( _Optional, 2, block2 );
    test.addGuide( _Optional, 2, block3 );
    test.addGuide( _Optional, 2, block4 );
    test.solve( 10 );

    test.addGuide( _Optional, 5, cross2, Accept<3>::RotAxis( _Z ) );
    test.addGuide( _Optional, 5, corners, Accept<3>::RotAxis( _Z ) );
    test.solve( 10 );
  }

  finish( "Progress", success );

  return success;
}

