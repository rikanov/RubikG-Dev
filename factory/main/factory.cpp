#ifndef FACTORY_CREATE__H
#define FACTORY_CREATE__H

#include <bitmap_set.h>
#include <guide.h>

template< cube_size N >
GuideFactory<N>::GuideFactory()
  : m_guides( Node::MaxPatterns )
{
}

template< cube_size N >
typename GuideFactory<N>::Guide GuideFactory<N>::getGuide( const Pattern<N> & pattern, AcceptFunction af )
{
  static size_t index = 0;
  for ( auto P : m_guides )
  {
    CubeID trans = 0;
    if ( P.isTransposed( pattern, trans ) )
    {
      Guide result;
      if( Accept<N>::Equal( P.acceptance(), af ) )
      {
        result = P;
        result.rebase( trans );
        result.index( index ++ );
      }
      else
      {
        result = Guide( P, af, index ++, trans );
      }
      return result;
    }
  }

  Guide next( pattern, af, index ++, 0 );
  m_guides.push( next );

  return next;
}

#endif  //  ! FACTORY_CREATE__H
