#ifndef SCANNER__H
#define SCANNER__H

#include <rubik.h>

namespace Scan
{
  template< cube_size N >
  CubeID GetPrior( const Rubik<N> & Cube, const CubeID prior, const CubeID trans = 0 ) const
  {
    return Cube.transpose( prior, trans );
  }

  template< cube_size N >
  GroupID GetState( const Rubik<N> & Cube, const PosID * startPositions, const size_t size, const CubeID trans = 0 )
  {
    BitMapID result = 0;
    const CubeID invPrior = Simplex::Inverse( getPrior( Cube, trans ) );
    for ( int id = 0; id < m_size - 1; ++ id )
    {
      result += Simplex::Composition( Cube.transpose( startPositions[id], trans ), invPrior ) * pow24( id );
    }
    return result;
  }
}
#endif  //  ! SCANNER__H
