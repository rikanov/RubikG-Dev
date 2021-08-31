#include <simplex.h>
#include <projection.h>

const Projection * Projection::Singleton = nullptr;

Projection::Projection()
 : m_projection( new const GroupID * [ SUBGROUPS_MAX_SIZE ] )
{
  
}

void Projection::Instance( void )
{
  if( Singleton == nullptr )
  {
    Projection * singleton = new Projection();
    for( size_t i = 1; i < SUBGROUPS_MAX_SIZE; ++ i )
      singleton -> init( i );
    Singleton = singleton;
  }
}

void Projection::init( const size_t size )
{
  GroupID * projection = new GroupID[ pow24( size ) ]{};
  
  all_cubeid( prior )
  {
    const CubeID inv = Simplex::Inverse( prior );
    for( size_t next = 0; next < pow24( size - 1 ); ++ next )
    {
      for( GroupID stateID = next, radix = 1; stateID > 0; stateID /= 24, radix *= 24 )
      {
        projection[ next ] += Simplex::Composition( stateID % 24, inv ) * radix;
      }
    }
  }
  m_projection[ size ] = projection;
}

Projection::~Projection()
{
  for( size_t i = 0; i < SUBGROUPS_MAX_SIZE; ++ i )
    delete[] m_projection[i];
  
  delete[] m_projection;
}