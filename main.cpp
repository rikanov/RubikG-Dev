#include "rubik_head.h"
#include <progress.h>

int main()
{
  CLS(); 
  init();

  const PosID cross[] = {
    CPositions<3>::GetPosID( 1, 2, 2 ),
    CPositions<3>::GetPosID( 0, 1, 2 ),
    CPositions<3>::GetPosID( 1, 1, 2 ),
    CPositions<3>::GetPosID( 2, 1, 2 ),
    CPositions<3>::GetPosID( 1, 0, 2 )
  };

  Rubik<3> r;
  r.rotate( CRotations<3>::GetRotID(_Y, 1, 2 ) );
  r.rotate( CRotations<3>::GetRotID(_X, 1, 2 ) );
  Progress<3> test; clog( " constructor" );
  test.toSolve( &r ); clog( " set cube " );
  test.addGuide( _Scheduled, 5, cross, Accept<3>::Normal ); clog( " guide added" );
  test.startIDA( 5 );

  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." );
  std::cin.get();
  
  UnitTests tests;
  bool success = true;
  //success &= tests.testCore();
  success &= tests.testEngine();
 // success &= tests.testAI();

  CLS();
  clog( "Quit." );
  
  return 0;
}
