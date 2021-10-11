#ifndef SNAPSHOTS__H
#define SNAPSHOTS__H

#include <base_types.h>

struct Snapshots
{
  static constexpr size_t MaximumSubgroups = 40;

  BitMap     gradient;
  BitMap32ID target;
  
  GroupID * state;
  CubeID  * prior;
  
  RotID step;

public:

  Snapshots()
   :  target ( ( 1 << 24 ) - 1 )
   ,  state  ( new GroupID [ MaximumSubgroups ] )
   ,  prior  ( new CubeID  [ MaximumSubgroups ] )
  {
  }

  ~Snapshots()
  {
    delete[] state;
    delete[] prior;
  }
};

#endif  //  ! SNAPSHOTS__H