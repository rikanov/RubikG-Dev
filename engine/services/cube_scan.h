#ifndef ___CUBE_SCANNER__H
#define ___CUBE_SCANNER__H

#include <factory_tree.h>
#include <rubik.h>

template< cube_size N >
struct Scan
{
  using PatchAPI = const typename GuideFactory<N>::PatchAPI;

  CubeID GetPrior( const Rubik<N> & Cube, PatchAPI & patch, const CubeID trans = 0 )
  {
    return Cube.transpose( patch.getPriorPos(), trans );
  }

  GroupID GetState( const Rubik<N> & Cube, PatchAPI & patch, const CubeID trans = 0 )
  {
    BitMapID result = 0;
    const CubeID invPrior = Simplex::Inverse( GetPrior( Cube, patch, trans ) );
    for ( int id = 0; id < patch.size() - 1; ++ id )
    {
      result += Simplex::Composition( Cube.transpose( patch.getPosID( id ), trans ), invPrior ) * pow24( id );
    }
    return result;
  }


};

#endif  //  ! ___CUBE_SCANNER__H
