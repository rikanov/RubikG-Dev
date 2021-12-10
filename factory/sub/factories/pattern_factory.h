#ifndef __PATTERN_FACTORY_H
#define __PATTERN_FACTORY_H

#include <dynamic_array.h>
#include <factory_tree.h>

template< cube_size N >
class GuideFactory<N>::Pattern
{
protected:
  Array<PosID> m_pattern;

  Pattern();
  Pattern( const size_t size, const PosID * );
};


template< cube_size N >
GuideFactory<N>::Pattern::Pattern()
{
}

template< cube_size N >
GuideFactory<N>::Pattern::Pattern( const size_t size, const PosID * ref )
  : m_pattern( size )
{
  for ( size_t i = 0; i < size; ++ i )
  {
    m_pattern[i] = ref[i];
  }
}

#endif  //  ! __PATTERN_FACTORY_H