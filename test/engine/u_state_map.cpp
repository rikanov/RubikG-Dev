#include<test.h>
#include<state_map.h>

bool UnitTests::unit_StateMap() const
{

  bool success = true;
  head( "StateMap" );

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
  A.move( CRotations<4>::GetRotID( _X, 0, 1 ) );
  A.print();
  A.move( CRotations<4>::GetRotID( _X, 0, 1 ) );
  A.print();
  A.move( CRotations<4>::GetRotID( _Y, 1, 3 ) );
  A.print();
  A.move( CRotations<4>::GetRotID( _Z, 2, 2 ) );
  A.print();
  A.move( CRotations<4>::GetRotID( _Y, 2, 2 ) );
  A.print();
  A.move( CRotations<4>::GetRotID( _Z, 1, 2 ) );
  A.print();
  stamp( true , success );
  finish( "StateMap", success );
  return success;
}