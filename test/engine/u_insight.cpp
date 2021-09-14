#include <test.h>
#include <bitmap_set.h>
#include <insight.h>

template< cube_size N >
static void PlayWith( Insight<N> & insight, bool & success )
{
  bool ok = true;
  const std::string tCaseName( "Insight<"+numL( N, 1 ) + "> size " + numL( insight.size(), 1 ) );
  NL(); NL();
  UnitTests::tcase( tCaseName );
  insight.print();
  constexpr size_t NUMBER_OF_TESTS = 6;
  RotID test[ NUMBER_OF_TESTS ] = {};
  for ( int i = 0; i < NUMBER_OF_TESTS; ++ i)
  {
    const RotID t = CRotations<N>::Random();
    test[i] = t;
    clog_( CRotations<N>::ToString( t ), "-->" );
    insight.move( t );
    clog( "stateID:", insight.state() );
    insight.print();
    clog( "depth:", (int) insight.distance() );
    BitMap::Print( insight.gradient(), 9 * N, 3 * N );
  }
  for ( int i = 1; i <= NUMBER_OF_TESTS; ++ i)
  {
    const RotID t = CRotations<N>::GetInvRotID( test[ NUMBER_OF_TESTS - i ] );
    clog_( CRotations<N>::ToString( t ), "-->" );
    insight.move( t );
    clog( "stateID:", insight.state() );
    insight.print();
    clog( "depth:", (int) insight.distance() );
    BitMap::Print( insight.gradient(), 9 * N, 3 * N );
  }
  UnitTests::stamp( insight.distance() == 0, ok );
  UnitTests::tail( tCaseName, ok );
  success &= ok;
}

bool UnitTests::unit_Insight() const
{

  bool success = true;
  head( "CacheIDmapper & Insight" );

  tcase( "Memory allocation" );

  const PosID toSolve_2[] = {
                  CPositions<2>::GetPosID( 0, 0, 0 ),
                  CPositions<2>::GetPosID( 0, 0, 1 ),
                  CPositions<2>::GetPosID( 0, 1, 0 ),
                  CPositions<2>::GetPosID( 0, 1, 1 )
                };
  constexpr size_t size_2 = 4;
  
  const PosID toSolve_3[] = {
                  CPositions<3>::GetPosID( 1, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 0, 2 ),
                  CPositions<3>::GetPosID( 0, 1, 2 ),
                  CPositions<3>::GetPosID( 2, 1, 2 ),
                  CPositions<3>::GetPosID( 1, 2, 2 )
                };
  constexpr size_t size_3 = 5;
  
  const PosID toSolve_4[] = {
                  CPositions<4>::GetPosID( 1, 1, 0 ),
                  CPositions<4>::GetPosID( 1, 2, 0 ),
                  CPositions<4>::GetPosID( 2, 1, 0 ),
                  CPositions<4>::GetPosID( 2, 2, 0 ),
                  CPositions<4>::GetPosID( 2, 0, 0 )
                };
  constexpr size_t size_4 = 5;
  
  const PosID toSolve_5[] = {
                  CPositions<5>::GetPosID( 0, 0, 4 ),
                  CPositions<5>::GetPosID( 1, 1, 4 ),
                  CPositions<5>::GetPosID( 2, 2, 4 ),
                  CPositions<5>::GetPosID( 3, 3, 4 ),
                  CPositions<5>::GetPosID( 4, 4, 4 )
                };
  constexpr size_t size_5 = 5;

  timerON();
 // Rubik 2x2
//-----------
  Insight<2> baseInsight_2 ( toSolve_2, size_2 );
  Insight<2> transInsight_2( toSolve_2, size_2, Simplex::Tilt( _Z, 1 ) );
  baseInsight_2.build();
  transInsight_2.build();

 // Rubik 3x3
//-----------
  Insight<3> baseInsight_3 ( toSolve_3, size_3 );
  Insight<3> transInsight_3( toSolve_3, size_3, Simplex::Tilt( _Y, 1 ) );
  baseInsight_3.build();
  transInsight_3.build();

 // Rubik 4x4
//-----------
  Insight<4> baseInsight_4 ( toSolve_4, size_4 );
  Insight<4> transInsight_4( toSolve_4, size_4, Simplex::Tilt( _X, 1 ) );
  baseInsight_4.build();
  transInsight_4.build();

 // Rubik 5x5
//-----------
  Insight<5> baseInsight_5 ( toSolve_5, size_5 );
  Insight<5> transInsight_5( toSolve_5, size_5, Simplex::Composition( Simplex::Tilt( _X, 2 ), Simplex::Tilt( _Z, 1 ) ) );
  baseInsight_5.build();
  transInsight_5.build();

  timerOFF();
  tail( "Memory allocation", success );
  clog( Color::cyan, "Ellapsed time:", Color::white, Color::bold, ellapsed(), Color::off );
  NL();

  tcase( "Base tests" );
  PlayWith <2> ( baseInsight_2, success );
  PlayWith <3> ( baseInsight_3, success );
//  PlayWith <4> ( baseInsight_4, success );
//  PlayWith <5> ( baseInsight_5, success );
  tail( "Base rotations", success );

  tcase( "Transformed tests" );
//  PlayWith <2> ( transInsight_2, success );
//  PlayWith <3> ( transInsight_3, success );
//  PlayWith <4> ( transInsight_4, success );
//  PlayWith <5> ( transInsight_5, success );
  tcase( "Transformed tests" );

  finish( "CacheIDmapper & Insight", success );
  return success;
}
