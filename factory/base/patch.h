#ifndef ___CUBE_PATCH__H
#define ___CUBE_PATCH__H

#include <base_types.h>
#include <dynamic_array.h>
#include <bool_array.h>

class Patch: public Array<PosID>
{

public:

  bool getTransposition( const Patch &, CubeID & ) const;
};

bool Patch::getTransposition( const Patch & patch, CubeID & trans ) const
{
  bool result = true;
  return result;
}

#endif  //  ! ___CUBE_PATCH__H
