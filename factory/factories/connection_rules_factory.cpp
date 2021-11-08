#ifndef CONNECTION_RULES_FACTORY__H
#define CONNECTION_RULES_FACTORY__H

#include <cube_rotations.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Connection: public Factory<N>::SubgroupAPI
{
  void init();

protected:
  cArray<CubeID> m_rotatePriorCube;

public:
  Connection( const size_t size, const PosID * pos );
};

template< cube_size N >
Factory<N>::Connection::Connection( const size_t size, const PosID* pos )
  : SubgroupAPI( size, pos )
{
}

template< cube_size N >
void Factory<N>::Connection::init()
{
  constexpr size_t size = CRotations<N>::AllRotIDs;
  Array<CubeID> priorCube = MakeArray( new CubeID[ size ] );
  auto raw = priorCube.get();
  all_rotid( rotID, N )
  {
    raw[rotID] = this -> priorMoving( rotID ) ? CRotations<N>::GetTilt( rotID ) : 0;
  }
  m_rotatePriorCube = priorCube;
}
#endif  //  ! CONNECTION_RULES_FACTORY__H
