#ifndef CONNECTION_RULES_FACTORY__H
#define CONNECTION_RULES_FACTORY__H

#include <cube_rotations.h>
#include <factory_tree.h>

template< cube_size N >
class Factory<N>::Connection: public Factory<N>::SubgroupAPI
{
  void init();

protected:
  Array<CubeID> m_rotatePriorCube;

public:
  Connection() = default;
  Connection( const size_t size, const PosID * pos );
};

template< cube_size N >
Factory<N>::Connection::Connection( const size_t size, const PosID* pos )
  : SubgroupAPI( size, pos )
  , m_rotatePriorCube( CRotations<N>::AllRotIDs )
{
  init();
}

template< cube_size N >
void Factory<N>::Connection::init()
{
  all_rotid( rotID, N )
  {
    m_rotatePriorCube[rotID] = this -> priorMoving( rotID ) ? CRotations<N>::GetTilt( rotID ) : 0;
  }
}
#endif  //  ! CONNECTION_RULES_FACTORY__H
