#include <test.h>
#include <simplex.h>

bool UnitTests::unit_Simplex() const
{
  bool resultGroup = true;
  
  head( "Simplex" );
  NL();
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
    stamp( C == X, resultGroup );
  }
  
  tail( "Group operation test", resultGroup );
  
  tcase( "Testing inverses" );
  bool resultInverse = true;
  all_cubeid( cube )
  {
	  CubeID inv = Simplex::Inverse( cube );
	  clog_( Color::blue, "The inverse of", Simplex::GetCube( cube ), "is", Simplex::GetCube( inv ) );
	  stamp( Simplex::Composition( cube, inv ) == 0 /* identity */, resultInverse );
  }
  tail( "Test inverse function" , resultInverse );
  
  const bool result = resultGroup && resultInverse;
  finish( "Simplex", result );
  return result;
}
