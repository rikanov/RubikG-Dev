#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <factory_tree.h>

template< cube_size N >
void Factory<N>::create( const size_t size, const PosID* startPos, AcceptFunction af)
{
  EvaluatorAPI test( size, startPos );
  clog( "group size:", test.groupSize() );
  clog( "LookUP:", (int)test.distance(1313), test.lookUp( 1313, 1) );
  Array<int> a;
  clog( "is nullptr ? ", nullptr == a.get() );
  BitMap::Print( test.gradient( 0, 1313, 4 ), 9 * N + 1, 3 * N );
  BitMap::Print( test.target  ( 0, 1313, 4 ), 24, 4 );
  Factory<N>::Progress tprog;
  clog( "constructed" );
  tprog.setMaximumLevel( 7 );
}

#endif  //  ! FACTORY_CREATE__H
