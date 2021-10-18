#ifndef SNAPSHOTS__H
#define SNAPSHOTS__H

#include <base_types.h>
#include <bitmap_set.h>

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

  void print( const cube_size N )
  {
    BitMap::Print_( target, 24, 4 );
    clog_( " / " );
    gradient.print( 9 * N + 1, 3 * N );
  }

  ~Snapshots()
  {
    delete[] state;
    delete[] prior;
  }
};

#endif  //  ! SNAPSHOTS__H
