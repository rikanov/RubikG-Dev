#ifndef PROJECTION__H
#define PROJECTION__H

#include <base_types.h>

static constexpr size_t SUBGROUP_SIZE_BOUND = 6;

class Projection
{
  static const Projection * Singleton;
  
  const GroupID ** m_projection;
  
  Projection();
  ~Projection();
  
  void init( const size_t );
    
  GroupID lookUp( const size_t dim, const GroupID gid ) const
  {
    return m_projection[ dim ][ gid ];
  }
  
public:
  static void Instance( void );
  static void OnExit( void )
  {
    delete Singleton; 
    Singleton = nullptr;
  }
  
  static GroupID LookUp( const size_t dim, const GroupID gid )
  {
    return Singleton -> lookUp( dim, gid );
  }
};

#endif  //  ! PROJECTION__H
