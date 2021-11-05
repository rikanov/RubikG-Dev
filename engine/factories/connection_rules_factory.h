#ifndef CONNECTION_RULES_FACTORY__H
#define CONNECTION_RULES_FACTORY__H

#include <patch_api.h>
#include <cube_rotations.h>

template< cube_size N >
class FConnections
{
  PatchAPI<N> m_patch;

public:
  pArray<CubeID> create( pArray<PosID> p );
};

template< cube_size N >
pArray<CubeID> FConnections<N>::create( pArray<PosID> p )
{
  m_patch.init( p );
  constexpr size_t size = CRotations<N>::AllRotIDs;
  CubeID * priorCube = new CubeID[ size ];
  all_rotid( rotID, N )
  {
    priorCube[rotID] = m_patch.priorMoving( rotID ) ? CRotations<N>::GetTilt( rotID ) : 0;
  }
  return std::make_shared< Array<CubeID> >( size, priorCube, m_patch.priorRotIDs() );
}
#endif  //  ! CONNECTION_RULES_FACTORY__H