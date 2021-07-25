#include<test.h>
#include<raw_map.h>

bool UnitTests::unit_RawMap() const
{

  bool success = true;
  head( "RawStateMap" );

  const PosID pattern4_1[] = { CPositions<4>::GetPosID( 0, 0, 3 ),
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 3 )
  };

  RawStateMap<4> test4_1( pattern4_1, 4 );
  test4_1.print();
  RawStateMap<4> test4_1x = test4_1;
  test4_1x.print( true );
  test4_1x.print();
  test4_1x.move( CRotations<4>::GetRotID( _X, 1, 1 ) );
  test4_1x.print();
  test4_1x.move( CRotations<4>::GetRotID( _Y, 1, 1 ) );
  test4_1x.print();

  stamp(true, success );
  finish( "RawStateMap", success );
  return success;
}
