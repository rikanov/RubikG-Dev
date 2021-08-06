#include<test.h>
#include<state_map.h>

template< cube_size N >
static bool test10Moves( StateMap<N> & rsm, const RotID * moves )
{
  for( int step = 0; step < 10; ++ step )
  {
    NL();
    clog( Color::blue, "Rotation by", Color::white, CRotations<4>::ToString( moves[step] ) );
    rsm.move( moves[step] );
    rsm.print();
  }
  return rsm.state() == 0;
}

bool UnitTests::unit_StateMap() const
{

  bool success = true;
  head( "StateMap" );

  const PosID stateMap4_1[] = {
                               CPositions<4>::GetPosID( 0, 0, 3 ),
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 2 ),
                               CPositions<4>::GetPosID( 3, 3, 1 ),
                               CPositions<4>::GetPosID( 3, 3, 0 )
  };

  const RotID rotations4[] = {
                               CRotations<4>::GetRotID( _X, 0, 1 ),
                               CRotations<4>::GetRotID( _X, 3, 1 ),
                               CRotations<4>::GetRotID( _Y, 1, 3 ),
                               CRotations<4>::GetRotID( _Z, 2, 2 ),
                               CRotations<4>::GetRotID( _Y, 2, 2 ),
                               CRotations<4>::GetRotID( _Y, 2, 2 ),
                               CRotations<4>::GetRotID( _Z, 2, 2 ),
                               CRotations<4>::GetRotID( _Y, 1, 1 ),
                               CRotations<4>::GetRotID( _X, 3, 3 ),
                               CRotations<4>::GetRotID( _X, 0, 3 )
  };
  
  StateMap<4> test4_1;
  test4_1.build( stateMap4_1, 7 );
  tcase( " 4x4 direct definition " );
  stamp( test10Moves( test4_1, rotations4 ), success );

  
  const PosID stateMap4_2a[] = {
                               CPositions<4>::GetPosID( 0, 0, 3 ),
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 3 )
  };
  
  const PosID stateMap4_2b[] = {
                               CPositions<4>::GetPosID( 3, 3, 2 ),
                               CPositions<4>::GetPosID( 3, 3, 1 )
  };
  
  StateMap<4> test4_2;
  tcase( "Add front diagonal" );
  test4_2.add( RawStateMap<4> ( stateMap4_2a, 4 ) );
  test4_2.print();
  
  tcase( "Add right-up edge" );
  test4_2.add( RawStateMap<4> ( stateMap4_2b, 2 ) );
  test4_2.print();
  
  tcase( "Add back diagonal" );
  test4_2.add( RawStateMap<4> ( stateMap4_2a, 4 ), Simplex::Composition( Simplex::Tilt( _Y, 2), Simplex::Tilt( _Z, 1 ) ) );
  test4_2.print();
  
  tcase( "Add left-down edge" );
  test4_2.add( RawStateMap<4> ( stateMap4_2b, 2 ), Simplex::Tilt( _Z, 2 ) );
  test4_2.print();
  
  tcase( " 4x4 composed " );
  stamp( test10Moves( test4_2, rotations4 ), success );
  
  finish( "StateMap", success );
  return success;
}