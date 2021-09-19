#include <test.h>
#include <rubik_ai.h>


bool UnitTests::unit_RubikAI() const
{
  bool success = true;
  head( "AI" );

  tcase( "Cube size 2" );
  bool ok2 = true;
  {
    bool ok = true;
    Rubik<2> testCube; 
    RubikAI<2> test( testCube );
    test.solve();
    testCube.print();
    stamp( ok, ok2 );
  }
  tail( "Cube size 2", ok2 );

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
      CPositions<3>::GetPosID( 1, 0, 2 ),
      CPositions<3>::GetPosID( 1, 0, 1 )
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
      CPositions<3>::GetPosID( 1, 0, 1 ),
    };
    const size_t cross_size = 5;
    
    bool ok = true;
    Rubik<3> testCube;
    testCube.shuffle();
    testCube.print();
    RubikAI<3> test( testCube );

    test.addInsight( cross, 5 ).reference();;
    test.solve();
    testCube.print();

    test.addInsight( bind, 2 );
    test.solve();
    testCube.print();

    test.addInsight( block1, 2 );
    test.solve();
    testCube.print();

    test.addInsight( block2, 2 );
    test.solve();
    testCube.print();

    test.addInsight( block3, 2 );
    test.solve();
    testCube.print();

    test.addInsight( block4, 2 );
    test.solve();
    testCube.print();

    stamp( ok, ok3 );
  }
  tail( "Cube size 3", ok3 );

  tcase( "Cube size 4" );
  bool ok4 = true;
  {
    
    bool ok = true;
    Rubik<4> testCube; testCube.print();
    RubikAI<4> test( testCube );
    test.solve();
    stamp( ok, ok4 );
  }
  tail( "Cube size 4", ok4 );

  tcase( "Cube size 5" );
  bool ok5 = true;
  {
    
    bool ok = true;
    Rubik<5> testCube; testCube.print();
    RubikAI<5> test( testCube );
    test.solve();
    stamp( ok, ok5 );
  }
  tail( "Cube size 5", ok5 );
  finish( "AI", success );
  return success;
}
