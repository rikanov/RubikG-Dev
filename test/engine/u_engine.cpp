#include<test.h>
#include<engine.h>


bool UnitTests::unit_Engine() const
{
  bool success = true;
  head( "Search-engine" );

  SubSpace cross = {
                  CPositions<3>::GetPosID( 1, 0, 2 ),
                  CPositions<3>::GetPosID( 1, 1, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
                  CPositions<3>::GetPosID( 2, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 2, 2 ),
                  CPositions<3>::GetPosID( 1, 2, 1 )
  };
  SubSpace cornerBlock = {
                  CPositions<3>::GetPosID( 0, 0, 1 ),
                  CPositions<3>::GetPosID( 0, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 2, 1 ),
                  CPositions<3>::GetPosID( 0, 2, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
  };
  SubSpace corners = {
                  CPositions<3>::GetPosID( 0, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 2, 2 ),
                  CPositions<3>::GetPosID( 2, 0, 2 ),
                  CPositions<3>::GetPosID( 2, 2, 2 ),
                  CPositions<3>::GetPosID( 2, 2, 1 )

  };
  SubSpace edgePool = {
                  CPositions<3>::GetPosID( 0, 0, 1 ),
                  CPositions<3>::GetPosID( 0, 2, 1 ),
                  CPositions<3>::GetPosID( 2, 0, 1 ),
                  CPositions<3>::GetPosID( 2, 2, 1 ),
                  CPositions<3>::GetPosID( 1, 1, 2 )

  };


  SubSpace lastEdges = {
                  CPositions<3>::GetPosID( 0, 1, 0 ),
                  CPositions<3>::GetPosID( 0, 1, 1 ),
                  CPositions<3>::GetPosID( 1, 0, 0 ),
                  CPositions<3>::GetPosID( 1, 0, 1 ),
                  CPositions<3>::GetPosID( 2, 1, 0 ),
                  CPositions<3>::GetPosID( 1, 2, 0 )
  };
  SubSpace lastCorners = {
                  CPositions<3>::GetPosID( 0, 0, 0 ),
                  CPositions<3>::GetPosID( 0, 0, 1 ),
                  CPositions<3>::GetPosID( 0, 2, 0 ),
                  CPositions<3>::GetPosID( 0, 2, 1 ),
                  CPositions<3>::GetPosID( 2, 0, 0 ),
                  CPositions<3>::GetPosID( 2, 2, 0 )
  };


  tcase( "Memory allocation & pool creations" );

  timerON();
  Insight<3> step_1 ( cross );
  Insight<3> test   ( cornerBlock );
  Insight<3> step_2 ( test, 0 );
  Insight<3> step_3 ( test, Simplex::Tilt( _Z, 2 ) );
  Insight<3> step_6 ( step_1,   Simplex::Tilt( _X, 2 ) );
  Insight<3> step_7 ( corners,  Simplex::Tilt( _X, 2 ), _Z );
  Insight<3> step_8 ( corners,  Simplex::Tilt( _X, 2 ) );
  timerOFF();
  clog( Color::cyan, "Ellapsed time:", Color::white, Color::bold, ellapsed(), Color::off );

  tail( "Memory allocation & pool creations", success );
  Rubik <3> testCube3;

  testCube3.shuffle();
  testCube3.print();

  Engine<3>  testEngine1( testCube3 );
  Engine<3>  testEngine2( testCube3 );
  testEngine2 << step_1;
  testEngine2.run( 11 );
  testCube3.print();
  testEngine2 << step_2;
  testEngine2.run( 12 );
  testCube3.print();
  testEngine2 << step_3;
  testEngine2.run( 12 );
  testCube3.print();
  testEngine2 << step_6;
  testEngine2.run( 12 );
  testCube3.print();
  testEngine2 << step_7;
  testEngine2.run( 12 );
  testCube3.print();
  testEngine2.swap( &step_7, &step_8 );
  testEngine2.run( 12 );
  testCube3.print();
 // testEngine2 << addCache;
 // testEngine2.run( 11 ); // it does nothing right now
 //  testCube3.print();
/*  testEngine2 << step_6;
  testEngine2.run( 11 );
  testCube3.print();
  testEngine2 << step_7;
  testEngine2.run( 11 );
  testCube3.print();
  testEngine2 << step_8;
  testEngine2.run( 12 );
  testCube3.print();
  testEngine2 << step_9;
  testEngine2.run( 12 );
  testCube3.print(); */

  stamp(true, success );
  finish( "Search-engine", success );
  return success;
}
