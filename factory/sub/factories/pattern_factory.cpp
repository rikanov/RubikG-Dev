#ifndef FACTORY_PATCH__H
#define FACTORY_PATCH__H

#include <dynamic_array.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Pattern
{
protected:
  Array<PosID> m_pattern;

  Pattern();
  Pattern( const size_t size, const PosID * );
};


template< cube_size N >
Factory<N>::Pattern::Pattern()
{
}

template< cube_size N >
Factory<N>::Pattern::Pattern( const size_t size, const PosID * ref )
  : m_pattern( size )
{
  for ( size_t i = 0; i < size; ++ i )
  {
    m_pattern[i] = ref[i];
  }
}

#endif  //  ! FACTORY_PATCH__H
