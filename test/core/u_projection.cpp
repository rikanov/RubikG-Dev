#include <test.h>
#include <simplex.h>
#include <projection.h>

static void RandomGroupID( GroupID & normal, GroupID & projected, size_t size )
{  
  const CubeID prior = Simplex::Random();
  normal    = prior * pow24( -- size );
  projected = 0;
  while( 0 < size -- )
  {
    const CubeID random = Simplex::Random();
    normal    += Simplex::Composition( random, prior ) * pow24( size );
    projected += random * pow24( size );
  }
}

bool UnitTests::unit_Projection() const
{
  bool success = true;
  head( "Subgroup projections" );

  for ( size_t size = 2; size < 6; ++ size )
  {
    bool ok = true;
    GroupID normal, projected;
    
    tcase( "Projection", "size", size );
    for ( int i = 0; i < 10; ++ i )
    {
      RandomGroupID( normal, projected, size );
      clog_( Color::cyan, numR( normal, size + 2 ), Color::yellow, "-->", Color::cyan, numR( projected, size + 1 ) );
      stamp( Projection::LookUp( size, normal ) == projected, ok );
    }
    success &= ok;
    tail( "Projection", success );
  }
  
  finish( "Subgroup projections", success );
  return success;
}
