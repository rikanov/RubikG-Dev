#include <test.h>
#include <bitmap_set.h>
#include <insight.h>


template< cube_size N >
static void ShowGradients ( const Insight<N> & insight )
{
  const DistID D = insight.distance();
  clog( "depth:", (int) D );
  BitMap::Print( insight.gradient( D     ), 9 * N, 3 * N );
  BitMap::Print( insight.gradient( D + 1 ), 9 * N, 3 * N );
}

template< cube_size N >
static bool CheckInsight( Insight<N> & insight )
{
  constexpr size_t NUMBER_OF_TESTS = 6;
  bool ok = true;
  NL(); NL();
  insight.print();
  DistID deep[ NUMBER_OF_TESTS ] = {};
  RotID  test[ NUMBER_OF_TESTS ] = {};
  for ( int i = 0; i < NUMBER_OF_TESTS; ++ i)
  {
    const RotID t = CRotations<N>::Random();
    deep[i] = insight.distance();
    test[i] = t;
    clog_( CRotations<N>::ToString( t ), "-->" );
    insight.move( t );
    insight.print();
    ShowGradients( insight );
  }
  for ( int i = 1; i <= NUMBER_OF_TESTS; ++ i)
  {
    const RotID t = CRotations<N>::GetInvRotID( test[ NUMBER_OF_TESTS - i ] );
    clog_( CRotations<N>::ToString( t ), "-->" );
    insight.move( t );
    insight.print();
    const DistID D = insight.distance();
    ShowGradients( insight );
    UnitTests::stamp ( insight.distance() == deep[ NUMBER_OF_TESTS - i ] , ok );
  }
  return ok;
}


template< cube_size N >
static bool CheckGradient( Insight<N> & insight )
{
  bool ok = true;
  constexpr size_t NUMBER_OF_TESTS = 6;
  NL();
  for ( int t = 0; t < NUMBER_OF_TESTS; ++ t )
  {
    for( int rotNum = UnitTests::random( 3 * N, 5 * N ); rotNum > 0; -- rotNum )
    {
      insight.move( CRotations<N>::Random() ); 
    }
    ShowGradients( insight );
  }
  return ok;
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
  baseInsight_2.build();

 // Rubik 3x3
//-----------
  Insight<3> baseInsight_3 ( toSolve_3, size_3 );
  baseInsight_3.build();

 // Rubik 4x4
//-----------
  Insight<4> baseInsight_4 ( toSolve_4, size_4 );
  baseInsight_4.build();

 // Rubik 5x5
//-----------
  Insight<5> baseInsight_5 ( toSolve_5, size_5 );
  baseInsight_5.build();

  timerOFF();
  tail( "Memory allocation", success );
  clog( Color::cyan, "Ellapsed time:", Color::white, Color::bold, ellapsed(), Color::off );
  NL();

  tcase( "Base tests" );
  success &= CheckInsight <2> ( baseInsight_2 );
  success &= CheckInsight <3> ( baseInsight_3 );
  success &= CheckInsight <4> ( baseInsight_4 );
  success &= CheckInsight <5> ( baseInsight_5 );
  tail( "Base rotations", success );

  tcase( "Check gradients" );
  success &= CheckGradient <2> ( baseInsight_2 );
  success &= CheckGradient <3> ( baseInsight_3 );
  success &= CheckGradient <4> ( baseInsight_4 );
  success &= CheckGradient <5> ( baseInsight_5 );
  tail( "Ceck gradients" , success );

  finish( "CacheIDmapper & Insight", success );
  return success;
}
