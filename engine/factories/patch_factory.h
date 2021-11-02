#ifndef PATCH_FACTORY__H
#define PATCH_FACTORY__H

#include <base_types.h>
#include <smart_array.h>

class FPatch
{

public:
  FPatch() = default;
  ~FPatch();

  pArray< PosID > create( const PosID *, const size_t );
};


pArray< PosID > FPatch::create( const PosID * ref, const size_t size )
{
  PosID * refPos = new PosID [ size ];
  for ( size_t i = 0; i < size; ++ i )
  {
    refPos[i] = ref[i];
  }
  return std::make_shared< Array< PosID > >( size, refPos );
}

FPatch::~FPatch()
{
}

#endif  //  ! PATCH_FACTORY__H
