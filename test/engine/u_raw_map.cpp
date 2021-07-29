#include<test.h>
#include<state_map.h>

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
  test4_1x.move( CRotations<4>::GetRotID( _X, 0, 1 ) );
  test4_1x.print( true );
  test4_1x.move( CRotations<4>::GetRotID( _X, 0, 1 ) );
  test4_1x.print( true );
  test4_1x.move( CRotations<4>::GetRotID( _Y, 1, 3 ) );
  test4_1x.print( true );
  test4_1x.move( CRotations<4>::GetRotID( _Z, 2, 2 ) );
  test4_1x.print( true );

  const PosID stateMap[] = {
                               CPositions<4>::GetPosID( 0, 0, 3 ),
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 2 ),
                               CPositions<4>::GetPosID( 3, 3, 1 ),
                               CPositions<4>::GetPosID( 3, 3, 0 )

  };

  StateMap<4> A;
  A.build( stateMap, 7 );
  A.print();
  stamp(true, success );
  finish( "RawStateMap", success );
  return success;
}
