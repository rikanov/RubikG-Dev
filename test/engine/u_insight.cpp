#include <test.h>
#include <insight.h>


bool UnitTests::unit_Insight() const
{

  bool success = true;
  head( "CacheIDmapper" );

  tcase( "Memory allocation" );

  SubSpace toSolve = {
                 CPositions<4>::GetPosID( 1, 1, 0 ),
                 CPositions<4>::GetPosID( 1, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 1, 0 ),
                 CPositions<4>::GetPosID( 2, 2, 0 ),
                 CPositions<4>::GetPosID( 2, 0, 0 )
                };

  Insight<4> baseInsight( toSolve );
  Insight<4> transInsight( toSolve, Simplex::Tilt( _X, 1 ) );

  Rubik<4> test4;
  baseInsight.set( test4 );
  transInsight.set( test4 );

  tail( "Memory allocation", success );

  tcase( "Base tests" );
  
  const std::initializer_list<RotID>
  baseRotations = {
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
    CExtRotations<4>::GetRotID( _X, 0, 1 ), 
    CExtRotations<4>::GetRotID( _X, 0, 3 ), 
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

  const CacheID stateResults[] = {
    0, 0, 129816, 130416, 185712, 185719
  };

  baseInsight.print();
  for( const RotID rotID : baseRotations )
  {
    tcase( "Operation", CExtRotations<4>::ToString( rotID ) );
    baseInsight.rotate( rotID );
    baseInsight.print();
    clog( "State:\t", baseInsight.state(), "\tPrior position:\t", baseInsight.priorCube().toString(), "\tDepth:\t", baseInsight.distance() );
    stamp( true, success );
  }

  tail( "Base rotations", success );

  tcase( "Transformed tests" );

  const std::initializer_list<RotID> 
  transRotations = { 
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

  transInsight.print();
  for( const RotID rotID : transRotations )
  {
    tcase( "Operation", CExtRotations<4>::ToString( rotID ) );
    transInsight.rotate( rotID );
    transInsight.print();
    clog( "State:\t", transInsight.state(), "\tPrior position:\t", transInsight.priorCube().toString(), "\tDepth:\t", transInsight.distance() );
    stamp( true, success );
  }
  
  finish( "CacheIDmapper", success );
  return success;
}
