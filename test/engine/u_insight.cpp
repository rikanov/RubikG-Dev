#include <test.h>
#include <insight.h>

using Trial = const std::initializer_list< RotID > &;

template< size_t N >
static void PlayWith( Insight<N> insight, Trial trial, const int * expectation, bool & success )
{
  insight.print();
  const int * next = expectation;
  for( const RotID rotID : trial )
  {
    UnitTests::tcase( "Rotation by", CExtRotations<N>::ToString( rotID ) );
    insight.rotate( rotID );
    insight.print();
    // printf( "%7i,%3i, %3i,   //  %s\n", baseInsight_4.state(), baseInsight_4.prior(), baseInsight_4.distance(), CExtRotations<4>::ToString( rotID ).c_str() );
    clog( "State:", insight.state(), "\t\tPrior position:", insight.priorCube().toString(), "  Depth:", insight.distance() );
    UnitTests::stamp(
           insight.state()    == *( next ++ ) &&
           insight.prior()    == *( next ++ ) &&
           insight.distance() == *( next ++ )
           , success );
  }
}

bool UnitTests::unit_Insight() const
{

  bool success = true;
  head( "CacheIDmapper & Insight" );

  tcase( "Memory allocation" );

  SubSpace toSolve_2 = {
                  CPositions<2>::GetPosID( 0, 0, 0 ),
                  CPositions<2>::GetPosID( 0, 0, 1 ),
                  CPositions<2>::GetPosID( 0, 1, 0 ),
                  CPositions<2>::GetPosID( 0, 1, 1 )
                };

  SubSpace toSolve_3 = {
                CPositions<3>::GetPosID( 1, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
                  CPositions<3>::GetPosID( 2, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 2, 2 )
                };

  SubSpace toSolve_4 = {
                  CPositions<4>::GetPosID( 1, 1, 0 ),
                  CPositions<4>::GetPosID( 1, 2, 0 ),
                  CPositions<4>::GetPosID( 2, 1, 0 ),
                  CPositions<4>::GetPosID( 2, 2, 0 ),
                  CPositions<4>::GetPosID( 2, 0, 0 )
                };

  SubSpace toSolve_5 = {
                  CPositions<5>::GetPosID( 0, 0, 0 ),
                  CPositions<5>::GetPosID( 0, 0, 4 ),
                  CPositions<5>::GetPosID( 0, 4, 4 ),
                  CPositions<5>::GetPosID( 0, 4, 0 )
                };

  Trial baseTrial_2 = {
                  CExtRotations<2>::GetRotID( _Z, 0, 1 ),
                  CExtRotations<2>::GetRotID( _Z, 1, 3 ),
                  CExtRotations<2>::GetRotID( _Y, 0, 1 ),
                  CExtRotations<2>::GetRotID( _X, 1, 3 ),
                  CExtRotations<2>::GetRotID( _Z, 0, 1 ),
                  CExtRotations<2>::GetRotID( _Z, 1, 1 )
                };

  constexpr int expectedBaseResults_2[] = {
                        0,  1,   0,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 1, 3 }
                     9232,  7,   1,   //  { _Y, 0, 1 }
                        0,  0,   0,   //  { _X, 1, 3 }
                   129816, 13,   1,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 1, 1 }
                        0,  1,   0,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 0, 3 }
                    96768,  0,   1,   //  { _Y, 0, 1 }
                  };

  Trial baseTrial_3 = {
                CExtRotations<3>::GetRotID( _Z, 2, 1 ),
                CExtRotations<3>::GetRotID( _X, 1, 2 ),
                CExtRotations<3>::GetRotID( _Y, 1, 1 ),
                CExtRotations<3>::GetRotID( _X, 1, 3 ),
                CExtRotations<3>::GetRotID( _Y, 0, 1 ),
                CExtRotations<3>::GetRotID( _Z, 2, 2 )
              };

  constexpr int expectedBaseResults_3[] = {
                        0,  1,   0,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 1, 3 }
                     9232,  7,   1,   //  { _Y, 0, 1 }
                        0,  0,   0,   //  { _X, 1, 3 }
                   129816, 13,   1,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 1, 1 }
                        0,  1,   0,   //  { _Z, 0, 1 }
                        0,  0,   0,   //  { _Z, 0, 3 }
                    96768,  0,   1,   //  { _Y, 0, 1 }
                };

  Trial baseTrial_4 = {
                CExtRotations<4>::GetRotID( _Z, 4, 1 ),
                CExtRotations<4>::GetRotID( _Z, 4, 3 ),
                CExtRotations<4>::GetRotID( _Y, 4, 1 ),
                CExtRotations<4>::GetRotID( _Y, 4, 3 ),
                CExtRotations<4>::GetRotID( _X, 4, 1 ),
                CExtRotations<4>::GetRotID( _X, 4, 3 ),
                CExtRotations<4>::GetRotID( _Z, 0, 1 ),
                CExtRotations<4>::GetRotID( _Z, 0, 3 ),
                CExtRotations<4>::GetRotID( _Y, 0, 1 ),
                CExtRotations<4>::GetRotID( _Y, 0, 3 ),
                CExtRotations<4>::GetRotID( _X, 1, 1 ),
                CExtRotations<4>::GetRotID( _X, 1, 3 ),
                CExtRotations<4>::GetRotID( _Z, 0, 2 ),
                CExtRotations<4>::GetRotID( _Z, 1, 2 ),
                CExtRotations<4>::GetRotID( _X, 1, 1 ),
                CExtRotations<4>::GetRotID( _Z, 0, 2 ),
                CExtRotations<4>::GetRotID( _X, 4, 2 ),
                CExtRotations<4>::GetRotID( _Y, 4, 1 ),
                CExtRotations<4>::GetRotID( _Y, 4, 2 ),
                CExtRotations<4>::GetRotID( _Z, 4, 1 ),
                CExtRotations<4>::GetRotID( _Y, 3, 1 ),
                CExtRotations<4>::GetRotID( _Y, 3, 3 ),
                CExtRotations<4>::GetRotID( _X, 4, 3 ),
                CExtRotations<4>::GetRotID( _X, 4, 1 ),
                CExtRotations<4>::GetRotID( _Z, 4, 3 ),
                CExtRotations<4>::GetRotID( _Y, 4, 1 ),
                CExtRotations<4>::GetRotID( _X, 4, 2 ),
                CExtRotations<4>::GetRotID( _Z, 0, 2 ),
                CExtRotations<4>::GetRotID( _X, 1, 3 ),
                CExtRotations<4>::GetRotID( _Z, 4, 2 )
              };

  constexpr int expectedBaseResults_4[] = {
                      0,  1,   0,   //  { _Z, 4, 1 }
                      0,  0,   0,   //  { _Z, 4, 3 }
                   9232,  7,   1,   //  { _Y, 4, 1 }
                      0,  0,   0,   //  { _Y, 4, 3 }
                 129816, 13,   1,   //  { _X, 4, 1 }
                      0,  0,   0,   //  { _X, 4, 3 }
                      0,  1,   0,   //  { _Z, 0, 1 }
                      0,  0,   0,   //  { _Z, 0, 3 }
                  96768,  0,   1,   //  { _Y, 0, 1 }
                      0,  0,   0,   //  { _Y, 0, 3 }
                 129816, 13,   1,   //  { _X, 1, 1 }
                      0,  0,   0,   //  { _X, 1, 3 }
                      0,  3,   0,   //  { _Z, 0, 2 }
                      0,  3,   0,   //  { _Z, 1, 2 }
                 129816,  3,   1,   //  { _X, 1, 1 }
                 130416,  0,   2,   //  { _Z, 0, 2 }
                 185712, 20,   3,   //  { _X, 4, 2 }
                 185728, 20,   4,   //  { _Y, 4, 1 }
                 185719, 20,   4,   //  { _Y, 4, 2 }
                 213943, 20,   5,   //  { _Z, 4, 1 }
                 213823, 20,   6,   //  { _Y, 3, 1 }
                 213943, 20,   5,   //  { _Y, 3, 3 }
                 266943, 13,   5,   //  { _X, 4, 3 }
                 213943, 20,   5,   //  { _X, 4, 1 }
                 185719, 20,   4,   //  { _Z, 4, 3 }
                 185712, 20,   3,   //  { _Y, 4, 1 }
                 130416,  0,   2,   //  { _X, 4, 2 }
                 129816,  3,   1,   //  { _Z, 0, 2 }
                      0,  3,   0,   //  { _X, 1, 3 }
                      0,  0,   0,   //  { _Z, 4, 2 }
            };

  Trial transRotations_4 = {
                CExtRotations<4>::GetRotID( _Z, 4, 1 ),
                CExtRotations<4>::GetRotID( _Z, 4, 3 ),
                CExtRotations<4>::GetRotID( _Y, 4, 1 ),
                CExtRotations<4>::GetRotID( _Y, 4, 3 ),
                CExtRotations<4>::GetRotID( _X, 4, 1 ),
                CExtRotations<4>::GetRotID( _X, 4, 3 ),
                CExtRotations<4>::GetRotID( _Y, 1, 2 ),
                CExtRotations<4>::GetRotID( _Y, 0, 2 ),
                CExtRotations<4>::GetRotID( _Y, 0, 1 ),
                CExtRotations<4>::GetRotID( _Y, 0, 1 ),
                CExtRotations<4>::GetRotID( _X, 4, 1 ),
                CExtRotations<4>::GetRotID( _Y, 4, 2 ),
                CExtRotations<4>::GetRotID( _Y, 4, 2 ),
                CExtRotations<4>::GetRotID( _X, 4, 3 )
              };

  const int expectedTransResults_4[] = {
                      577,  0,   1,   //  { _Z, 4, 1 }
                        0,  0,   0,   //  { _Z, 4, 3 }
                        0,  7,   0,   //  { _Y, 4, 1 }
                        0,  0,   0,   //  { _Y, 4, 3 }
                   129816, 13,   1,   //  { _X, 4, 1 }
                        0,  0,   0,   //  { _X, 4, 3 }
                        0,  0,   0,   //  { _Y, 1, 2 }
                        0, 23,   0,   //  { _Y, 0, 2 }
                        0, 16,   0,   //  { _Y, 0, 1 }
                        0,  0,   0,   //  { _Y, 0, 1 }
                   129816, 13,   1,   //  { _X, 4, 1 }
                   144243, 13,   2,   //  { _Y, 4, 2 }
                   129816, 13,   1,   //  { _Y, 4, 2 }
                        0,  0,   0,   //  { _X, 4, 3 }
              };
  timerON();

 // Rubik 2x2
//-----------
  Rubik<2> test_2;
  Insight<2> baseInsight_2 ( toSolve_2 );
  Insight<2> transInsight_2( toSolve_2, Simplex::Tilt( _Z, 1 ) );
  baseInsight_2.set( test_2 );
  transInsight_2.set( test_2 );

 // Rubik 3x3
//-----------
  Rubik<3> test_3;
  Insight<3> baseInsight_3 ( toSolve_3 );
  Insight<3> transInsight_3( toSolve_3, Simplex::Tilt( _Y, 1 ) );
  baseInsight_3.set( test_3 );
  transInsight_3.set( test_3 );

 // Rubik 4x4
//-----------
  Rubik<4> test_4;
  Insight<4> baseInsight_4 ( toSolve_4 );
  Insight<4> transInsight_4( toSolve_4, Simplex::Tilt( _X, 1 ) );
  baseInsight_4.set( test_4 );
  transInsight_4.set( test_4 );

 // Rubik 5x5
//-----------
  Rubik<5> test_5;
  Insight<5> baseInsight_5 ( toSolve_5 );
  Insight<5> transInsight_5( toSolve_5, Simplex::Tilt( _X, 2 ) );
  baseInsight_5.set ( test_5 );
  transInsight_5.set( test_5 );

  timerOFF();
  tail( "Memory allocation", success );
  clog( Color::cyan, "Ellapsed time:", Color::white, Color::bold, ellapsed(), Color::off );
  NL();

  tcase( "Base tests" );
  PlayWith <2> ( baseInsight_2, baseTrial_2, expectedBaseResults_2, success );
  PlayWith <3> ( baseInsight_3, baseTrial_3, expectedBaseResults_3, success );
  PlayWith <4> ( baseInsight_4, baseTrial_4, expectedBaseResults_4, success );
  tail( "Base rotations", success );

  tcase( "Transformed tests" );
  PlayWith <4> ( transInsight_4, transRotations_4, expectedTransResults_4, success );
  tcase( "Transformed tests" );
  
  finish( "CacheIDmapper & Insight", success );
  return success;
}
