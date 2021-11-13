#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <worker.h>
#include <factory_tree.h>


template< cube_size N >
Factory<N>::Factory()
  : patternSize     ( 0 )
  , patternTrans    ( 0 )
  , pattern      ( nullptr )
  , accept  ( Accept<N>::Normal )
{
  
}

template< cube_size N >
typename Factory<N>::Worker Factory<N>::get( ControlPanel & cPanel )
{
  return Worker( cPanel, EvaluatorAPI( patternSize, pattern, accept ) );
}

#endif  //  ! FACTORY_CREATE__H
