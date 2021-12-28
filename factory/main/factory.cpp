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
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( const Pattern<N> & pattern, AcceptFunction af )
{
  static size_t index = 0;
  Subgroup next;
  for ( auto P : m_subgroups )
  {
    CubeID trans = 0;
    if ( P.getTransposition( pattern, trans ) )
    {
      return Guide( P, af, index ++, trans );
    }
  }

  next = Subgroup( pattern );
  m_subgroups.push( next );

  return Guide( next, af, index ++, 0 );
}

#endif  //  ! FACTORY_CREATE__H
