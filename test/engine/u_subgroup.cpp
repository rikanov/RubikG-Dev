#include<test.h>
#include<insight.h>

template< cube_size N >
static bool test10Moves( Insight<N> & rsm, const RotID * moves )
{
  for( int step = 0; step < 10; ++ step )
  {
    NL();
    clog( Color::blue, "Rotation by", Color::white, CRotations<4>::ToString( moves[step] ) );
    rsm.move( moves[step] );
    rsm.print();
    rsm.print( true, true );
  }
  return rsm.state() == 0;
}

bool UnitTests::unit_Subgroup() const
{

  bool success = true;
  head( "Subgroup" );

  const PosID pattern3_1[] = {
                  CPositions<3>::GetPosID( 1, 2, 2 ),
                  CPositions<3>::GetPosID( 1, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
                  CPositions<3>::GetPosID( 2, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 1, 2 )
                };

  const RotID rotations3_1[] = {
                              CRotations<3>::GetRotID( _X, 0, 3 ),
                              CRotations<3>::GetRotID( _Y, 1, 2 ),
                              CRotations<3>::GetRotID( _X, 2, 3 ),
                              CRotations<3>::GetRotID( _Y, 1, 2 ),
                              CRotations<3>::GetRotID( _Z, 2, 3 ),
                              CRotations<3>::GetRotID( _Z, 2, 1 ),
                              CRotations<3>::GetRotID( _Y, 1, 2 ),
                              CRotations<3>::GetRotID( _X, 2, 1 ),
                              CRotations<3>::GetRotID( _Y, 1, 2 ),
                              CRotations<3>::GetRotID( _X, 0, 1 )
        };
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
    Insight<3> Test3_1( pattern3_1, 5 );
    Insight<4> Test4_1( pattern4_1, 4 );
    Insight<4> Test4_2( pattern4_2, 4 );
    Test3_1.build();
    Test4_1.build();
    Test4_2.build();
    timerOFF();
    clog( "Build time: ", ellapsed() );
  stamp( true, success );
  
  tcase( " 3x3 cross " );
  stamp( test10Moves( Test3_1, rotations3_1 ), success );
  tcase( " 4x4 diagonal " );
  stamp( test10Moves( Test4_1, rotations4_1 ), success );

  tcase( " 4x4 square " );
  stamp( test10Moves( Test4_2, rotations4_2 ), success );


  finish( "Subgroup", success );

  return success;
}
