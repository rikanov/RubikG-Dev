#include<test.h>
#include<engine.h>


bool UnitTests::unit_Engine() const
{
  bool success = true;
  head( "Search-engine" );

  Engine<2> e2;
  Engine<3> e3;
  Engine<4> e4;
  Engine<5> e5;

  finish( "Search-engine", success );
  return success;
}
