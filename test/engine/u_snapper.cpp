#include <test.h>
#include <snapper2.h>
#include <tuple>

bool UnitTests::unit_Snapper() const
{
  bool success = true;
  head( "Snapper" );

  const std::string path = "../test/test_data/cubes/";

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
    
    using TestData = std::tuple< int, const char * >;
    
    const TestData files[] = {
      { 17, "17_20" },
      { 19, "19_22" },
      { 20, "20_23" }
    };
       
    Rubik<2> testCube2;
    Snapper2<2> test2;
    
    test2.toSolve( &testCube2 );
    test2.newTask( left,  4 );
    test2.newTask( right, 4 );

    for ( auto file: files )
    {
      const int step = std::get<0>( file );
      const char * postfix = std::get<1>( file );
      const std::string fileName = path + "2/" + postfix + ".rub";
      
      NL();
      clog( fileName );
      NL();
      cdraw ( '-', 30 );
      testCube2.load( fileName );
      testCube2.revert ( step );
      test2.start();  

      testCube2.load( fileName );
      testCube2.revert ( step + 1 );
      test2.start();     
      cdraw ( '-', 30 );
    }
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
    
    using TestData = std::tuple< int, int, const char * >;
    
    const TestData files[] = {
      { 2, 27, "27_30" },
      { 2, 30, "30_33" },
      { 2, 36, "36_39" },
      { 3, 33, "33_36" },
      { 3, 33, "33_37" },
      { 4, 37, "38_44" },
      { 5, 52, "52_55" },
      { 5, 56, "56_59" },
      { 6, 50, "50_53" },
      { 6, 55, "55_60" },
      { 6, 56, "56_59" },
      { 6, 58, "58_64" },
      { 6, 63, "63_66" },
      { 6, 63, "64_68" },
      { 7, 58, "58_61" },
      { 7, 69, "69_72" },
      { 7, 70, "70_73" }
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
    
    for ( auto file: files )
    {
      const int task = std::get<0>( file );
      const int step = std::get<1>( file );
      const char * postfix = std::get<2>( file );
      const std::string fileName = path + "3/task_" + std::to_string( task ) + "/" + postfix + ".rub";
      
      NL();
      clog( fileName );
      NL();
      cdraw ( '-', 30 );
      testCube3.load( fileName );
      testCube3.revert ( step );
      test3.start( task + 1 );  

      testCube3.load( fileName );
      testCube3.revert ( step + 1 );
      test3.start( task + 1 );     
      cdraw ( '-', 30 );
    }

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

    test.newTask( cross2, 5, 0, Accept<3>::RotAxis( _Z ) );
    test.start();

    test.newTask( corners, 5, 0, Accept<3>::RotAxis( _Z ) );
    test.start();

    testCube.save( "test_cube_3.rub" );

  }

  finish( "Snapper", success );

  return success;
}
