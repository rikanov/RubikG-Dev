#include <test.h>
#include <simplex.h>

bool UnitTests::unit_Simplex() const
{
  bool success = true;
  
  head( "Simplex" );
  NL();
  tcase( "Testing consistency" );
  int counter[24] = {};
  all_cubeid( a )
    all_cubeid( b )
    {
      ++ counter[ Simplex::Composition( a, b) ];
    }
  all_cubeid( cid )
  {
    const bool ok = ( 24 == counter[ cid ] );
    clog_( Simplex::GetCube( cid ).toString() );
    stamp( ok, success );
  }
  tail( "Consistency test", success );

  tcase( "Check cube tilting" );
  all_cubeid( cid )
  {
    bool ok = true;
    clog_( Color::blue, Simplex::GetCube( cid ).toString(), Color::gray, "-->" );
    for ( Axis axis: { _X, _Y, _Z } )
    {
      clog_( Color::off, Color::cyan, '|' );
      for ( Turn turn: { 1, 2, 3 } )
      {
        if ( Simplex::Tilt( cid, axis, turn ) == Simplex::Composition( cid, Simplex::Tilt( axis, turn ) ) )
        {
          clog_( Color::dark );
        }
        else
        {
          clog_( Color::red, Color::flash );
          ok = false;
        }
        clog_( Simplex::GetCube( Simplex::Tilt( cid, axis, turn ) ).toString() );
      }
      clog_( Color::off, Color::cyan, '|' );
    }
    stamp( ok, success );
  }

  tail( "Check cube tilting", success );

  tcase( "Testing group operations" );
  // test cases
  Orient testCases[10][3][2]  = 
                              {/*       A           B      expected AxB  */
                                 { { _R, _U }, { _R, _U }, { _R, _U } }
                                ,{ { _R, _U }, { _B, _D }, { _B, _D } }
                                ,{ { _F, _U }, { _R, _U }, { _F, _U } }
                                ,{ { _L, _D }, { _L, _D }, { _R, _U } }
                                ,{ { _B, _R }, { _L, _U }, { _F, _R } }
                                ,{ { _D, _F }, { _L, _F }, { _U, _L } }
                                ,{ { _L, _B }, { _L, _B }, { _R, _U } }
                                ,{ { _U, _L }, { _U, _B }, { _L, _B } }
                                ,{ { _F, _D }, { _D, _L }, { _U, _B } }
                                ,{ { _D, _F }, { _B, _L }, { _R, _U } }
                              };
  
  for( auto c: testCases )
  {
    const Orient rA = c[0][0], uA = c[0][1];
    const Orient rB = c[1][0], uB = c[1][1];
    const Orient rC = c[2][0], uC = c[2][1];
    CubeID A = Simplex::GetGroupID ( rA , uA );
    CubeID B = Simplex::GetGroupID ( rB , uB );
    CubeID C = Simplex::GetGroupID ( rC , uC );
    CubeID X = Simplex::Composition( A  , B  );
    clog_( Color::blue, Simplex::GetCube( A ), 'X', Simplex::GetCube( B ), '=', Simplex::GetCube( C ), '\t' );
    stamp( C == X, success );
  }
  
  tail( "Group operation test", success );
  
  tcase( "Testing inverses" );
  bool resultInverse = true;
  all_cubeid( cube )
  {
	  CubeID inv = Simplex::Inverse( cube );
	  clog_( Color::blue, "The inverse of", Simplex::GetCube( cube ), "is", Simplex::GetCube( inv ) );
	  stamp( Simplex::Composition( cube, inv ) == 0 /* identity */, resultInverse );
  }
  tail( "Test inverse function" , resultInverse );
  
  const bool result = success && resultInverse;
  finish( "Simplex", result );
  return result;
}
