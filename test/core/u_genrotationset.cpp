#include <test.h>
#include <bitmap_set.h>
#include <cube_rotations.h>
#include <gen_rotation_set.h>


bool UnitTests::unit_CGenRotations() const
{
  bool success = true;
  head( "Generate rotation set" );

  tcase( "2 X 2 X 2 cube" );
  for ( int i= 0; i < 10; ++ i )
  {
    BitMapID test = 0;
    BitMapID gen  = 0;
    const CubeID rotation = Simplex::Random();
    for ( int i = 0; i < 6; ++ i)
    {
      const RotID next  = CRotations<2>::Random();
      const RotID trans = CRotations<2>::GetRotID( next, rotation );

      test |= 1ULL << next;
      gen  |= 1ULL << trans;
    }

    clog ( Color::light, "Rotation :", Color::yellow, Simplex::GetCube( rotation ).toString() );
    clog_( Color::dark, "Original :" ); BitMap::Print( test, 18, 6 );
    cdraw( '-', 16 + 18 );
    NL();;

    GenerateRotationSet<2>::Transform( test, rotation );

    clog_( Color::dark, "Generated:" ); BitMap::Print( test, 18, 6 );
    clog_( Color::dark, "Expected :" ); BitMap::Print(  gen, 18, 6 );
    stamp( test == gen, success );
    NL();
  }
  tail( "2 X 2 X 2 cube", success );

  tcase( "3 X 3 X 3 cube" );
  for ( int i= 0; i < 10; ++ i )
  {
    BitMapID test = 0;
    BitMapID gen  = 0;
    const CubeID rotation = Simplex::Random();
    for ( int i = 0; i < 9; ++ i)
    {
      const RotID next  = CRotations<3>::Random();
      const RotID trans = CRotations<3>::GetRotID( next, rotation );

      test |= 1ULL << next;
      gen  |= 1ULL << trans;
    }

    clog ( Color::light, "Rotation :", Color::yellow, Simplex::GetCube( rotation ).toString() );
    clog_( Color::dark, "Original :" ); BitMap::Print( test, 27, 9 );
    cdraw( '-', 16 + 27 );
    NL();

    GenerateRotationSet<3>::Transform( test, rotation );

    clog_( Color::dark, "Generated:" ); BitMap::Print( test, 27, 9 );
    clog_( Color::dark, "Expected :" ); BitMap::Print(  gen, 27, 9 );
    stamp( test == gen, success );
    NL();
  }
  tail( "3 X 3 X 3 cube", success );

  tcase( "4 X 4 X 4 cube" );
  for ( int i= 0; i < 10; ++ i )
  {
    BitMapID test = 0;
    BitMapID gen  = 0;
    const CubeID rotation = Simplex::Random();
    for ( int i = 0; i < 12; ++ i)
    {
      const RotID next  = CRotations<4>::Random();
      const RotID trans = CRotations<4>::GetRotID( next, rotation );

      test |= 1ULL <<  next;
      gen  |= 1ULL <<  trans;
    }

    clog ( Color::light, "Rotation :", Color::yellow, Simplex::GetCube( rotation ).toString() );
    clog_( Color::dark, "Original :" ); BitMap::Print( test, 36, 12 );
    cdraw( '-', 16 + 36 );
    NL();

    GenerateRotationSet<4>::Transform( test, rotation );

    clog_( Color::dark, "Generated:" ); BitMap::Print( test, 36, 12 );
    clog_( Color::dark, "Expected :" ); BitMap::Print(  gen, 36, 12 );
    stamp( test == gen, success );
    NL();
  }
  tail( "4 X 4 X 4 cube", success );


  tcase( "5 X 5 X 5 cube" );
  for ( int i= 0; i < 10; ++ i )
  {
    BitMapID test = 0;
    BitMapID gen  = 0;
    const CubeID rotation = Simplex::Random();
    for ( int i = 0; i < 15; ++ i)
    {
      const RotID next  = CRotations<5>::Random();
      const RotID trans = CRotations<5>::GetRotID( next, rotation );

      test |= 1ULL << next;
      gen  |= 1ULL << trans;
    }

    clog ( Color::light, "Rotation :", Color::yellow, Simplex::GetCube( rotation ).toString() );
    clog_( Color::dark, "Original :" ); BitMap::Print( test, 45, 15 );
    cdraw( '-', 16 + 45 );
    NL();

    GenerateRotationSet<5>::Transform( test, rotation );

    clog_( Color::dark, "Generated:" ); BitMap::Print( test, 45, 15 );
    clog_( Color::dark, "Expected :" ); BitMap::Print(  gen, 45, 15);
    stamp( test == gen, success );
    NL();
  }
  tail( "5 X 5 X 5 cube", success );

  finish( "Generate rotation set", success );
  return success;
}
