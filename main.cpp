#include "rubik_head.h"
#include <state_control.h>
#include <shift_control.h>

int main()
{
  CLS(); 
  init();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." );

  Factory<3> test;

    const PosID left[] = {
      CPositions<2>::GetPosID( 0, 0, 0 ),
      CPositions<2>::GetPosID( 0, 0, 1 ),
      CPositions<2>::GetPosID( 0, 1, 1 ),
      CPositions<2>::GetPosID( 0, 1, 0 ),
    };

  size_t level = 0;
  size_t depth = 0;
  Factory<3>::StateAPI state[200] = {};
  RotID step = 0;

  Factory<3>::ControlPanel( &level, &depth, state, &step );
  ShiftControl<3> shiftTest;

  test.patternSize = 4;
  test.pattern = left;
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
