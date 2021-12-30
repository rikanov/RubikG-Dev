#ifndef ___RUBIK__H
#define ___RUBIK__H

#include <rubik_api.h>

template< cube_size N >
class Rubik: public RubikAPI<N>
{
public:
  bool integrity() const;
  size_t shuffle( const size_t depth = 0 );
};

// include template source file
#include "../source/rubik.cpp"

#endif  //  ! ___RUBIK__H
