#include <test.h>
#include <insight.h>

template< cube_size N >
static void PlayWith( Insight<N> & insight, const int * test, bool & success )
{
  insight.print();
  const int * next = test;
  const int testLines = *( next ++ );
  for( int line = 0; line < testLines; ++ line )
  {
    const RotID rotID = *( next ++ );
    UnitTests::tcase( "Rotation by", std::to_string( rotID ), CRotations<N>::ToString( rotID ) );
    insight.move( rotID );
    insight.print();
    UnitTests::stamp( insight.projected() == *( next ++ ), success );
    UnitTests::stamp( insight.prior() == *( next ++ ), success );
    UnitTests::stamp( 0 <= *( next ++ ), success );
  }
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

  constexpr int baseTest_2[] = {    12,
     13,   1154,  1,  1,   //  { _Z, 0, 1 }
     18,   1731,  1,  1,   //  { _Z, 1, 3 }
      7,   8078,  1,  2,   //  { _Y, 0, 1 }
      6,   8462,  1,  3,   //  { _X, 1, 3 }
     13,   8508,  3,  4,   //  { _Z, 0, 1 }
     16,   8462,  3,  3,   //  { _Z, 1, 1 }
     18,   8508,  3,  4,   //  { _Z, 1, 3 }
     15,   8462,  1,  3,   //  { _Z, 0, 3 }
      4,   8078,  1,  2,   //  { _X, 1, 1 }
      9,   1731,  1,  1,   //  { _Y, 0, 3 }
     16,   1154,  1,  1,   //  { _Z, 1, 1 }
     15,      0,  0,  0,   //  { _Z, 0, 3 }
  };
  constexpr int transTest_2[] = {    12,
      16,    577,  0,  1,   //  { _Z, 1, 1 }
      12,   6337, 16,  2,   //  { _Y, 1, 3 }
       7,  12673, 16,  2,   //  { _Y, 0, 1 }
       5,  12674, 16,  3,   //  { _X, 1, 2 }
      14,  13154, 16,  4,   //  { _Z, 0, 2 }
      10,   6800,  0,  5,   //  { _Y, 1, 1 }
      14,   5075,  3,  6,   //  { _Z, 0, 2 }
      15,   5650,  1,  6,   //  { _Z, 0, 3 }
       5,   8170, 21,  6,   //  { _X, 1, 2 }
       9,   8183, 15,  6,   //  { _Y, 0, 3 }
      10,   8174, 15,  5,   //  { _Y, 1, 1 }
      18,   9881, 15,  5,   //  { _Z, 1, 3 }
};

  constexpr int baseTest_3[] = {    12,
     25,      0,  1,  0,   //  { _Z, 2, 1 }
      5, 317975, 21,  1,   //  { _X, 1, 2 }
     13, 323375,  8,  2,   //  { _Y, 1, 1 }
      6, 322775,  8,  3,   //  { _X, 1, 3 }
     10, 322775,  8,  3,   //  { _Y, 0, 1 }
     26,   8855,  8,  3,   //  { _Z, 2, 2 }
     26, 322775,  8,  3,   //  { _Z, 2, 2 }
     12, 322775,  8,  3,   //  { _Y, 0, 3 }
      4, 323375,  8,  2,   //  { _X, 1, 1 }
     15, 317975, 21,  1,   //  { _Y, 1, 3 }
      5,      0,  1,  0,   //  { _X, 1, 2 }
     27,      0,  0,  0,   //  { _Z, 2, 3 }
  };
  constexpr int transTest_3[] = {    12,
      17, 317952,  0,  1,   //  { _Y, 2, 2 }
      16, 221184,  0,  1,   //  { _Y, 2, 1 }
      16,      0,  0,  0,   //  { _Y, 2, 1 }
       6,      0,  0,  0,   //  { _X, 1, 3 }
      10,      7,  0,  1,   //  { _Y, 0, 1 }
      23,   1804,  3,  2,   //  { _Z, 1, 2 }
      23,      7,  0,  1,   //  { _Z, 1, 2 }
      12,      0,  0,  0,   //  { _Y, 0, 3 }
       4,      0,  0,  0,   //  { _X, 1, 1 }
       9,      0,  0,  0,   //  { _X, 2, 3 }
       8,      0,  0,  0,   //  { _X, 2, 2 }
       7,      0,  0,  0,   //  { _X, 2, 1 }
  };

  constexpr int baseTest_4[] = {     30,
      43,      0,  1,  0,   //  { _Z, 4, 1 }
      45,      0,  0,  0,   //  { _Z, 4, 3 }
      28,   9232,  7,  1,   //  { _Y, 4, 1 }
      30,      0,  0,  0,   //  { _Y, 4, 3 }
      13, 129816, 13,  1,   //  { _X, 4, 1 }
      15,      0,  0,  0,   //  { _X, 4, 3 }
      31,      0,  1,  0,   //  { _Z, 0, 1 }
      33,      0,  0,  0,   //  { _Z, 0, 3 }
      16,  96768,  0,  1,   //  { _Y, 0, 1 }
      18,      0,  0,  0,   //  { _Y, 0, 3 }
       4, 129816, 13,  1,   //  { _X, 1, 1 }
       6,      0,  0,  0,   //  { _X, 1, 3 }
      32,      0,  3,  0,   //  { _Z, 0, 2 }
      35,      0,  3,  0,   //  { _Z, 1, 2 }
       4, 129816,  3,  1,   //  { _X, 1, 1 }
      32, 130416,  0,  2,   //  { _Z, 0, 2 }
      14, 185712, 20,  3,   //  { _X, 4, 2 }
      28, 185728, 20,  4,   //  { _Y, 4, 1 }
      29, 185719, 20,  4,   //  { _Y, 4, 2 }
      43, 213943, 20,  5,   //  { _Z, 4, 1 }
      25, 213823, 20,  6,   //  { _Y, 3, 1 }
      27, 213943, 20,  5,   //  { _Y, 3, 3 }
      15, 266943, 13,  5,   //  { _X, 4, 3 }
      13, 213943, 20,  5,   //  { _X, 4, 1 }
      45, 185719, 20,  4,   //  { _Z, 4, 3 }
      28, 185712, 20,  3,   //  { _Y, 4, 1 }
      14, 130416,  0,  2,   //  { _X, 4, 2 }
      32, 129816,  3,  1,   //  { _Z, 0, 2 }
       6,      0,  3,  0,   //  { _X, 1, 3 }
      44,      0,  0,  0,   //  { _Z, 4, 2 }
};

  constexpr int transTest_4[] = {    14,
      43,    577,  0,  1,   //  { _Z, 4, 1 }
      45,      0,  0,  0,   //  { _Z, 4, 3 }
      28,      0,  7,  0,   //  { _Y, 4, 1 }
      30,      0,  0,  0,   //  { _Y, 4, 3 }
      13, 129816, 13,  1,   //  { _X, 4, 1 }
      15,      0,  0,  0,   //  { _X, 4, 3 }
      20,      0,  0,  0,   //  { _Y, 1, 2 }
      17,      0, 23,  0,   //  { _Y, 0, 2 }
      16,      0, 16,  0,   //  { _Y, 0, 1 }
      16,      0,  0,  0,   //  { _Y, 0, 1 }
      13, 129816, 13,  1,   //  { _X, 4, 1 }
      29, 144243, 13,  2,   //  { _Y, 4, 2 }
      29, 129816, 13,  1,   //  { _Y, 4, 2 }
      15,      0,  0,  0,   //  { _X, 4, 3 }
};

  constexpr int baseTest_5[] = {    12,
     16, 129816, 13,  1,   //  { _X, 5, 1 }
     21,    216,  0,  1,   //  { _X, 6, 3 }
      7,      0,  0,  0,   //  { _X, 2, 1 }
     56,      0,  3,  0,   //  { _Z, 4, 2 }
     37, 230400,  3,  1,   //  { _Y, 5, 1 }
     50, 230400,  3,  1,   //  { _Z, 2, 2 }
     50, 230400,  3,  1,   //  { _Z, 2, 2 }
     39,      0,  3,  0,   //  { _Y, 5, 3 }
     56,      0,  0,  0,   //  { _Z, 4, 2 }
     40, 230400,  7,  1,   //  { _Y, 6, 1 }
     41, 100800, 16,  1,   //  { _Y, 6, 2 }
     40,      0,  0,  0,   //  { _Y, 6, 1 }
};
  constexpr int transTest_5[] = {    12,
      37, 100800,  0,  1,   //  { _Y, 5, 1 }
      42,    384,  0,  1,   //  { _Y, 6, 3 }
      28,      0,  0,  0,   //  { _Y, 2, 1 }
      14, 288500, 20,  1,   //  { _X, 4, 2 }
      37, 288100,  4,  2,   //  { _Y, 5, 1 }
      20, 331303, 16,  3,   //  { _X, 6, 2 }
      20, 288100,  4,  2,   //  { _X, 6, 2 }
      39, 288500, 20,  1,   //  { _Y, 5, 3 }
      14,      0,  0,  0,   //  { _X, 4, 2 }
      40, 100968,  0,  1,   //  { _Y, 6, 1 }
      41, 230784,  0,  1,   //  { _Y, 6, 2 }
      40,      0,  0,  0,   //  { _Y, 6, 1 }
};
  timerON();
 // Rubik 2x2
//-----------
  Insight<2> baseInsight_2 ( toSolve_2, size_2 );
  Insight<2> transInsight_2( toSolve_2, size_2, Simplex::Tilt( _Z, 1 ) );

 // Rubik 3x3
//-----------
  Insight<3> baseInsight_3 ( toSolve_3, size_3 );
  Insight<3> transInsight_3( toSolve_3, size_3, Simplex::Tilt( _Y, 1 ) );

 // Rubik 4x4
//-----------
  Insight<4> baseInsight_4 ( toSolve_4, size_4 );
  Insight<4> transInsight_4( toSolve_4, size_4, Simplex::Tilt( _X, 1 ) );

 // Rubik 5x5
//-----------
  Insight<5> baseInsight_5 ( toSolve_5, size_5 );
  Insight<5> transInsight_5( toSolve_5, size_5, Simplex::Composition( Simplex::Tilt( _X, 2 ), Simplex::Tilt( _Z, 1 ) ) );

  timerOFF();
  tail( "Memory allocation", success );
  clog( Color::cyan, "Ellapsed time:", Color::white, Color::bold, ellapsed(), Color::off );
  NL();

  tcase( "Base tests" );
  PlayWith <2> ( baseInsight_2, baseTest_2, success );
  PlayWith <3> ( baseInsight_3, baseTest_3, success );
//  PlayWith <4> ( baseInsight_4, baseTest_4, success );
//  PlayWith <5> ( baseInsight_5, baseTest_5, success );
  tail( "Base rotations", success );

  tcase( "Transformed tests" );
  PlayWith <2> ( transInsight_2, transTest_2, success );
  PlayWith <3> ( transInsight_3, transTest_3, success );
//  PlayWith <4> ( transInsight_4, transTest_4, success );
//  PlayWith <5> ( transInsight_5, transTest_5, success );
  tcase( "Transformed tests" );
  
  finish( "CacheIDmapper & Insight", success );
  return success;
}
