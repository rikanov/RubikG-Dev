#include <test.h>
#include <resolver.h>


bool UnitTests::unit_Resolver() const
{
  bool success = true;
  head( "AI" );

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
   // testCube.rotate( CRotations<2>::GetRotID( _X, 1, 1 ) );
    testCube.shuffle();
    
    Resolver<2> test( testCube );

    test.addInsight( left, 4 );
    test.addInsight( right, 4 );
    test.solve();

    

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
      CPositions<3>::GetPosID( 1, 0, 1 ),
    };
    const size_t cross_size = 5;
    
    bool ok = true;
    Rubik<3> testCube;
    //testCube.rotate( CRotations<3>::GetRotID( _Y, 1, 1 ) );
    testCube.shuffle();
    
    Resolver<3> test( testCube );

    test.addInsight( cross, 5 );
    test.addInsight( bind, 2 );
    test.solve();
    

    test.addInsight( block1, 2 );
    test.addInsight( block2, 2 );
    test.addInsight( block3, 2 );
    test.addInsight( block4, 2 );
    test.solve( false );
    

    stamp( ok, ok3 );
  }
  tail( "Cube size 3", ok3 );

  tcase( "Cube size 4" );
  bool ok4 = true;
  {
    
    bool ok = true;
    Rubik<4> testCube; 
    Resolver<4> test( testCube );
    test.solve();
    stamp( ok, ok4 );
  }
  tail( "Cube size 4", ok4 );

  tcase( "Cube size 5" );
  bool ok5 = true;
  {
    
    bool ok = true;
    Rubik<5> testCube; 
    Resolver<5> test( testCube );
    test.solve();
    stamp( ok, ok5 );
  }
  tail( "Cube size 5", ok5 );
  finish( "AI", success );
  return success;
}