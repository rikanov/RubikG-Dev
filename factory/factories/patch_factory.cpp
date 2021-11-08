#ifndef FACTORY_PATCH__H
#define FACTORY_PATCH__H

#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Patch
{
protected:
  const size_t  m_patchSize;
  cArray<PosID> m_patch;

  Patch( const size_t size, const PosID * );
};


template< cube_size N >
Factory<N>::Patch::Patch( const size_t size, const PosID * ref )
  : m_patchSize( size )
{
  Array<PosID> sharedPatch = MakeArray<PosID> ( new PosID [ size ] );
  auto refPos = sharedPatch.get();
  for ( size_t i = 0; i < size; ++ i )
  {
    refPos[i] = ref[i];
  }
  m_patch = sharedPatch;
}

#endif  //  ! FACTORY_PATCH__H
