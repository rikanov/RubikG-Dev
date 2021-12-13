#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <guide.h>


template< cube_size N >
size_t GuideFactory<N>::m_index = 0;

template< cube_size N >
GuideFactory<N>::GuideFactory()
{
}

template< cube_size N >
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( Pattern<N> pattern, AcceptFunction af )
{
  return Guide( pattern, af, m_index ++ );
}

#endif  //  ! FACTORY_CREATE__H
