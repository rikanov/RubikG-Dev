#include <test.h>
#include <rubik.h>

bool UnitTests::unit_Rubik() const
{
  bool success = true;
  head( "Rubik framework" );

  clog_( "Rubik framework test..." );
  Rubik<5> test1, test2;
  done();

  NL();
  NL();

  clog( Color::white, "Start testing turns" );
  clog(               "-------------------" );
  NL();
  tcase( "Test case 1", " X1, Y1" );
  test1.rotate(  CRotations<5>::GetRotID( _X, 1, 1 ) );
  test1.rotate(  CRotations<5>::GetRotID( _Y, 1, 1 ) );
  test1.print();
  success &= test1.integrity();
  
  tcase( "Test case 2 :", " Y1, Z4" );
  test2.rotate(  CRotations<5>::GetRotID( _Y, 1, 1 ) );
  test2.rotate(  CRotations<5>::GetRotID( _Z, 4, 1 ) );
  test2.print();
  success &= test2.integrity();
  
  tcase( "Test case 5", " revert of T4", "Z4, Z4, Z4, Y1, Y1, X1, X1, X1" );
  Rubik<5> test5;
  test5.rotate( CRotations<5>::GetRotID( _Z, 4, 3 ) );
  test5.rotate( CRotations<5>::GetRotID( _Y, 1, 2 ) );
  test5.rotate( CRotations<5>::GetRotID( _X, 1, 3 ) );
  test5.print();
  success &= test5.integrity();
 
  finish( "Rubik framework", success );
  return success;
}
