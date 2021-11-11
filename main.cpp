#include "rubik_head.h"
#include <factory_tree.h>

int main()
{
  CLS(); 
  init();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." );

  Factory<3> test;

    const PosID * left= new PosID[4] {
      CPositions<2>::GetPosID( 0, 0, 0 ),
      CPositions<2>::GetPosID( 0, 0, 1 ),
      CPositions<2>::GetPosID( 0, 1, 1 ),
      CPositions<2>::GetPosID( 0, 1, 0 ),
    };

  Rubik<3> * rub = new Rubik<3>;;
  
  clog( ( long ) left );
  test.cube = rub;
  test.patternSize = 4;
  test.pattern = left;
  test.get();
  std::cin.get();
  
  UnitTests tests;
  bool success = true;
  {
   std::shared_ptr<uint8_t> m_pattern;
  }
  //success &= tests.testCore();
  success &= tests.testEngine();
 // success &= tests.testAI();

  CLS();
  clog( "Quit." );
  
  return 0;
}
