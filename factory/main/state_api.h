#ifndef STATE_API_TPP
#define STATE_API_TPP

#include <state_factory.h>

template< cube_size N >
class Factory<N>::StateAPI: protected Factory<N>::StateFactory
{

public:
  BitMap gradient;
  BitMap target;

  // pool data
  BitMap pGradient;
  BitMap pGarget;

  void initGradient( const RotID rotID )
  {
    gradient.set( StateFactory::AllowedRotations[ rotID ] );
  }
};

#endif  //  ! STATE_API_TPP
