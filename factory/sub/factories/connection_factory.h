#ifndef ___CONNECTION_RULES_FACTORY__H
#define ___CONNECTION_RULES_FACTORY__H

#include <cube_rotations.h>
#include <node_chart.h>
#include <factory_tree.h>

template< cube_size N >
class GuideFactory<N>::Connection
{
  Pattern<N> m_pattern;
  void init();

protected:
  Array<CubeID> m_rotatePriorCube;

public:
  Connection();
  Connection( Pattern<N> pattern );
};


template< cube_size N >
GuideFactory<N>::Connection::Connection()
{
}

template< cube_size N >
GuideFactory<N>::Connection::Connection( Pattern<N> pattern )
  : m_pattern ( pattern )
  , m_rotatePriorCube( CRotations<N>::AllRotIDs )
{
  init();
}

template< cube_size N >
void GuideFactory<N>::Connection::init()
{
  all_rotid( rotID, N )
  {
    m_rotatePriorCube[rotID] = m_pattern.priorMoving( rotID ) ? CRotations<N>::GetTilt( rotID ) : 0;
  }
}
#endif  //  ! ___CONNECTION_RULES_FACTORY__H
