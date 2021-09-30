#include <simplex.h>
#include <projection.h>

const Projection * Projection::Singleton = nullptr;

Projection::Projection()
 : m_projection( new const GroupID * [ SUBGROUP_SIZE_BOUND ] {} )
{
  
}

void Projection::Instance( void )
{
  if( Singleton == nullptr )
  {
    Projection * singleton = new Projection();
    for( size_t i = 1; i < SUBGROUP_SIZE_BOUND; ++ i )
      singleton -> init( i );
    Singleton = singleton;
  }
}

void Projection::init( const size_t size )
{
  GroupID * projection = new GroupID[ pow24( size ) ]{};
  GroupID next = 0;
  all_cubeid( prior )
  {
    if ( 0 == prior )
    {
      for ( ; next < pow24( size - 1 ); ++ next )
      {
        projection[ next ] = next;
      }
      continue;
    }
    const CubeID inv = Simplex::Inverse( prior );
    for ( size_t i = 0; i < pow24( size - 1 ); ++ i, ++ next )
    {
      for ( GroupID stateID = next, radix = 0; radix < size - 1; stateID /= 24, ++ radix )
      {
        projection[ next ] += Simplex::Composition( stateID % 24, inv ) * pow24( radix );
      }
    }
  }
  m_projection[ size ] = projection;
}

Projection::~Projection()
{
  for( size_t i = 0; i < SUBGROUP_SIZE_BOUND; ++ i )
    delete[] m_projection[i];
  
  delete[] m_projection;
}
