#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <signpost.h>
#include <guide.h>


template< cube_size N >
GuideFactory<N>::GuideFactory()
{
}

template< cube_size N >
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( Task<N> & task ) const
{
  return Guide( task );
}

template< cube_size N >
Signpost * GuideFactory<N>::actual()
{
  return m_stack + m_level;
}
#endif  //  ! FACTORY_CREATE__H
