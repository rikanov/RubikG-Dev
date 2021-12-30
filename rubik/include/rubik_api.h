#ifndef ___RUBIK_API__H
#define ___RUBIK_API__H

#include <rubik_group.h>

template< cube_size N >
class RubikAPI:  public RubikGroup<N>
{
  OCube getCube( const PosID ) const;
  Orient getOrient( const Orient, const Orient, const Layer, const Layer ) const;
  void show( const Orient ) const;
  void show( const Orient, const Orient, const Layer, const Layer ) const;
  void print( const Orient, const Orient ) const;
public:

  void print( const bool sep = true ) const;
};

// include temnplate source file
#include "../source/rubik_api.cpp"

#endif  //  ! ___RUBIK_API__H
