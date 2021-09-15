#include "rubik_head.h"


int main()
{
  CLS(); 
  init();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." );
  
  std::cin.get();
  
  UnitTests tests;
  bool success = true;
  
  success &= tests.testCore();
  success &= tests.testEngine();
  success &= tests.testAI();
  
  CLS();
  clog( "Quit." );
  
  return 0;
}
