#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <guide.h>


template< cube_size N >
int GuideFactory<N>::m_index = 0;

template< cube_size N >
GuideFactory<N>::GuideFactory()
{
}

template< cube_size N >
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( const size_t size, const PosID * pattern, AcceptFunction af )
{
  return Guide( size, pattern, af, m_index ++ );
}

#endif  //  ! FACTORY_CREATE__H
