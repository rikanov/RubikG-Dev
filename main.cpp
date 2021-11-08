#include "rubik_head.h"
#include <factory_tree.h>

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

test.create( 4, left );
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
