#include "all_includes.h"


int main()
{
  CLS();
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  init();
  
  std::cin.get();
  
  UnitTests tests;
  bool success = true;
  
  success &= tests.testCore();
  success &= tests.testEngine();
  
  CLS();
  clog( "Quit." );
  
  return 0;
}
