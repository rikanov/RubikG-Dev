#include<test.h>
#include<raw_map.h>

template< cube_size N >
static bool test10Moves( RawStateMap2<N> & rsm, const RotID * moves )
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

bool UnitTests::unit_RawMap() const
{

  bool success = true;
  head( "RawStateMap" );

  const PosID pattern4_1[] = {
                               CPositions<4>::GetPosID( 0, 0, 3 ),
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 ),
                               CPositions<4>::GetPosID( 3, 3, 3 )
        };

  const RotID rotations4_1[] = {
                              CRotations<4>::GetRotID( _X, 0, 3 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 2, 3 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _Z, 3, 3 ),
                              CRotations<4>::GetRotID( _Z, 3, 1 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 2, 1 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 0, 1 )
        };

  const PosID pattern4_2[] = {
                               CPositions<4>::GetPosID( 1, 1, 3 ),
                               CPositions<4>::GetPosID( 1, 2, 3 ),
                               CPositions<4>::GetPosID( 2, 1, 3 ),
                               CPositions<4>::GetPosID( 2, 2, 3 )
        };

  const RotID rotations4_2[] = {
                              CRotations<4>::GetRotID( _X, 2, 3 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 2, 3 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _Z, 1, 3 ),
                              CRotations<4>::GetRotID( _Z, 1, 1 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 2, 1 ),
                              CRotations<4>::GetRotID( _Y, 1, 2 ),
                              CRotations<4>::GetRotID( _X, 2, 1 )
        };
  tcase( "Constructor" );
    timerON();
    RawStateMap2<4> Test4_1( 4, pattern4_1 );
    RawStateMap2<4> Test4_2( 4, pattern4_2 );
    timerOFF();
    clog( "Ellapsed time: ", ellapsed() );
  stamp( true, success );
  
  tcase( " 4x4 diagonal " );
  stamp( test10Moves( Test4_1, rotations4_1 ), success );

  tcase( " 4x4 square " );
  stamp( test10Moves( Test4_2, rotations4_2 ), success );

  finish( "RawStateMap", success );
  return success;
}