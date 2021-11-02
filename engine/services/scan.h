#ifndef SCANNER__H
#define SCANNER__H

#include <rubik.h>

namespace Scan
{
  template< cube_size N >
  CubeID GetPrior( const Rubik<N> & Cube, const PatchAPI<N> & patch, const CubeID trans = 0 )
  {
    return Cube.transpose( patch.getPriorPos(), trans );
  }

  template< cube_size N >
  GroupID GetState( const Rubik<N> & Cube, const PatchAPI<N> & patch, const CubeID trans = 0 )
  {
    BitMapID result = 0;
    const CubeID invPrior = Simplex::Inverse( GetPrior( Cube, patch, trans ) );
    for ( int id = 0; id < patch.size() - 1; ++ id )
    {
      result += Simplex::Composition( Cube.transpose( patch.getPosID( id ), trans ), invPrior ) * pow24( id );
    }
    return result;
  }
}
#endif  //  ! SCANNER__H
