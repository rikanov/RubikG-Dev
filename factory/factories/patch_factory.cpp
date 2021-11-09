#ifndef FACTORY_PATCH__H
#define FACTORY_PATCH__H

#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Patch
{
protected:
  Array<PosID> m_patch;

  Patch();
  Patch( const size_t size, const PosID * );
};


template< cube_size N >
Factory<N>::Patch::Patch()
{
}

template< cube_size N >
Factory<N>::Patch::Patch( const size_t size, const PosID * ref )
  : m_patch( size )
{
  for ( size_t i = 0; i < size; ++ i )
  {
    m_patch[i] = ref[i];
  }
}

#endif  //  ! FACTORY_PATCH__H
