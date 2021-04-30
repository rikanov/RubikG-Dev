#include<test.h>
#include<engine.h>


bool UnitTests::unit_Engine() const
{
  bool success = true;
  SubSpace cross = {
                  CPositions<3>::GetPosID( 1, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
                  CPositions<3>::GetPosID( 2, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 2, 2 )
  };
  SubSpace corners = {
                  CPositions<3>::GetPosID( 0, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 2, 2 ),
                  CPositions<3>::GetPosID( 2, 0, 2 ),
                  CPositions<3>::GetPosID( 2, 2, 2 ),
                  CPositions<3>::GetPosID( 1, 1, 2 )
  };
  Insight<3> step1( cross );
  Insight<3> step2( corners );

  Rubik <3> testCube3;
  testCube3.rotate( _X,0,1);
  testCube3.rotate( _X,1,1);
  testCube3.rotate( _X,2,2);
  testCube3.rotate( _Z,2,2);
  testCube3.rotate( _Y,1,2);
  testCube3.rotate( _Z,1,2);
  testCube3.shuffle();
  testCube3.print();
  Engine<3> testEngine ( testCube3 );
  testEngine << step1;
  testEngine << step2;

  testEngine.run( 12 );

  testCube3.print();
  return success;
}
