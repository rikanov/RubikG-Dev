#ifndef SUBGROUP__H
#define SUBGROUP__H

#include <patch.h>

template< cube_size N >
struct Subgroup
{
  const Patch     patch;
  const GroupID * group;

  Subgroup( const Patch & p, const GroupID * g )
    : patch ( p )
    , group ( g )
  {

  }
};

#endif  //  ! SUBGROUP__H
