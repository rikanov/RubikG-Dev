#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <guide.h>

template< cube_size N >
GuideFactory<N>::GuideFactory()
  : m_subgroups( Node::MaxPatterns )
{
}

template< cube_size N >
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( Pattern<N> pattern, AcceptFunction af )
{
  m_subgroups.push( Subgroup( pattern ) );

  return Guide( m_subgroups.top(), af, m_subgroups.size() - 1, 0 );
}

#endif  //  ! FACTORY_CREATE__H
