#include <acceptance.h>


AcceptFunction operator | ( AcceptFunction A, AcceptFunction B )
{
  return [ &A, &B ]( PosID posID ) -> BitMap32ID { return A( posID ) | B( posID ) ;  };
}

AcceptFunction operator & ( AcceptFunction A, AcceptFunction B )
{
  return [ &A, &B ]( PosID posID ) -> BitMap32ID { return A( posID ) & B( posID ) ;  };
}
