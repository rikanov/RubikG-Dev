#include "all_includes.h"


int main()
{
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  init();
  
  std::cin.get();
  
  UnitTests tests;
  bool success = true;
  
  success &= tests.testCore();
  success &= tests.testEngine();
  
  clog( "Quit." );
  
  return 0;
}
