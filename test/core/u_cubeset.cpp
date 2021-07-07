#include <test.h>
#include <simplex.h>
#include <cube_set.h>
#include <bitmap_set.h>
#include <set>

bool UnitTests::unit_CubeSet() const
{
  bool success = true;
  head( "rotation by cube sets" );
  
  // create std::set for random CubeIDs
  std::set<int> cubeSet;
  
  // create bitmap for random CubeIDs
  BitMap32ID cubeSetID = 0;
  
  // fill up with random cubeIDs
  for( int i = 0; i < 20; ++ i )
  {
    const CubeID next = random( 0, 23 );
    cubeSetID |= ( 1 << next );
    cubeSet.insert( next );
  }
  
  // test the sets equality
  BitMap bitMap( cubeSetID );
  auto it = cubeSet.begin();
  CubeID next = 0;
  
  tcase( "Testing funcionality of bitmaps" );
  while( bitMap >> next )
  {
    clog_( Color::cyan, (int) next, '=', (int) *it );
    stamp( next == *( it ++ ), success );
  }
  clog_( "Test equal sizes: ");
  stamp( it == cubeSet.end(), success );
  tail( "Bitmap functionaliy", success );
  
  for( int test = 0; test < 6; ++ test )
  {
    const CubeID state = random( 0, 23 );
    tcase( "Multiplication", std::to_string( state ) );
    clog( cubeSetID );
    BitMap generated( CubeSet::GetBitMap32ID( state, cubeSetID ) );
    BitMap original( cubeSetID );
    CubeID next, expected;
    clog_( "generated bitmap:", CubeSet::GetBitMap32ID( state, cubeSetID ) );
    generated.print(8);
    while( generated >> expected )
    {
      const bool exists = original >> next;
      clog_( (int) state, 'X', (int) next, '=', (int) Simplex::Composition( state, next ), '=', (int) expected );
      stamp( exists && ( Simplex::Composition( state, next ) == expected ), success );
    }
    tail( "Multiplicationr", success );
  }
  
  finish( "Cube rotations", success );
  return success;
}