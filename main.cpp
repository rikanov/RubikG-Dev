#include "rubik_head.h"
#include <progress.h>

int main()
{
  CLS(); 
  init();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." );

  const PosID cross[] = {
    CPositions<3>::GetPosID( 1, 2, 2 ),
    CPositions<3>::GetPosID( 0, 1, 2 ),
    CPositions<3>::GetPosID( 1, 1, 2 ),
    CPositions<3>::GetPosID( 2, 1, 2 ),
    CPositions<3>::GetPosID( 1, 0, 2 )
  };

  Progress<3> testProgress;
  testProgress.addScheduled( 5, cross, Accept<3>::Normal );
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
