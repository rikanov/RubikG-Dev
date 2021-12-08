#include <test.h>
#include <simplex.h>
#include <cube_set.h>
#include <bitmap_set.h>
#include <set>

bool UnitTests::unit_CubeSet() const
{
  bool success = true;
  head( "CubeSet rotations" );
  
  // create std::set for random CubeIDs
  std::set<int> stdSet;

  // create bitmap for random CubeIDs
  BitMapID cubeSetID = 0;
  
  // fill up with random cubeIDs
  for( int i = 0; i < 20; ++ i )
  {
    const CubeID next = random( 0, 23 );
    cubeSetID |= ( 1 << next );
    stdSet.insert( next );
  }
  
  // test the sets equality
  BitMap bitMap( cubeSetID );
  auto it = stdSet.begin();
  CubeID next = 0;
  
  tcase( "Testing funcionality of bitmaps" );
  while( bitMap >> next )
  {
    clog_( Color::cyan, (int) next, '=', (int) *it );
    stamp( next == *( it ++ ), success );
  }
  clog_( "Test equal sizes: ");
  stamp( it == stdSet.end(), success );
  tail( "Bitmap functionaliy", success );
  
  for( int test = 0; test < 6; ++ test )
  {
    const CubeID state = Simplex::Random();
    tcase( "Multiplication", Simplex::GetCube( state ).toString() );
    BitMap generated( CubeSet::GetCubeSet( state, cubeSetID ) );
    BitMap original( cubeSetID );
    original.print( 24, 4 );
    generated.print( 24, 4 );
    CubeID next;
    while( original >> next )
    {
      clog_( (int) next, 'X', (int) state, '=', (int) Simplex::Composition( next, state ), '\t' );
      stamp( generated.contains( Simplex::Composition( next, state ) ), success );
      // remove from generated
      generated.remove( Simplex::Composition( next, state ) );
    }
    clog_( "Test equal sizes: ");
   // stamp( generated.empty(), success );
    tail( "Multiplication", success );
  }
  
  finish( "CubeSet rotations", success );
  return success;
}
