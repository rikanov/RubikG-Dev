#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <progress.h>
#include <factory_tree.h>


template< cube_size N >
Factory<N>::Factory()
  : cube         ( nullptr )
  , patternSize     ( 0 )
  , patternTrans    ( 0 )
  , pattern      ( nullptr )
  , accept  ( Accept<N>::Normal )
{
  
}

template< cube_size N >
typename Factory<N>::Task * Factory<N>::get()
{
    const PosID * left= new PosID[4] {
      CPositions<2>::GetPosID( 0, 0, 0 ),
      CPositions<2>::GetPosID( 0, 0, 1 ),
      CPositions<2>::GetPosID( 0, 1, 1 ),
      CPositions<2>::GetPosID( 0, 1, 0 ),
    };
  clog( (long) cube, patternSize, (int) patternTrans, (long) pattern );
//  Progress<3> test;
  PatternAPI ea( 4, left );
  std::shared_ptr<uint8_t> m_pattern;
  //test.addWorker( 4, left );
  return nullptr;
}

#endif  //  ! FACTORY_CREATE__H
