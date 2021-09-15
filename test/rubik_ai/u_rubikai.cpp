#include <test.h>
#include <rubik_ai.h>


bool UnitTests::unit_RubikAI() const
{
  bool success = true;
  head( "AI" );

  tcase( "Cube size 2" );
  bool ok2 = true;
  
    NL(); clog( "test" );
    bool ok = true;
    Rubik<2> testCube; testCube.print();
    RubikAI<2> test( testCube );
    test.solve();
    stamp( ok, ok2 );
  
  tail( "Cube size 2", ok2 );

  tcase( "Cube size 3" );
  bool ok3 = true;
  {
    NL(); clog( "test" );
    bool ok = true;
    Rubik<3> testCube; testCube.print();
    RubikAI<3> test( testCube );
    test.solve();
    stamp( ok, ok3 );
  }
  tail( "Cube size 3", ok3 );

  tcase( "Cube size 4" );
  bool ok4 = true;
  {
    NL(); clog( "test" );
    bool ok = true;
    Rubik<4> testCube; testCube.print();
    RubikAI<4> test( testCube );
    test.solve();
    stamp( ok, ok4 );
  }
  tail( "Cube size 4", ok4 );

  tcase( "Cube size 5" );
  bool ok5 = true;
  {
    NL(); clog( "test" );
    bool ok = true;
    Rubik<5> testCube; testCube.print();
    RubikAI<5> test( testCube );
    test.solve();
    stamp( ok, ok5 );
  }
  tail( "Cube size 5", ok5 );
  finish( "AI", success );
  return success;
}
