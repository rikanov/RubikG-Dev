#include <test.h>
#include <insight.h>


bool UnitTests::unit_Insight() const
{

  bool success = true;
  head( "CacheIDmapper & Insight" );

  tcase( "Memory allocation" );

  SubSpace toSolve = {
                 CPositions<4>::GetPosID( 1, 1, 0 ),
                 CPositions<4>::GetPosID( 1, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 1, 0 ),
                 CPositions<4>::GetPosID( 2, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 0, 0 )
                };

  Insight<4> baseInsight_4 ( toSolve );
  Insight<4> transInsight_4( toSolve, Simplex::Tilt( _X, 1 ) );

  Rubik<4> test4;
  baseInsight_4.set ( test4 );
  transInsight_4.set( test4 );

  tail( "Memory allocation", success );

  tcase( "Base tests" );
  
  const std::initializer_list<RotID>
  baseRotations_4 = {
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

  baseInsight_4.print();
  const int * next = expectedBaseResults_4;
  for( const RotID rotID : baseRotations_4 )
  {
    tcase( "Rotation by", CExtRotations<4>::ToString( rotID ) );
    baseInsight_4.rotate( rotID );
    baseInsight_4.print();
    // printf( "%7i,%3i, %3i,   //  %s\n", baseInsight_4.state(), baseInsight_4.prior(), baseInsight_4.distance(), CExtRotations<4>::ToString( rotID ).c_str() );
    clog_( "State:", baseInsight_4.state(), "\t\tPrior position:", baseInsight_4.priorCube().toString(), "  Depth:", baseInsight_4.distance() );
    stamp( baseInsight_4.state()    == *( next ++ ) && 
           baseInsight_4.prior()    == *( next ++ ) && 
           baseInsight_4.distance() == *( next ++ )
           , success );
  }

  tail( "Base rotations", success );

  tcase( "Transformed tests" );

  const std::initializer_list<RotID> 
  transRotations_4 = { 
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

  transInsight_4.print();
  next = expectedTransResults_4;
  for( const RotID rotID : transRotations_4 )
  {
    tcase( "Operation", CExtRotations<4>::ToString( rotID ) );
    transInsight_4.rotate( rotID );
    transInsight_4.print();
    // printf( "%7i,%3i, %3i,   //  %s\n", transInsight_4.state(), transInsight_4.prior(), transInsight_4.distance(), CExtRotations<4>::ToString( rotID ).c_str() );
    clog( "State:", transInsight_4.state(), "\t\tPrior position:", transInsight_4.priorCube().toString(), "  Depth:", transInsight_4.distance() );
    stamp( transInsight_4.state()    == *( next ++ ) && 
           transInsight_4.prior()    == *( next ++ ) && 
           transInsight_4.distance() == *( next ++ )
           , success );
  }
  
  finish( "CacheIDmapper & Insight", success );
  return success;
}
