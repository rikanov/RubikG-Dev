#include "all_includes.h"


int main()
{
  clog( Color::off, "Press", Color::white, Color::bold, "ENTER", Color::off,"to start." ); 
  init();
  
  std::cin.get();
  
  UnitTests tests;
  tests.testCore();
  clog( "Quit." );
  
  return 0;
}
